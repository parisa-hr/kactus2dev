//-----------------------------------------------------------------------------
// File: MemoryColumn.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Joni-Matti Maatta
// Date: 17.10.2012
//
// Description:
// Memory column.
//-----------------------------------------------------------------------------

#include "MemoryColumn.h"

#include <common/graphicsItems/GraphicsConnection.h>
#include <common/graphicsItems/GraphicsColumnLayout.h>
#include <common/layouts/VStaticLayout.h>
#include <common/layouts/VStackedLayout.h>
#include <common/layouts/VCollisionLayout.h>
#include <common/layouts/IVGraphicsLayout.h>

#include <editors/MemoryDesigner/AddressSpaceGraphicsItem.h>
#include <editors/MemoryDesigner/MemoryMapGraphicsItem.h>
#include <editors/MemoryDesigner/MainMemoryGraphicsItem.h>
#include <editors/MemoryDesigner/MemoryExtensionGraphicsItem.h>
#include <editors/MemoryDesigner/MemoryConnectionItem.h>
#include <editors/MemoryDesigner/MemoryDesignerConstants.h>

#include <IPXACTmodels/Component/Component.h>

//-----------------------------------------------------------------------------
// Function: MemoryColumn()
//-----------------------------------------------------------------------------
MemoryColumn::MemoryColumn(QSharedPointer<ColumnDesc> desc, GraphicsColumnLayout* layout, int itemSpacing):
GraphicsColumn(desc, layout, false)
{
    QSharedPointer<IVGraphicsLayout<QGraphicsItem> > newItemLayout (
        new VCollisionLayout<QGraphicsItem>(itemSpacing));

    setItemLayout(newItemLayout);
}

