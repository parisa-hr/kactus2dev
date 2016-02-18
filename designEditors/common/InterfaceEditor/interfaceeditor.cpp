//-----------------------------------------------------------------------------
// File: interfaceeditor.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Antti Kamppi
// Date: 10.10.2011
//
// Description:
// Editor to display/edit the details of a bus interface.
//-----------------------------------------------------------------------------

#include "interfaceeditor.h"

#include <IPXACTmodels/generaldeclarations.h>

#include <IPXACTmodels/common/DirectionTypes.h>
#include <IPXACTmodels/common/Vector.h>

#include <IPXACTmodels/AbstractionDefinition/AbstractionDefinition.h>
#include <IPXACTmodels/kactusExtensions/ApiInterface.h>
#include <IPXACTmodels/kactusExtensions/ComInterface.h>
#include <IPXACTmodels/kactusExtensions/ComDefinition.h>
#include <IPXACTmodels/kactusExtensions/ComProperty.h>

#include <IPXACTmodels/Component/BusInterface.h>
#include <IPXACTmodels/Component/PortMap.h>
#include <IPXACTmodels/Component/Port.h>

#include <common/GenericEditProvider.h>
#include <common/graphicsItems/ComponentItem.h>

#include <IPXACTmodels/validators/namevalidator.h>

#include <designEditors/common/DesignDiagram.h>
#include <designEditors/HWDesign/HWConnectionEndpoint.h>
#include <designEditors/HWDesign/HWChangeCommands.h>

#include <editors/ComponentEditor/common/ComponentParameterFinder.h>
#include <editors/ComponentEditor/common/IPXactSystemVerilogParser.h>

#include <QVBoxLayout>
#include <QStringList>
#include <QBrush>
#include <QHeaderView>
#include <QDockWidget>

//! The maximum height for the description editor.
static const int MAX_DESC_HEIGHT = 50;

