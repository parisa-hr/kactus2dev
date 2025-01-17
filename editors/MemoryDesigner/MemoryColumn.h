//-----------------------------------------------------------------------------
// File: MemoryColumn.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Joni-Matti Maatta
// Date: 17.10.2012
//
// Description:
// Memory column.
//-----------------------------------------------------------------------------

#ifndef MEMORYCOLUMN_H
#define MEMORYCOLUMN_H

#include <common/graphicsItems/GraphicsItemTypes.h>
#include <common/graphicsItems/GraphicsColumn.h>

class GraphicsColumnLayout;
class GraphicsConnection;
class MainMemoryGraphicsItem;
class MemoryItem;
class MemoryConnectionItem;
class MemoryMapGraphicsItem;

#include <QGraphicsRectItem>
#include <QSet>
#include <QCursor>

//-----------------------------------------------------------------------------
//! MemoryColumn class.
//-----------------------------------------------------------------------------
class MemoryColumn : public GraphicsColumn
{
    Q_OBJECT

public:
    enum { Type = GFX_TYPE_SYSTEM_COLUMN };

    /*!
     *  Constructor.
     *
     *      @param [in] name            The column description.
     *      @param [in] layout          The parent column layout.
     *      @param [in] itemSpacing     Spacing for the graphics items.
     */
    MemoryColumn(QSharedPointer<ColumnDesc> desc, GraphicsColumnLayout* layout, int itemSpacing = 30);

    /*!
     *  The destructor.
     */
    virtual ~MemoryColumn();

    /*!
     *  Find a graphics item by a given memory item.
     *
     *      @param [in] containedMemoryItem     The selected memory item.
     *
     *      @return Memory graphics item matching the selected memory item.
     */
    MainMemoryGraphicsItem* findGraphicsItemByMemoryItem(QSharedPointer<MemoryItem const> containedMemoryItem)
        const;

    /*!
     *  Get the type of this item.
     */
    int type() const { return Type; }

    /*!
     *  Get the main memory graphics items contained within this column in the order of their scene positions.
     *
     *      @return Map containing y-coordinate, main memory graphics item pairs.
     */
    QMap<qreal, MainMemoryGraphicsItem*> getGraphicsItemInOrder() const;

    /*!
     *  Check if the memory column contains memory map graphics items.
     *
     *      @return True, if the column contains memory map graphics items, false otherwise.
     */
    bool containsMemoryMapItems() const;

    /*!
     *  Change the width of the memory column and the contained memory map graphics items.
     *
     *      @param [in] deltaWidth  Change of width.
     */
    void changeWidth(qreal deltaWidth);

    /*!
     *  Get the maximum needed change in width to display the name labels of all the field items contained within
     *  the memory map items.
     *
     *      @return The maximum needed change in width to display the name labels of all the field items.
     */
    qreal getMaximumNeededChangeInWidth() const;

    /*!
     *  Compress graphics items contained within.
     *
     *      @param [in] condenseMemoryItems     Value for compressing graphics items.
     *      @param [in] spaceYPlacement         Y-coordinate of the address space graphics items.
     *      @param [in] spaceColumn             Column containing address space graphics items.
     *      @param [in] placedSpaceItems        List of the placed address space graphics items.
     *      @param [in] movedConnectionItems    List of already moved connections.
     */
    void compressGraphicsItems(bool condenseMemoryItems, qreal& spaceYPlacement, MemoryColumn* spaceColumn,
        QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedSpaceItems,
        QSharedPointer<QVector<MemoryConnectionItem*> > movedConnectionItems);

    /*!
     *  Move the unconnected memory items to the bottom of the column.
     *
     *      @param [in] placedItems     List of the connected items of the column.
     */
    void moveUnconnectedMemoryItems(QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedItems);

    /*!
     *  Move a memory graphics item.
     *
     *      @param [in] memoryItem          The selected memory graphics item.
     *      @param [in] currentPosition     The new y-coordinate for the graphics item.
     */
    void setGraphicsItemPosition(MainMemoryGraphicsItem* memoryItem, qreal currentPosition);