//-----------------------------------------------------------------------------
// Function: ~MemoryColumn()
//-----------------------------------------------------------------------------
MemoryColumn::~MemoryColumn()
{

}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::isItemAllowed()
//-----------------------------------------------------------------------------
bool MemoryColumn::isItemAllowed(QGraphicsItem* item, unsigned int allowedItems) const
{
    if (allowedItems & ColumnTypes::MEMORY &&
        (item->type() == AddressSpaceGraphicsItem::Type || item->type() == MemoryMapGraphicsItem::Type))
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::findGraphicsItemByMemoryItem()
//-----------------------------------------------------------------------------
MainMemoryGraphicsItem* MemoryColumn::findGraphicsItemByMemoryItem(
    QSharedPointer<MemoryItem const> containedMemoryItem) const
{
    if (containedMemoryItem)
    {
        foreach (QGraphicsItem* item, getItems())
        {
            MainMemoryGraphicsItem* memoryGraphicsItem = dynamic_cast<MainMemoryGraphicsItem*>(item);
            if (memoryGraphicsItem && memoryGraphicsItem->getMemoryItem() == containedMemoryItem)
            {
                return memoryGraphicsItem;
            }
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::getGraphicsItemInOrder()
//-----------------------------------------------------------------------------
QMap<qreal, MainMemoryGraphicsItem*> MemoryColumn::getGraphicsItemInOrder() const
{
    QMap<qreal, MainMemoryGraphicsItem*> orderedGraphicsItems;

    foreach (QGraphicsItem* graphicsItem, getItems())
    {
        MainMemoryGraphicsItem* memoryItem = dynamic_cast<MainMemoryGraphicsItem*>(graphicsItem);
        if (memoryItem)
        {
            qreal itemScenePositionY = graphicsItem->scenePos().y();
            orderedGraphicsItems.insert(itemScenePositionY, memoryItem);
        }
    }

    return orderedGraphicsItems;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::containsMemoryMapItems()
//-----------------------------------------------------------------------------
bool MemoryColumn::containsMemoryMapItems() const
{
    return name().contains(MemoryDesignerConstants::MEMORYMAPCOLUMNCOMMON_NAME);
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::changeWidth()
//-----------------------------------------------------------------------------
void MemoryColumn::changeWidth(qreal deltaWidth)
{
    bool columnHasItems = getItems().size() > 0;

    int previousXPosition = 0;
    if (columnHasItems)
    {
        previousXPosition = getItems().first()->pos().x();
    }

    int previousWidth = getColumnDesc()->getWidth();
    setWidth(previousWidth + deltaWidth);

    foreach (QGraphicsItem* subItem, getItems())
    {
        MemoryMapGraphicsItem* mapItem = dynamic_cast<MemoryMapGraphicsItem*>(subItem);
        if (mapItem)
        {
            mapItem->setX(previousXPosition);
            mapItem->changeWidth(deltaWidth);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::getMaximumNeededChangeInWidth()
//-----------------------------------------------------------------------------
qreal MemoryColumn::getMaximumNeededChangeInWidth() const
{
    qreal maximumWidthChange = 0;
    foreach (QGraphicsItem* subItem, getItems())
    {
        MemoryMapGraphicsItem* mapItem = dynamic_cast<MemoryMapGraphicsItem*>(subItem);
        if (mapItem)
        {
            maximumWidthChange = qMax(maximumWidthChange, mapItem->getMaximumNeededChangeInFieldWidth());
        }
    }

    return maximumWidthChange;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::compressGraphicsItems()
//-----------------------------------------------------------------------------
void MemoryColumn::compressGraphicsItems(bool condenseMemoryItems, qreal& spaceYPlacement, MemoryColumn* spaceColumn,
    QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedSpaceItems,
    QSharedPointer<QVector<MemoryConnectionItem*> > movedConnectionItems)
{
    qreal yTransfer = 0;
    quint64 spaceItemLowAfter = 0;

    QVector<MainMemoryGraphicsItem*> movedSpaceItems;

    foreach (QGraphicsItem* graphicsItem, getGraphicsItemInOrder())
    {
        MainMemoryGraphicsItem* memoryItem = dynamic_cast<MainMemoryGraphicsItem*>(graphicsItem);
        if (memoryItem)
        {
            MainMemoryGraphicsItem* originalItem = memoryItem;

            memoryItem = getFirstConnectedAddressSpaceItem(memoryItem);

            qreal memoryItemLowBefore = memoryItem->getLowestPointOfConnectedItems();

            memoryItem->condenseItemAndChildItems(movedConnectionItems, condenseMemoryItems);

            extendMemoryItem(originalItem, spaceYPlacement);

            AddressSpaceGraphicsItem* spaceItem = dynamic_cast<AddressSpaceGraphicsItem*>(memoryItem);
            if (spaceItem && placedSpaceItems->contains(memoryItem) && this == spaceColumn &&
                !movedSpaceItems.contains(spaceItem))
            {
                quint64 spaceItemTop = spaceItem->sceneBoundingRect().top() + spaceItem->pen().width();
                if (spaceItemTop != spaceItemLowAfter)
                {
                    qint64 spaceInterval = (spaceItemTop + yTransfer) - spaceItemLowAfter;

                    if (spaceInterval < MemoryDesignerConstants::SPACEITEMINTERVAL)
                    {
                        quint64 yMovementAddition = MemoryDesignerConstants::SPACEITEMINTERVAL -
                            ((spaceItemTop + yTransfer) - spaceItemLowAfter);
                        yTransfer += yMovementAddition;
                    }

                    if (!movedSpaceItems.contains(memoryItem))
                    {
                        spaceItem->moveItemAndConnectedItems(yTransfer);

                        foreach (MainMemoryGraphicsItem* connectedSpace, spaceItem->getAllConnectedSpaceItems())
                        {
                            movedSpaceItems.append(connectedSpace);
                        }

                        movedSpaceItems.append(spaceItem);
                    }
                }

                spaceItemLowAfter = spaceItem->getLowestPointOfConnectedItems();

                yTransfer = spaceItemLowAfter - memoryItemLowBefore;

                spaceYPlacement = spaceItemLowAfter + MemoryDesignerConstants::SPACEITEMINTERVAL;
            }

            memoryItem->resizeSubItemNameLabels();
            memoryItem->createOverlappingSubItemMarkings();
        }
    }
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::getFirstConnectedAddressSpaceItem()
//-----------------------------------------------------------------------------
MainMemoryGraphicsItem* MemoryColumn::getFirstConnectedAddressSpaceItem(MainMemoryGraphicsItem* memoryItem)
{
    MainMemoryGraphicsItem* firstItem = memoryItem;

    AddressSpaceGraphicsItem* spaceItem = dynamic_cast<AddressSpaceGraphicsItem*>(memoryItem);
    if (spaceItem)
    {
        qreal spaceTop = spaceItem->sceneBoundingRect().top();

        foreach (MainMemoryGraphicsItem* topSpaceItem, spaceItem->getAllConnectedSpaceItems())
        {
            qreal connectedTop = topSpaceItem->sceneBoundingRect().top();
            if (connectedTop < spaceTop)
            {
                spaceTop = connectedTop;
                firstItem = topSpaceItem;
            }
        }
    }

    return firstItem;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::extendMemoryItem()
//-----------------------------------------------------------------------------
void MemoryColumn::extendMemoryItem(MainMemoryGraphicsItem* graphicsItem, qreal& spacePlacementY)
{
    MemoryConnectionItem* firstConnection = graphicsItem->getFirstConnection();
    MemoryConnectionItem* lastConnection = graphicsItem->getLastConnection();

    if (firstConnection && lastConnection)
    {
        QPointF itemTopLeft = graphicsItem->boundingRect().topLeft();
        QPointF itemLowRight = graphicsItem->boundingRect().bottomRight();

        int lineWidth = graphicsItem->pen().width();

        qreal connectionTop =
            graphicsItem->mapFromItem(firstConnection, firstConnection->boundingRect().topLeft()).y() + lineWidth;
        qreal connectionLow =
            graphicsItem->mapFromItem(lastConnection, lastConnection->boundingRect().bottomRight()).y();

        bool connectionsAreBeyond = false;

        qreal extensionTop = itemLowRight.y();
        if (connectionTop < itemTopLeft.y())
        {
            extensionTop = connectionTop;
            connectionsAreBeyond = true;
        }

        qreal extensionLow = itemTopLeft.y();
        if (connectionLow > itemLowRight.y())
        {
            extensionLow = connectionLow;
            connectionsAreBeyond = true;
        }

        if (connectionsAreBeyond)
        {
            qreal positionX = itemTopLeft.x() + lineWidth;
            qreal extensionWidth = itemLowRight.x() - itemTopLeft.x() - lineWidth;
            qreal positionY = extensionTop;
            qreal extensionHeight = extensionLow - extensionTop;

            MemoryExtensionGraphicsItem* extensionItem = new MemoryExtensionGraphicsItem(positionX, positionY,
                extensionWidth, extensionHeight, graphicsItem->getContainingInstance(), graphicsItem);
            graphicsItem->setExtensionItem(extensionItem);

            if (extensionLow > itemLowRight.y())
            {
                qreal placementAddition = extensionLow - itemLowRight.y();
                spacePlacementY += placementAddition;
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::moveUnconnectedMemoryItems()
//-----------------------------------------------------------------------------
void MemoryColumn::moveUnconnectedMemoryItems(QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedItems)
{
    qreal currentPosition = getUnconnectedItemPosition(placedItems);

    foreach (QGraphicsItem* graphicsItem, getItems())
    {
        MainMemoryGraphicsItem* memoryItem = dynamic_cast<MainMemoryGraphicsItem*>(graphicsItem);
        if (memoryItem && memoryItem->getMemoryConnections().isEmpty())
        {
            setGraphicsItemPosition(memoryItem, currentPosition);

            currentPosition +=
                memoryItem->getHeightWithSubItems() + MemoryDesignerConstants::UNCONNECTED_ITEM_INTERVAL;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::getLastItemLow()
//-----------------------------------------------------------------------------
quint64 MemoryColumn::getUnconnectedItemPosition(QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedItems)
    const
{
    qreal lastItemLowLinePosition = 0;
    foreach (MainMemoryGraphicsItem* graphicsItem, *placedItems)
    {
        qreal extensionHeight = 0;
        MemoryExtensionGraphicsItem* extensionItem = graphicsItem->getExtensionItem();
        if (extensionItem)
        {
            extensionHeight = extensionItem->sceneBoundingRect().height();
        }
        qreal subItemHeightAddition = graphicsItem->getSubItemHeightAddition();
        if (subItemHeightAddition > extensionHeight)
        {
            extensionHeight = subItemHeightAddition;
        }

        qreal itemLow = graphicsItem->sceneBoundingRect().bottom() + extensionHeight;
        if (itemLow > lastItemLowLinePosition)
        {
            lastItemLowLinePosition = itemLow;
        }
    }

    quint64 positionY = 0;
    if (lastItemLowLinePosition == 0)
    {
        positionY = MemoryDesignerConstants::SPACEITEMINTERVAL;
    }
    else
    {
        positionY = lastItemLowLinePosition + MemoryDesignerConstants::CONNECTED_UNCONNECTED_INTERVAL;
    }

    return positionY;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::setGraphicsItemPosition()
//-----------------------------------------------------------------------------
void MemoryColumn::setGraphicsItemPosition(MainMemoryGraphicsItem* memoryItem, qreal currentPosition)
{
    memoryItem->setY(currentPosition);
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::memoryMapOverlapsInColumn()
//-----------------------------------------------------------------------------
bool MemoryColumn::memoryMapOverlapsInColumn(quint64 connectionBaseAddress, quint64 connectionLastAddress,
    MainMemoryGraphicsItem* memoryGraphicsItem, QRectF memoryItemRect, int memoryPenWidth,
    QVector<MainMemoryGraphicsItem*> connectedSpaceItems,
    QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedMaps) const
{
    foreach (QGraphicsItem* graphicsItem, childItems())
    {
        MemoryMapGraphicsItem* comparisonMemoryItem = dynamic_cast<MemoryMapGraphicsItem*>(graphicsItem);
        if (comparisonMemoryItem && comparisonMemoryItem != memoryGraphicsItem &&
            placedMaps->contains(comparisonMemoryItem))
        {
            QRectF comparisonRectangle = comparisonMemoryItem->getSceneRectangleWithSubItems();
            int comparisonLineWidth = comparisonMemoryItem->pen().width();

            quint64 comparisonBaseAddress = comparisonMemoryItem->getLowestConnectedBaseAddress();
            quint64 comparisonLastAddress = comparisonMemoryItem->getHighestConnectedLastAddress();

            bool itemIsConnectionedToSpaceItems = comparisonMemoryItem->isConnectedToSpaceItems(connectedSpaceItems);
            bool overlap = MemoryDesignerConstants::itemOverlapsAnotherItem(
                memoryItemRect, memoryPenWidth, comparisonRectangle, comparisonLineWidth);

            bool connectionOverlapsItem =
                (connectionBaseAddress >= comparisonBaseAddress && connectionBaseAddress <= comparisonLastAddress)
                ||
                (connectionLastAddress >= comparisonBaseAddress && connectionLastAddress <= comparisonLastAddress);

            if (itemIsConnectionedToSpaceItems && (overlap || connectionOverlapsItem))
            {
                return true;
            }
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::itemOverlapsAnotherItem()
//-----------------------------------------------------------------------------
bool MemoryColumn::itemOverlapsAnotherItem(quint64 itemBaseAddress, quint64 itemLastAddress, QRectF memoryItemRect,
    int memoryPenWidth, MainMemoryGraphicsItem* comparisonMemoryItem) const
{
    QRectF comparisonRectangle = comparisonMemoryItem->getSceneRectangleWithSubItems();

    if (memoryItemRect.x() == comparisonRectangle.x())
    {
        int mapItemLineWidth = comparisonMemoryItem->pen().width();

        qreal comparisonItemEnd = comparisonRectangle.bottom();

        foreach (MemoryConnectionItem* connectionItem, comparisonMemoryItem->getMemoryConnections())
        {
            qreal connectionLow = connectionItem->sceneBoundingRect().bottom();
            if (connectionLow > comparisonItemEnd)
            {
                comparisonItemEnd = connectionLow;
            }
        }

        comparisonRectangle.setBottom(comparisonItemEnd);

        quint64 comparisonBaseAddress = comparisonMemoryItem->getBaseAddress();
        quint64 comparisonLastAddress = comparisonMemoryItem->getLastAddress();
        MemoryConnectionItem* comparisonItemLastConnection = comparisonMemoryItem->getLastConnection();
        if (comparisonItemLastConnection)
        {
            comparisonBaseAddress += comparisonItemLastConnection->getRangeStartValue();
            comparisonLastAddress = comparisonItemLastConnection->getRangeEndValue();
        }
        if (MemoryDesignerConstants::itemOverlapsAnotherItem(
            memoryItemRect, memoryPenWidth, comparisonRectangle, mapItemLineWidth) ||
            (itemBaseAddress >= comparisonBaseAddress && itemBaseAddress <= comparisonLastAddress) ||
            (comparisonBaseAddress >= itemBaseAddress && comparisonBaseAddress <= itemLastAddress))
        {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Function: MemoryColumn::itemOverlapsAnotherColumnItem()
//-----------------------------------------------------------------------------
bool MemoryColumn::itemOverlapsAnotherColumnItem(MainMemoryGraphicsItem* memoryItem, QRectF itemRectangle,
    int lineWidth) const
{
    foreach (MainMemoryGraphicsItem* comparisonItem, getGraphicsItemInOrder())
    {
        if (comparisonItem != memoryItem)
        {
            QRectF comparisonRectangle = comparisonItem->getSceneRectangleWithSubItems();
            int comparisonLineWidth = comparisonItem->pen().width();

            if (MemoryDesignerConstants::itemOverlapsAnotherItem(
                itemRectangle, lineWidth, comparisonRectangle, comparisonLineWidth))
            {
                return true;
            }
        }
    }

    return false;
}
