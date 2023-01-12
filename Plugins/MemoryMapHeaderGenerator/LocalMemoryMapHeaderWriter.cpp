//-----------------------------------------------------------------------------
// File: LocalMemoryMapHeaderWriter.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 07.05.2015
//
// Description:
// The implementation for creating c-headers of local memory maps.
//-----------------------------------------------------------------------------

#include "LocalMemoryMapHeaderWriter.h"

#include "localheadersavedelegate.h"

#include <KactusAPI/include/LibraryInterface.h>

#include <KactusAPI/include/IPluginUtility.h>
#include <Plugins/MemoryMapHeaderGenerator/fileSaveDialog/filesavedialog.h>

#include <KactusAPI/include/ComponentParameterFinder.h>

#include <IPXACTmodels/Component/MemoryMap.h>

#include <QDate>
#include <QDir>
#include <QDesktopServices>

//-----------------------------------------------------------------------------
// Function: LocalMemoryMapHeaderWriter::LocalMemoryMapHeaderWriter()
//-----------------------------------------------------------------------------
LocalMemoryMapHeaderWriter::LocalMemoryMapHeaderWriter(IPluginUtility* utility,
    QSharedPointer<Component> localComponent, QObject* parentObject):
BaseMemoryMapHeaderWriter(utility),
component_(localComponent),
parentObject_(parentObject),
saveOptions_()
{

}

//-----------------------------------------------------------------------------
// Function: LocalMemoryMapHeaderWriter::~LocalMemoryMapHeaderWriter()
//-----------------------------------------------------------------------------
LocalMemoryMapHeaderWriter::~LocalMemoryMapHeaderWriter()
{

}

//-----------------------------------------------------------------------------
// Function: LocalMemoryMapHeaderWriter::writeMemoryMapHeader()
//-----------------------------------------------------------------------------
void LocalMemoryMapHeaderWriter::writeMemoryMapHeader(QList<LocalHeaderSaveModel::SaveFileOptions*> saveOptions)
{
    saveOptions_ = saveOptions;
    QList<LocalHeaderSaveModel::SaveFileOptions*> options = saveOptions_;
    bool changed = false;

    LocalHeaderSaveModel model(utility_->getLibraryInterface(), parentObject_);
    model.setComponent(component_);

    if (options.isEmpty())
    {
        FileSaveDialog dialog(utility_->getParentWidget());
        dialog.setModel(&model);
        dialog.setItemDelegate(new LocalHeaderSaveDelegate(component_, parentObject_));

        int result = dialog.exec();

        // if user clicked cancel
        if (result == QDialog::Rejected)
        {
            informGenerationAbort();
            return;
        }

        options = model.getHeaderOptions();
    }

    informStartOfGeneration();

	foreach (LocalHeaderSaveModel::SaveFileOptions* headerOpt, options)
    {
		QFile file(headerOpt->fileInfo_.absoluteFilePath());

        checkDirectoryStructure(headerOpt->fileInfo_.dir());

		if (!file.open(QFile::Truncate | QFile::WriteOnly))
        {
            openFileErrorMessage(headerOpt->fileInfo_.absoluteFilePath());
			break;
		}

        QTextStream stream(&file);

        QString description (" * Header file generated by Kactus2 from local memory map \"" +
            headerOpt->localMemMap_->name() + "\".\n" +
            " * This file contains addresses of the memories and registers defined in the local memory map.\n" +
            " * Source component: " + component_->getVlnv().toString() + ".\n" +
            "*/\n");

        QString headerGuard ("__" + component_->getVlnv().toString("_") + "_" + headerOpt->localMemMap_->name()
            + "_H");
        headerGuard = headerGuard.toUpper();

        writeTopOfHeaderFile(stream, headerOpt->fileInfo_.fileName(), headerGuard, description);

        QSharedPointer<ComponentParameterFinder> finder (new ComponentParameterFinder(component_));

        writeMemoryAddresses(finder, headerOpt->localMemMap_, stream, 0);

		// if the register names are unique then there is no need to add address block name
		/*QStringList regNames;
		if (headerOpt->localMemMap_->uniqueRegisterNames(regNames))
        {*/
        
        writeRegisterFromMemoryMap(finder, headerOpt->localMemMap_, stream, false, 0);
		
        /*}
		else
        {
            writeRegisterFromMemoryMap(finder, headerOpt->localMemMap_, stream, true, 0);
		}*/

		stream << "#endif /* " << headerGuard << " */" << Qt::endl << Qt::endl;

        informWritingFinished(headerOpt->fileInfo_.fileName());

		file.close();

		QStringList swViewRefs;

		// where user selected to add a reference to the generated file set
		QString swViewRef = headerOpt->swView_;
		Q_ASSERT(!swViewRef.isEmpty());

		// if user selected to add the reference to all SW views.
		if (swViewRef == QObject::tr("all"))
        {
			swViewRefs = component_->getViewNames();
		}
		// if user selected only a single view
		else
        {
			swViewRefs.append(swViewRef);
		}

		// add the file to the component's file sets
		addHeaderFile(component_, headerOpt->fileInfo_,
            QString("%1_header").arg(headerOpt->localMemMap_->name()), swViewRefs);

		// a header file was added
		changed = true;
	}

	if (changed)
    {
		// save the changes to the file sets
		utility_->getLibraryInterface()->writeModelToFile(component_);
	}
}

//-----------------------------------------------------------------------------
// Function: LocalMemoryMapHeaderWriter::displayMemoryMapHeader()
//-----------------------------------------------------------------------------
void LocalMemoryMapHeaderWriter::displayMemoryMapHeader(QString const& filePath) const
{
    if (saveOptions_.isEmpty())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }

    informGenerationComplete();
}