//-----------------------------------------------------------------------------
// Function: interfaceeditor::InterfaceEditor()
//-----------------------------------------------------------------------------
InterfaceEditor::InterfaceEditor(QWidget *parent, LibraryInterface* handler):
QWidget(parent),
type_(this),
absType_(this),
nameEdit_(this),
nameLabel_(tr("Interface name"), this),
modeEdit_(this),
modeLabel_(tr("Interface mode"), this),
dependencyDirLabel_(tr("Dependency direction"), this),
dependencyDirCombo_(this),
comDirectionLabel_(tr("Direction"), this),
comDirectionCombo_(this),
transferTypeLabel_(tr("Transfer type"), this),
transferTypeCombo_(this),
interface_(NULL),
comDef_(),
mappingsLabel_(tr("Port map"), this),
mappings_(this),
descriptionLabel_(tr("Description"), this),
descriptionEdit_(this),
propertyValueLabel_(tr("Property values"), this),
propertyValueEditor_(this),
dummyWidget_(this),
handler_(handler)
{
	Q_ASSERT(parent);
	Q_ASSERT(handler);

	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

	type_.setTitle(tr("Bus type VLNV"));
	type_.setFlat(false);
	absType_.setTitle(tr("Abstraction type VLNV"));
	absType_.setFlat(false);

	// set validator for interface name editor
    nameEdit_.setValidator(new NameValidator(&nameEdit_));

	// set the possible modes to the mode editor
	QStringList modes;
	modes.append("master");
	modes.append("slave");
	modes.append("system");
	modes.append("mirroredMaster");
	modes.append("mirroredSlave");
	modes.append("mirroredSystem");
	modes.append("monitor");
	modes.append("undefined");
	modeEdit_.addItems(modes);

    // Set possible modes to the direction editors.
    dependencyDirCombo_.addItem("requester");
    dependencyDirCombo_.addItem("provider");

    comDirectionCombo_.addItem("in");
    comDirectionCombo_.addItem("out");
    comDirectionCombo_.addItem("inout");

	// There are always 2 columns.
	mappings_.setColumnCount(2);
	mappings_.setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Logical name")));
	mappings_.setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Physical name")));
	mappings_.horizontalHeader()->setStretchLastSection(true);
	mappings_.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	mappings_.verticalHeader()->hide();
	mappings_.setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked |
		QAbstractItemView::EditKeyPressed);

	// set the maximum height for the description editor
	descriptionEdit_.setMaximumHeight(MAX_DESC_HEIGHT);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(&type_);
	layout->addWidget(&absType_);
	layout->addWidget(&nameLabel_);
	layout->addWidget(&nameEdit_);
	layout->addWidget(&modeLabel_);
	layout->addWidget(&modeEdit_);
    layout->addWidget(&dependencyDirLabel_);
    layout->addWidget(&dependencyDirCombo_);
    layout->addWidget(&transferTypeLabel_);
    layout->addWidget(&transferTypeCombo_);
    layout->addWidget(&comDirectionLabel_);
    layout->addWidget(&comDirectionCombo_);
	layout->addWidget(&descriptionLabel_);
	layout->addWidget(&descriptionEdit_);
	layout->addWidget(&mappingsLabel_);
    layout->addWidget(&mappings_, 1);
    layout->addWidget(&propertyValueLabel_);
    layout->addWidget(&propertyValueEditor_, 1);
    layout->addWidget(&dummyWidget_, 1);

	clear();
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::~InterfaceEditor()
//-----------------------------------------------------------------------------
InterfaceEditor::~InterfaceEditor()
{

}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::setInterface()
//-----------------------------------------------------------------------------
void InterfaceEditor::setInterface( ConnectionEndpoint* interface )
{
	Q_ASSERT(interface);

	parentWidget()->raise();

	// disconnect the previous interface
	if (interface_)
    {
		disconnect(interface_, SIGNAL(destroyed(ConnectionEndpoint*)), this, SLOT(clear()));
		disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

        comDef_.clear();
	}

	interface_ = interface;

    // set text for the name editor, signal must be disconnected when name is set to avoid loops 
    disconnect(&nameEdit_, SIGNAL(textEdited(const QString&)), this, SLOT(onInterfaceNameChanged(const QString&)));
    nameEdit_.setText(interface->name());
    connect(&nameEdit_, SIGNAL(textEdited(const QString&)),
        this, SLOT(onInterfaceNameChanged(const QString&)), Qt::UniqueConnection);

    // display the current description of the interface.
    disconnect(&descriptionEdit_, SIGNAL(textChanged()), this, SLOT(onDescriptionChanged()));
    descriptionEdit_.setPlainText(interface->description());
    connect(&descriptionEdit_, SIGNAL(textChanged()), this, SLOT(onDescriptionChanged()), Qt::UniqueConnection);

    // Fill the rest of the editors based on the interface type.
    if (interface->isBus())
    {
        Q_ASSERT(interface->getBusInterface());
        type_.setTitle(tr("Bus type VLNV"));
	    type_.setVLNV(interface->getBusInterface()->getBusType(), true);
        if (interface->getBusInterface()->getAbstractionTypes() && 
            !interface->getBusInterface()->getAbstractionTypes()->isEmpty() &&
            interface->getBusInterface()->getAbstractionTypes()->first()->getAbstractionRef())
        {
            absType_.setVLNV(*interface->getBusInterface()->getAbstractionTypes()->first()->getAbstractionRef(),
                true);
        }

	    // set selection for mode editor, signal must be disconnected when mode is set to avoid loops 
	    disconnect(&modeEdit_, SIGNAL(currentIndexChanged(const QString&)),
		    this, SLOT(onInterfaceModeChanged(const QString&)));
	    // select the correct mode for mode editor
	    int index = modeEdit_.findText(General::interfaceMode2Str(
            interface->getBusInterface()->getInterfaceMode()));

	    modeEdit_.setCurrentIndex(index);
	    connect(&modeEdit_, SIGNAL(currentIndexChanged(const QString&)),
		    this, SLOT(onInterfaceModeChanged(const QString&)), Qt::UniqueConnection);

	    // set port maps to be displayed in the table widget.
	    setPortMaps();
    }
    else if (interface->isApi())
    {
        Q_ASSERT(interface->getApiInterface());
        type_.setTitle(tr("API type VLNV"));
        type_.setVLNV(interface->getApiInterface()->getApiType(), true);

        // Set selection for dependency direction.
        disconnect(&dependencyDirCombo_, SIGNAL(currentIndexChanged(const QString&)),
                   this, SLOT(onDependencyDirectionChanged(QString const&)));

        int index = dependencyDirCombo_.findText(dependencyDirection2Str(
            interface->getApiInterface()->getDependencyDirection()));
        dependencyDirCombo_.setCurrentIndex(index);

        connect(&dependencyDirCombo_, SIGNAL(currentIndexChanged(const QString&)),
                this, SLOT(onDependencyDirectionChanged(QString const&)), Qt::UniqueConnection);
    }
    else if (interface->isCom())
    {
        Q_ASSERT(interface->getComInterface());
        type_.setTitle(tr("COM type VLNV"));
        type_.setVLNV(interface->getComInterface()->getComType(), true);

        // Retrieve the COM definition.
        if (interface->getComInterface()->getComType().isValid())
        {
            QSharedPointer<Document const> libComp =
                handler_->getModelReadOnly(interface->getComInterface()->getComType());
            comDef_ = libComp.dynamicCast<ComDefinition const>();
        }

        // Fill in the possible values of the data type.
        disconnect(&transferTypeCombo_, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(onComTransferTypeChanged(QString const&)));

        transferTypeCombo_.clear();
        transferTypeCombo_.addItem("");

        if (comDef_ != 0)
        {
            transferTypeCombo_.addItems(*comDef_->getTransferTypes());

            // Set selection for the data type.
            QString const& transferType = interface->getComInterface()->getTransferType();

            if (comDef_->getTransferTypes()->contains(transferType))
            {
                transferTypeCombo_.setCurrentIndex(transferTypeCombo_.findText(transferType));
            }
        }

        connect(&transferTypeCombo_, SIGNAL(currentIndexChanged(const QString&)),
                this, SLOT(onComTransferTypeChanged(QString const&)), Qt::UniqueConnection);

        // Set selection for COM direction.
        disconnect(&comDirectionCombo_, SIGNAL(currentIndexChanged(const QString&)),
                   this, SLOT(onComDirectionChanged(QString const&)));

        int index = comDirectionCombo_.findText(DirectionTypes::direction2Str(
            interface->getComInterface()->getDirection()));
        comDirectionCombo_.setCurrentIndex(index);

        connect(&comDirectionCombo_, SIGNAL(currentIndexChanged(const QString&)),
                this, SLOT(onComDirectionChanged(QString const&)), Qt::UniqueConnection);

        // Set property values and try to read the allowed properties from the COM definition if valid.
        disconnect(&propertyValueEditor_, SIGNAL(contentChanged()), this, SLOT(onComPropertyValuesChanged()));

        propertyValueEditor_.setData(interface->getComInterface()->getPropertyValues());

        if (comDef_ != 0)
        {
            propertyValueEditor_.setAllowedProperties(*comDef_->getProperties());
        }

        connect(&propertyValueEditor_, SIGNAL(contentChanged()),
                this, SLOT(onComPropertyValuesChanged()), Qt::UniqueConnection);
    }
    else
    {
        type_.setTitle(tr("Type VLNV"));
    }

	connect(interface, SIGNAL(destroyed(ConnectionEndpoint*)),
		this, SLOT(clear()), Qt::UniqueConnection);
	connect(interface_, SIGNAL(contentChanged()), 
		this, SLOT(refresh()), Qt::UniqueConnection);

    bool locked = static_cast<DesignDiagram*>(interface->scene())->isProtected();

    // Allow editing of values only in unlocked mode when the interface is a hierarchical one
    // or unpackaged.
	if (!locked && (interface->isHierarchical() ||
                    (interface->encompassingComp() != 0 &&
                     !interface->encompassingComp()->componentModel()->getVlnv().isValid())))
    {
		nameEdit_.setEnabled(true);
		modeEdit_.setEnabled(true);
        dependencyDirCombo_.setEnabled(true);
        comDirectionCombo_.setEnabled(true);
		mappings_.setEnabled(true);
		mappings_.setEditTriggers(QAbstractItemView::NoEditTriggers);
		descriptionEdit_.setEnabled(true);
        propertyValueEditor_.setEnabled(true);
        transferTypeCombo_.setEnabled(true);
	}
	else
    {
		nameEdit_.setDisabled(true);
		modeEdit_.setDisabled(true);
        dependencyDirCombo_.setDisabled(true);
        comDirectionCombo_.setDisabled(true);
		descriptionEdit_.setDisabled(true);
        propertyValueEditor_.setDisabled(true);
        transferTypeCombo_.setDisabled(true);

		mappings_.setEditTriggers(QAbstractItemView::NoEditTriggers);
	}

	// show the editors
    type_.show();
	nameEdit_.show();
	nameLabel_.show();
	descriptionLabel_.show();
	descriptionEdit_.show();

    absType_.setVisible(interface->isBus());
    modeEdit_.setVisible(interface->isBus());
    modeLabel_.setVisible(interface->isBus());
    mappingsLabel_.setVisible(interface->isBus());
    mappings_.setVisible(interface->isBus());

    dependencyDirCombo_.setVisible(interface->isApi());
    dependencyDirLabel_.setVisible(interface->isApi());
    dummyWidget_.setVisible(!interface->isCom() && !interface->isBus());

    transferTypeCombo_.setVisible(interface->isCom());
    transferTypeLabel_.setVisible(interface->isCom());
    comDirectionCombo_.setVisible(interface->isCom());
    comDirectionLabel_.setVisible(interface->isCom());
    propertyValueLabel_.setVisible(interface->isCom());
    propertyValueEditor_.setVisible(interface->isCom());

	parentWidget()->setMaximumHeight(QWIDGETSIZE_MAX);
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::clear()
//-----------------------------------------------------------------------------
void InterfaceEditor::clear()
{
	if (interface_)
    {
		disconnect(interface_, SIGNAL(destroyed(ConnectionEndpoint*)), this, SLOT(clear()));
		disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));
		interface_ = 0;
        comDef_.clear();
	}

	disconnect(&nameEdit_, SIGNAL(textEdited(const QString&)), this, SLOT(onInterfaceNameChanged(const QString&)));
	disconnect(&modeEdit_, SIGNAL(currentIndexChanged(const QString&)),
		this, SLOT(onInterfaceModeChanged(const QString&)));
    disconnect(&dependencyDirCombo_, SIGNAL(currentIndexChanged(const QString&)),
        this, SLOT(onDependencyDirectionChanged(QString const&)));
    disconnect(&comDirectionCombo_, SIGNAL(currentIndexChanged(const QString&)),
        this, SLOT(onComDirectionChanged(QString const&)));
    disconnect(&transferTypeCombo_, SIGNAL(currentIndexChanged(const QString&)),
        this, SLOT(onComTransferTypeChanged(QString const&)));
    disconnect(&propertyValueEditor_, SIGNAL(contentChanged()), this, SLOT(onComPropertyValuesChanged()));
	disconnect(&descriptionEdit_, SIGNAL(textChanged()), this, SLOT(onDescriptionChanged()));
	disconnect(&mappings_, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onPortMapChanged()));

	type_.setVLNV(VLNV(), true);
	type_.hide();

	absType_.setVLNV(VLNV(), true);
	absType_.hide();

	nameEdit_.clear();
	nameEdit_.hide();
	nameLabel_.hide();

	modeEdit_.hide();
	modeLabel_.hide();

    dependencyDirCombo_.hide();
    dependencyDirLabel_.hide();

    comDirectionCombo_.hide();
    comDirectionLabel_.hide();

	descriptionLabel_.hide();
	descriptionEdit_.clear();
	descriptionEdit_.hide();

    propertyValueLabel_.hide();
    propertyValueEditor_.hide();
    dummyWidget_.hide();

    transferTypeCombo_.clear();
    transferTypeCombo_.hide();
    transferTypeLabel_.hide();

	mappings_.clearContents();
	mappings_.hide();
	mappingsLabel_.hide();

	parentWidget()->setMaximumHeight(20);
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::onInterfaceModeChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onInterfaceModeChanged( const QString& newMode )
{
	Q_ASSERT(interface_);

	disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

	QSharedPointer<QUndoCommand> cmd(new EndpointChangeCommand(
		static_cast<HWConnectionEndpoint*>(interface_), nameEdit_.text(),
        General::str2Interfacemode(newMode, General::MONITOR),
		descriptionEdit_.toPlainText()));
	static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();

	connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::onInterfaceNameChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onInterfaceNameChanged( const QString& newName )
{
	Q_ASSERT(interface_);
	
	disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));	

	QSharedPointer<QUndoCommand> cmd(new EndpointNameChangeCommand(interface_, newName));
	static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();
	
	connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::onDescriptionChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onDescriptionChanged()
{
	Q_ASSERT(interface_);

	disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

	QSharedPointer<QUndoCommand> cmd(new EndpointDescChangeCommand(interface_, descriptionEdit_.toPlainText()));
	static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();

	connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::onPortMapChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onPortMapChanged()
{
	disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

	// save the bus interface
	QSharedPointer<BusInterface> busIf = interface_->getBusInterface();

	Q_ASSERT_X(busIf, "PortmapInterfaceTab::onSave()", "Null BusInterface pointer");

	QList<QSharedPointer<PortMap> > portMaps;

	for (int i = 0; i < mappings_.rowCount(); i++)
    {
		QString logicalPortName = mappings_.item(i, 0)->text();
		QString physicalPortName = mappings_.item(i, 1)->text();

        QSharedPointer<PortMap::LogicalPort> logicalPort (new PortMap::LogicalPort());
        logicalPort->name_ = logicalPortName;
        QSharedPointer<PortMap::PhysicalPort> physicalPort (new PortMap::PhysicalPort());
        physicalPort->name_ = physicalPortName;

		QSharedPointer<PortMap> portMap(new PortMap);
        portMap->setLogicalPort(logicalPort);
        portMap->setPhysicalPort(physicalPort);

		portMaps.append(portMap);
	}

	QSharedPointer<QUndoCommand> cmd(
        new EndPointPortMapCommand(static_cast<HWConnectionEndpoint*>(interface_), portMaps));
	static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();

	connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::setPortMaps()
//-----------------------------------------------------------------------------
void InterfaceEditor::setPortMaps()
{
	Q_ASSERT(interface_);

	// signal must be disconnected when changing items to avoid loops
	disconnect(&mappings_, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onPortMapChanged()));

	QSharedPointer<BusInterface> busIf = interface_->getBusInterface();
	Q_ASSERT(busIf);
    QList<QSharedPointer<PortMap> > portMaps;
    if (busIf->getPortMaps())
    {
        portMaps = *busIf->getPortMaps();
    }    

	// get the abstraction def for the interface
    VLNV absDefVLNV;
    if (busIf->getAbstractionTypes() && !busIf->getAbstractionTypes()->isEmpty() &&
        busIf->getAbstractionTypes()->first()->getAbstractionRef())
    {
        absDefVLNV = *busIf->getAbstractionTypes()->first()->getAbstractionRef();
    }
    
	QSharedPointer<AbstractionDefinition> absDef;
	if (handler_->getDocumentType(absDefVLNV) == VLNV::ABSTRACTIONDEFINITION)
    {
		QSharedPointer<Document> libComp = handler_->getModel(absDefVLNV);
		absDef = libComp.staticCast<AbstractionDefinition>();
	}

	// get the component that contains the selected interface
	QSharedPointer<Component> component = interface_->getOwnerComponent();
	Q_ASSERT(component);

	// get the interface mode of the bus interface
	General::InterfaceMode interfaceMode = busIf->getInterfaceMode();

	// as many rows as there are interface maps and always 2 columns
	mappings_.setRowCount(portMaps.size());

    QSharedPointer<ComponentParameterFinder> finder (new ComponentParameterFinder(component));
    IPXactSystemVerilogParser parser(finder);

	// stop sorting when adding the ports to avoid sorting after each add
	mappings_.setSortingEnabled(false);
	int row = 0;
	foreach (QSharedPointer<PortMap> portMap, portMaps)
    {
        QString logicalPortName = portMap->getLogicalPort()->name_;

        int logicalSize = 1;
        // if the logical port is vectored
        if (portMap->getLogicalPort() && portMap->getLogicalPort()->range_)
        {
            QString logicalLeft = parser.parseExpression(portMap->getLogicalPort()->range_->getLeft());
            QString logicalRight = parser.parseExpression(portMap->getLogicalPort()->range_->getRight());

            logicalSize = abs(logicalLeft.toInt() - logicalRight.toInt()) + 1;

            logicalPortName += "[" + logicalLeft + ".." + logicalRight + "]";
        }

		QTableWidgetItem* logicalItem = new QTableWidgetItem(logicalPortName);
		logicalItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		// if no abs type is specified
		if (!absDef)
        {
			logicalItem->setForeground(QBrush(Qt::red));
		}
		// if the logical port does not belong to the abs def
        else if (!absDef->hasPort(portMap->getLogicalPort()->name_, interfaceMode))
        {
			logicalItem->setForeground(QBrush(Qt::red));
		}
		else
        {
			logicalItem->setForeground(QBrush(Qt::black));
		}
		
		// display at least the name of physical port
        QString physicalPortName = portMap->getPhysicalPort()->name_;

        int physicalSize = 1;

        int physicalLeft = 0;
        int physicalRight = 0;
        if (portMap->getPhysicalPort())
        {
            QSharedPointer<PortMap::PhysicalPort> physicalPort = portMap->getPhysicalPort();
            if (portMap->getPhysicalPort()->partSelect_)
            {
                physicalLeft = parser.parseExpression(physicalPort->partSelect_->getLeftRange()).toInt();
                physicalRight = parser.parseExpression(physicalPort->partSelect_->getRightRange()).toInt();

                physicalSize = abs(physicalLeft - physicalRight) + 1;
            }
		}
		// if port map does not contain physical vector but port is found on the component
		else if (component->hasPort(physicalPortName))
        {
            QSharedPointer<Port> componentPort = component->getPort(physicalPortName);

            physicalLeft = parser.parseExpression(componentPort->getLeftBound()).toInt();
            physicalRight = parser.parseExpression(componentPort->getRightBound()).toInt();

            physicalSize = abs(physicalLeft - physicalRight) + 1;
		}

        physicalPortName += "[" + QString::number(physicalLeft) + ".." + QString::number(physicalRight) + "]";

		QTableWidgetItem* physItem = new QTableWidgetItem(physicalPortName);
		physItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		// if the port is not contained in the component
        if (!component->hasPort(portMap->getPhysicalPort()->name_))
        {
			physItem->setForeground(QBrush(Qt::red));
		}
		else
        {
			physItem->setForeground(QBrush(Qt::black));
		}

		// if the sizes of the ports don't match
		if (logicalSize != physicalSize)
        {
			logicalItem->setForeground(QBrush(Qt::red));
			physItem->setForeground(QBrush(Qt::red));
		}
		
		mappings_.setItem(row, 0, logicalItem);
		mappings_.setItem(row, 1, physItem);

		row++;
	}

	// set sorting back after all items are added
	mappings_.setSortingEnabled(true);

	connect(&mappings_, SIGNAL(itemChanged(QTableWidgetItem*)),
        this, SLOT(onPortMapChanged()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: interfaceeditor::refresh()
//-----------------------------------------------------------------------------
void InterfaceEditor::refresh()
{
	Q_ASSERT(interface_);
	setInterface(interface_);
}

//-----------------------------------------------------------------------------
// Function: InterfaceEditor::onDependencyDirectionChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onDependencyDirectionChanged(QString const& newDir)
{
    disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

    QSharedPointer<QUndoCommand> cmd(new EndpointDependencyDirectionChangeCommand(
        interface_, str2DependencyDirection(newDir, DEPENDENCY_PROVIDER)));
    static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();

    connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: InterfaceEditor::onComDirectionChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onComDirectionChanged(QString const& newDir)
{
    disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

    QSharedPointer<QUndoCommand> cmd(new EndpointComDirectionChangeCommand(
        interface_, DirectionTypes::str2Direction(newDir, DirectionTypes::IN)));
    static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();

    connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: InterfaceEditor::onComTransferTypeChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onComTransferTypeChanged(QString const& newTransferType)
{
    disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

    QSharedPointer<QUndoCommand> cmd(new EndpointTransferTypeChangeCommand(interface_, newTransferType));
    static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();

    connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: InterfaceEditor::onComPropertyValuesChanged()
//-----------------------------------------------------------------------------
void InterfaceEditor::onComPropertyValuesChanged()
{
    disconnect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()));

    QSharedPointer<QUndoCommand> cmd(new EndpointPropertyValuesChangeCommand(interface_,
                                                                             propertyValueEditor_.getData()));
    static_cast<DesignDiagram*>(interface_->scene())->getEditProvider()->addCommand(cmd);
    cmd->redo();

    connect(interface_, SIGNAL(contentChanged()), this, SLOT(refresh()), Qt::UniqueConnection);
}