    /*!
     *  Check if a memory map overlaps within this column.
     *
     *      @param [in] connectionBaseAddress   Lowest base address of the connections in the selected memory item.
     *      @param [in] connectionLastAddress   Highest last address of the connections in the selected memory item.
     *      @param [in] memoryGraphicsItem      The selected memory map item.
     *      @param [in] memoryItemRect          Bounding rectangle of the selected memory map item.
     *      @param [in] memoryPenWidth          Line width of the memory map item.
     *      @param [in] connectedSpaceItems     List of address space items connected to the memory map item.
     *      @param [in] placedMaps              List of the placed memory map items.
     *
     *      @return True if the memory map overlaps another item in this column, false otherwise.
     */
    bool memoryMapOverlapsInColumn(quint64 connectionBaseAddress, quint64 connectionLastAddress,
        MainMemoryGraphicsItem* memoryGraphicsItem, QRectF memoryItemRect, int memoryPenWidth,
        QVector<MainMemoryGraphicsItem*> connectedSpaceItems,
        QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedMaps) const;

    /*!
     *  Check if a main memory graphics item overlaps another item in this column.
     *
     *      @param [in] itemBaseAddress         Base address of the selected memory item.
     *      @param [in] itemLastAddress         Last address of the selected memory item.
     *      @param [in] memoryItemRect          Bounding rectangle of the selected memory item.
     *      @param [in] memoryPenWidth          Line width of the selected memory item.
     *      @param [in] comparisonMemoryItem    Memory item being compared to.
     *
     *      @return True, if the memory map items overlap, false otherwise.
     */
    bool itemOverlapsAnotherItem(quint64 itemBaseAddress, quint64 itemLastAddress, QRectF memoryItemRect,
        int memoryPenWidth, MainMemoryGraphicsItem* comparisonMemoryItem) const;

    /*!
     *  Check if the selected item overlaps another main graphics item in this column.
     *
     *      @param [in] memoryItem      The selected memory graphics item.
     *      @param [in] itemRectangle   Rectangle of the selected item.
     *      @param [in] lineWidth       Line width of the selected item.
     *
     *      @return True if the selected item overlaps another main graphics item, false otherwise.
     */
    bool itemOverlapsAnotherColumnItem(MainMemoryGraphicsItem* memoryItem, QRectF itemRectangle, int lineWidth)
        const;

protected:

    /*!
     *  Returns true if the given item is allowed to reside in the column based on the allowed items.
     *
     *      @param [in] item          The item.
     *      @param [in] allowedItems  The allowed items flags.
     */
    virtual bool isItemAllowed(QGraphicsItem* item, unsigned int allowedItems) const;

private:
    // Disable copying.
    MemoryColumn(MemoryColumn const& rhs);
    MemoryColumn& operator=(MemoryColumn const& rhs);

    /*!
     *  Construct a memory graphics extension for the selected memory item.
     *
     *      @param [in] graphicsItem        The selected memory graphics item.
     *      @param [in] spaceYPlacement     Y-coordinate of the graphics item.
     */
    void extendMemoryItem(MainMemoryGraphicsItem* graphicsItem, qreal& spacePlacementY);

    /*!
     *  Get the position for the unconnected memory items.
     *
     *      @param [in] placedItems     List of the placed memory graphics items.
     *
     *      @return Y-coordinate for the unconnected memory items.
     */
    quint64 getUnconnectedItemPosition(QSharedPointer<QVector<MainMemoryGraphicsItem*> > placedItems) const;

    /*!
     *  Get the first address space graphics item connected the memory item.
     *
     *      @param [in] memoryItem  The selected memory item.
     *
     *      @return The first connected address space graphics item.
     */
    MainMemoryGraphicsItem* getFirstConnectedAddressSpaceItem(MainMemoryGraphicsItem* memoryItem);
};

//-----------------------------------------------------------------------------

#endif // MEMORYCOLUMN_H
