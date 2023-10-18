//-----------------------------------------------------------------------------
// File: MirroredSlaveModeEditor.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Antti Kamppi
// Date: 7.4.2011
//
// Description:
// Editor to edit mirrored slave details of a bus interface.
//-----------------------------------------------------------------------------

#include "MirroredSlaveModeEditor.h"

#include <IPXACTmodels/generaldeclarations.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/BusInterface.h>
#include <IPXACTmodels/Component/MirroredTargetInterface.h>

#include <editors/ComponentEditor/common/ExpressionEditor.h>
#include <KactusAPI/include/ExpressionFormatter.h>
#include <KactusAPI/include/ExpressionParser.h>
#include <editors/ComponentEditor/parameters/ComponentParameterModel.h>
#include <editors/ComponentEditor/memoryMaps/memoryMapsExpressionCalculators/ReferenceCalculator.h>
#include <KactusAPI/include/BusInterfaceInterface.h>

#include <QCompleter>
#include <QGridLayout>
#include <QLabel>

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::MirroredSlaveModeEditor()
//-----------------------------------------------------------------------------
MirroredSlaveModeEditor::MirroredSlaveModeEditor(BusInterfaceInterface* busInterface, std::string const& busName,
    QSharedPointer<ParameterFinder> parameterFinder, QSharedPointer<ExpressionParser> expressionParser,
    QWidget *parent):
ModeEditorBase(busInterface, busName, tr("Mirrored slave"), parent),
remapEditor_(new ExpressionEditor(parameterFinder, this)),
rangeEditor_(new ExpressionEditor(parameterFinder, this)),
expressionParser_(expressionParser),
parameterFinder_(parameterFinder)
{
    QString functionSymbol(QChar(0x0192));
	QLabel* remapLabel = new QLabel(tr("Remap address") + ", " + functionSymbol + "(x):", this);
	QLabel* rangeLabel = new QLabel(tr("Range") + ", " + functionSymbol + "(x):", this);

    remapEditor_->setFixedHeight(ExpressionEditor::DEFAULT_HEIGHT);
    rangeEditor_->setFixedHeight(ExpressionEditor::DEFAULT_HEIGHT);

    ComponentParameterModel* componentParameterModel = new ComponentParameterModel(parameterFinder, this);
    componentParameterModel->setExpressionParser(expressionParser);

    auto remapCompleter = new QCompleter(this);
    remapCompleter->setModel(componentParameterModel);
    remapEditor_->setAppendingCompleter(remapCompleter);

    auto rangeCompleter = new QCompleter(this);
    rangeCompleter->setModel(componentParameterModel);
    rangeEditor_->setAppendingCompleter(rangeCompleter);

	QGridLayout* topLayout = new QGridLayout(this);
	topLayout->addWidget(remapLabel, 0, 0, 1, 1);
    topLayout->addWidget(remapEditor_, 0, 1, 1, 1);
	topLayout->addWidget(rangeLabel, 1, 0, 1, 1);
    topLayout->addWidget(rangeEditor_, 1, 1, 1, 1);

	topLayout->setColumnStretch(1, 1);
	topLayout->setRowStretch(2, 1);

    connect(remapEditor_, SIGNAL(editingFinished()), this, SLOT(onRemapChange()), Qt::UniqueConnection);
    connect(rangeEditor_, SIGNAL(editingFinished()), this, SLOT(onRangeChange()), Qt::UniqueConnection);

    connect(remapEditor_, SIGNAL(increaseReference(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(remapEditor_, SIGNAL(decreaseReference(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);
    connect(rangeEditor_, SIGNAL(increaseReference(QString)),
        this, SIGNAL(increaseReferences(QString)), Qt::UniqueConnection);
    connect(rangeEditor_, SIGNAL(decreaseReference(QString)),
        this, SIGNAL(decreaseReferences(QString)), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::isValid()
//-----------------------------------------------------------------------------
bool MirroredSlaveModeEditor::isValid() const
{
	return true;
}

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::refresh()
//-----------------------------------------------------------------------------
void MirroredSlaveModeEditor::refresh()
{
    rangeEditor_->blockSignals(true);

    BusInterfaceInterface* busInterface = getBusInterface();
    std::string busName = getBusName();

    rangeEditor_->setExpression(QString::fromStdString(busInterface->getRangeExpression(busName)));
    rangeEditor_->setToolTip(QString::fromStdString(busInterface->getRangeValue(busName)));

    rangeEditor_->blockSignals(false);

    remapEditor_->blockSignals(true);

    remapEditor_->setExpression(QString::fromStdString(busInterface->getRemapAddressExpression(busName)));
    remapEditor_->setToolTip(QString::fromStdString(busInterface->getRemapAddressValue(busName)));

    remapEditor_->blockSignals(false);
}

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::getInterfaceMode()
//-----------------------------------------------------------------------------
General::InterfaceMode MirroredSlaveModeEditor::getInterfaceMode() const
{
	return General::MIRRORED_SLAVE;
}

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::saveModeSpecific()
//-----------------------------------------------------------------------------
void MirroredSlaveModeEditor::saveModeSpecific()
{
    BusInterfaceInterface* busInterface = getBusInterface();
    std::string busName = getBusName();

    busInterface->setRange(busName, rangeEditor_->getExpression().toStdString());
    busInterface->setRemapAddress(busName, remapEditor_->getExpression().toStdString());
}

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::onRemapChange()
//-----------------------------------------------------------------------------
void MirroredSlaveModeEditor::onRemapChange()
{
    remapEditor_->finishEditingCurrentWord();

    QString newRemapAddress = remapEditor_->getExpression();

    BusInterfaceInterface* busInterface = getBusInterface();
    std::string busName = getBusName();

    busInterface->setRemapAddress(busName, newRemapAddress.toStdString());

    remapEditor_->setToolTip(QString::fromStdString(busInterface->getRemapAddressValue(busName)));

    emit contentChanged();
}

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::onRangeChange()
//-----------------------------------------------------------------------------
void MirroredSlaveModeEditor::onRangeChange()
{
    rangeEditor_->finishEditingCurrentWord();

    BusInterfaceInterface* busInterface = getBusInterface();
    std::string busName = getBusName();

    busInterface->setRange(busName, rangeEditor_->getExpression().toStdString());
    rangeEditor_->setToolTip(QString::fromStdString(busInterface->getRangeValue(busName)));

    emit contentChanged();
}

//-----------------------------------------------------------------------------
// Function: MirroredSlaveModeEditor::removeReferencesFromExpressions()
//-----------------------------------------------------------------------------
void MirroredSlaveModeEditor::removeReferencesFromExpressions()
{
    QStringList expressionList;
    expressionList.append(rangeEditor_->getExpression());
    expressionList.append(remapEditor_->getExpression());

    ReferenceCalculator referenceCalculator(parameterFinder_);
    QMap<QString, int> referencedParameters = referenceCalculator.getReferencedParameters(expressionList);

    for (QString const& referencedId : referencedParameters.keys())
    {
        for (int i = 0; i < referencedParameters.value(referencedId); ++i)
        {
            emit decreaseReferences(referencedId);
        }
    }

    remapEditor_->clear();

    BusInterfaceInterface* busInterface = getBusInterface();
    std::string busName = getBusName();

    busInterface->setRemapAddress(busName, "");

    rangeEditor_->clear();
    busInterface->setRange(busName, "");
}
