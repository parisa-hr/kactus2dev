//-----------------------------------------------------------------------------
// File: FieldGraphicsItem.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 22.11.2016
//
// Description:
// Graphics item for visualizing a field in the memory designer.
//-----------------------------------------------------------------------------

#include "FieldGraphicsItem.h"

#include <common/KactusColors.h>

#include <designEditors/MemoryDesigner/MemoryItem.h>
#include <designEditors/MemoryDesigner/MemoryDesignerChildGraphicsItem.h>
#include <designEditors/MemoryDesigner/MemoryDesignerConstants.h>

#include <QBrush>
#include <QFont>
#include <QFontMetrics>

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::FieldGraphicsItem()
//-----------------------------------------------------------------------------
FieldGraphicsItem::FieldGraphicsItem(QString const& fieldName, quint64 fieldOffset, quint64 fieldLastBit,
    qreal fieldWidth, quint64 fieldHeight, bool isEmptyField, QFont labelFont, QString const& containingInstance,
    MemoryDesignerGraphicsItem* parentItem):
MemoryDesignerChildGraphicsItem(fieldName, QStringLiteral("Field"), fieldOffset, fieldHeight, fieldWidth,
    containingInstance, parentItem),
isEmpty_(isEmptyField),
combinedRangeLabel_(new QGraphicsTextItem("", this)),
fieldHeight_(fieldHeight),
fieldName_(fieldName)
{
    getNameLabel()->setFont(labelFont);
    combinedRangeLabel_->setFont(labelFont);

    setupLabels(fieldOffset, fieldLastBit);
    setupToolTip(QStringLiteral("Field"));
    setLabelPositions();
    setColors(KactusColors::FIELD_COLOR, isEmptyField);
    if (isEmptyField)
    {
        combinedRangeLabel_->setDefaultTextColor(getNameLabel()->defaultTextColor());
    }
}

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::~FieldGraphicsItem()
//-----------------------------------------------------------------------------
FieldGraphicsItem::~FieldGraphicsItem()
{

}

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::setLabelPositions()
//-----------------------------------------------------------------------------
void FieldGraphicsItem::setLabelPositions()
{
    getRangeStartLabel()->hide();
    getRangeEndLabel()->hide();
    
    QGraphicsTextItem* nameLabel = getNameLabel();

    if (combinedRangeLabel_->toPlainText().isEmpty())
    {
        QString rangeValue = QLatin1String("");

        quint64 fieldBaseAddress = getBaseAddress();
        quint64 fieldLastAddress = getLastAddress();
        if (fieldBaseAddress == fieldLastAddress)
        {
            rangeValue = removeZerosFromRangeValue(fieldBaseAddress);
        }
        else
        {
            QString offset = removeZerosFromRangeValue(fieldBaseAddress);
            QString lastBit = removeZerosFromRangeValue(fieldLastAddress);

            rangeValue = lastBit + QLatin1String("..") + offset;
        }

        combinedRangeLabel_->setPlainText(rangeValue);
    }

    qreal nameY = boundingRect().height() / 2 - nameLabel->boundingRect().height() / 2 - 6;
    qreal rangeY = nameY + nameLabel->boundingRect().height() / 2 + 1;
    qreal rangeX = - combinedRangeLabel_->boundingRect().width() / 2;

    nameLabel->setY(nameY);
    combinedRangeLabel_->setPos(rangeX, rangeY);

    fitNameToBoundaries(nameLabel);
}

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::removeZerosFromRangeValue()
//-----------------------------------------------------------------------------
QString FieldGraphicsItem::removeZerosFromRangeValue(quint64 rangeValue) const
{
    QString formattedValue = getValueFormattedToHexadecimal(rangeValue);
    if (formattedValue.size() > 0)
    {
        while (formattedValue.size() > 1 && formattedValue.at(0) == '0')
        {
            formattedValue = formattedValue.right(formattedValue.size() - 1);
        }
    }

    return formattedValue;
}

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::fitNameLabel()
//-----------------------------------------------------------------------------
void FieldGraphicsItem::fitNameToBoundaries(QGraphicsTextItem* nameLabel)
{
    const int NAME_MARGIN = 14;

    QFontMetrics nameFontMetrics(nameLabel->font());
    unsigned int itemBoundingWidth = boundingRect().width() - NAME_MARGIN;

    QString elidedName = nameFontMetrics.elidedText(fieldName_, Qt::ElideRight, itemBoundingWidth);
    nameLabel->setPlainText(elidedName);
    nameLabel->setX(-nameLabel->boundingRect().width() / 2);
}

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::condense()
//-----------------------------------------------------------------------------
void FieldGraphicsItem::condense(qreal newItemHeight)
{
    MemoryDesignerGraphicsItem::condense(newItemHeight);
    setLabelPositions();
}

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::changeWidth()
//-----------------------------------------------------------------------------
void FieldGraphicsItem::changeWidth(qreal widthChange)
{
    qreal newWidth = boundingRect().width() + widthChange - 1;

    setRect(-newWidth / 2, 0, newWidth, boundingRect().height() - 1);

    QGraphicsTextItem* nameLabel = getNameLabel();

    if (widthChange > 0)
    {
        nameLabel->setPlainText(fieldName_);
    }

    qreal nameX = - nameLabel->boundingRect().width() / 2;
    nameLabel->setX(nameX);

    fitNameToBoundaries(nameLabel);
}

//-----------------------------------------------------------------------------
// Function: FieldGraphicsItem::getMaximumNeededWidthChange()
//-----------------------------------------------------------------------------
qreal FieldGraphicsItem::getNeededWithChangeToDisplayFullName() const
{
    qreal neededWithChangeToDisplayFullName = 0;

    QString currentName = getNameLabel()->toPlainText();
    if (currentName.compare(fieldName_, Qt::CaseInsensitive) != 0)
    {
        qreal currentWidth = boundingRect().width();

        getNameLabel()->setPlainText(fieldName_);
        qreal nameLabelWidth = getNameLabel()->boundingRect().width();

        neededWithChangeToDisplayFullName = nameLabelWidth - currentWidth + 12;

        quint64 fieldWidth = getLastAddress() - getBaseAddress() + 1;
        neededWithChangeToDisplayFullName = neededWithChangeToDisplayFullName / fieldWidth;
    }

    return neededWithChangeToDisplayFullName;
}