//-----------------------------------------------------------------------------
// File: ComponentVerilogWriter.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 01.08.2014
//
// Description:
// Class for writing a component as a Verilog module.
//-----------------------------------------------------------------------------

#include "ComponentVerilogWriter.h"

#include <Plugins/VerilogGenerator/ModelParameterVerilogWriter/ModelParameterVerilogWriter.h>
#include <Plugins/VerilogGenerator/PortVerilogWriter/PortVerilogWriter.h>

#include <Plugins/VerilogGenerator/CommentWriter/CommentWriter.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/BusInterface.h>
#include <IPXACTmodels/Component/Port.h>
#include <IPXACTmodels/common/ModuleParameter.h>
#include <IPXACTmodels/common/VLNV.h>

#include <QSharedPointer>
#include "IPXACTmodels/Component/View.h"
#include "IPXACTmodels/Component/ComponentInstantiation.h"

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::ComponentVerilogWriter
//-----------------------------------------------------------------------------
ComponentVerilogWriter::ComponentVerilogWriter(QSharedPointer<Component> component, QString const& activeView,
    QSharedPointer<const PortSorter> sorter, QSharedPointer<ExpressionFormatter> expressionFormatter) :
component_(component),
activeView_(activeView),
sorter_(sorter),
childWriters_(),
formatter_(expressionFormatter)
{

}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::~ComponentVerilogWriter
//-----------------------------------------------------------------------------
ComponentVerilogWriter::~ComponentVerilogWriter()
{
    childWriters_.clear();
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::write
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::write(QTextStream& outputStream) const
{
    if (nothingToWrite())
    {
        return;
    }

    writeModuleDeclaration(outputStream);

    writeInternalWiresAndComponentInstances(outputStream);

    writeModuleEnd(outputStream);
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::nothingToWrite
//-----------------------------------------------------------------------------
bool ComponentVerilogWriter::nothingToWrite() const
{
    return component_.isNull();
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::writeModuleDeclaration()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writeModuleDeclaration( QTextStream& outputStream ) const
{
    outputStream << "module " << component_->getVlnv().getName();
    
    writeParameterDeclarations(outputStream);

    writePortDeclarations(outputStream);     
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::portNames()
//-----------------------------------------------------------------------------
QString ComponentVerilogWriter::portNames() const
{
    return sorter_->sortedPortNames(component_).join(", ");
}

//-----------------------------------------------------------------------------
// Function: VerilogComponentVerilogWriterGenerator::writeParameterDeclarations()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writeParameterDeclarations(QTextStream& outputStream) const
{
	QSharedPointer<View> view;

	foreach( QSharedPointer<View> currentView, *component_->getViews() )
	{
		if ( currentView->name() == activeView_ )
		{
			view = currentView;
			break;
		}
	}

	if ( view )
	{
		foreach( QSharedPointer<ComponentInstantiation> currentInsta, *component_->getComponentInstantiations() )
		{
			if ( currentInsta->name() == view->getComponentInstantiationRef() )
			{
				// Create a new list of module parameters.
				auto parametersToWrite = QSharedPointer<QList<QSharedPointer<ModuleParameter> > >
					( new QList<QSharedPointer<ModuleParameter> > );
				
				// Go through existing ones on the instance.
				foreach ( QSharedPointer<ModuleParameter> parameterAdd, *currentInsta->getModuleParameters() )
				{
					// If true, the parameter is appended to the end of list.
					bool append = true;

					// Go through parameters that have been so far appended to the list.
					for ( auto parameterCmp = parametersToWrite->begin();
						 parameterCmp != parametersToWrite->end(); ++parameterCmp )
					{
						// Resolve the value of the formerly appended item.
						QString formatted = this->formatter_->
							 formatReferringExpression((*parameterCmp)->getValue());

						// If it contains a reference to the inspected parameter, the inspected parameter comes
						// before it is referred.
						if ( formatted.contains(parameterAdd->name()) )
						{
							parametersToWrite->insert( parameterCmp, parameterAdd );

							// It will not be inserted twice, so break here.
							append = false;
							break;
						}
					 }

					// If the parameter was not needed anywhere, it is appended to the end of the list.
					if ( append )
					{
						parametersToWrite->append( parameterAdd );
					}
				}

				if (!parametersToWrite->isEmpty())
				{
					outputStream << " #(" << endl;

					foreach(QSharedPointer<ModuleParameter> parameter, *parametersToWrite)
					{
						bool isLastParameter = parameter == parametersToWrite->last();
						writeParameter(outputStream, parameter, isLastParameter);
					}

					outputStream << ") ";
				}

				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::writeParameter()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writeParameter(QTextStream& outputStream, QSharedPointer<ModuleParameter> parameter,
    bool isLast) const
{
    outputStream << indentation();
    ModuleParameterVerilogWriter parameterWriter(parameter, formatter_);
    parameterWriter.write(outputStream);

    if (!isLast)
    {
        outputStream << ",";
    }

    CommentWriter descriptionWriter(parameter->description());
    descriptionWriter.setIndent(4);
    descriptionWriter.write(outputStream);

    if (parameter->description().isEmpty())
    {
        outputStream << endl;
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::indentation()
//-----------------------------------------------------------------------------
QString ComponentVerilogWriter::indentation() const
{
    return "    ";
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::writePortDeclarations()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writePortDeclarations(QTextStream& outputStream) const
{
    QString previousInterfaceName = "";

    outputStream << "(";

    QStringList ports = sorter_->sortedPortNames(component_);
    foreach(QString portName, ports)
	{    
        QSharedPointer<QList<QSharedPointer<BusInterface> > > busInterfaces =
            component_->getInterfacesUsedByPort(portName);

        if (busInterfaces->size() == 1)
		{
            writeInterfaceIntroduction(busInterfaces->first()->name(), previousInterfaceName, outputStream);
		}
        else if (!busInterfaces->isEmpty())
        {
            writeInterfaceIntroduction("several", previousInterfaceName, outputStream);
        }
		else
		{
			writeInterfaceIntroduction("none", previousInterfaceName, outputStream);
		}

        bool lastPortToWrite = portName == ports.last();
        writePort(outputStream, component_->getPort(portName), lastPortToWrite);
    }
    
    outputStream << ");" << endl;
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::writeInterfaceIntroduction()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writeInterfaceIntroduction(QString const& interfaceName, QString& previousInterfaceName,
    QTextStream& outputStream) const
{
    if (previousInterfaceName.compare(interfaceName) != 0)
    {
        outputStream << endl;

        if (interfaceName == "none")
        {
            outputStream << indentation() << "// These ports are not in any interface" << endl;                
        }
        else if (interfaceName == "several")
        {
            outputStream << indentation() << "// There ports are contained in many interfaces" << endl;       
        }
        else
        {
            outputStream << indentation() << "// Interface: " << interfaceName << endl;

            QSharedPointer<const BusInterface> busInterface = component_->getBusInterface(interfaceName);
            CommentWriter descriptionWriter(busInterface->description());
            descriptionWriter.setIndent(4);
            descriptionWriter.write(outputStream);
        }        
        previousInterfaceName = interfaceName;                
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::writePort()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writePort(QTextStream& outputStream, QSharedPointer<Port> port, bool isLast) const
{
    outputStream << indentation();

    PortVerilogWriter writer(port, formatter_);
    writer.write(outputStream);

    if (!isLast)
    {
        outputStream << ",";
    }

    CommentWriter descriptionWriter(port->description());
    descriptionWriter.setIndent(4);
    descriptionWriter.write(outputStream);

    if (port->description().isEmpty())
    {
        outputStream << endl;
    }
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::writeInternalWiresAndComponentInstances()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writeInternalWiresAndComponentInstances(QTextStream& outputStream) const
{
    outputStream << endl;

    WriterGroup::write(outputStream);
}

//-----------------------------------------------------------------------------
// Function: ComponentVerilogWriter::writeModuleEnd()
//-----------------------------------------------------------------------------
void ComponentVerilogWriter::writeModuleEnd(QTextStream& outputStream) const
{
    outputStream << "endmodule" << endl;
}
