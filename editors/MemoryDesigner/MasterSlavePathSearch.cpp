//-----------------------------------------------------------------------------
// File: MasterSlavePathSearch.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Esko Pekkarinen
// Date: 25.08.2016
//
// Description:
// Finds the paths from master interfaces to slave interfaces.
//-----------------------------------------------------------------------------

#include "MasterSlavePathSearch.h"

#include <editors/MemoryDesigner/ConnectivityConnection.h>
#include <editors/MemoryDesigner/ConnectivityComponent.h>
#include <editors/MemoryDesigner/ConnectivityGraph.h>
#include <editors/MemoryDesigner/ConnectivityInterface.h>
#include <editors/MemoryDesigner/MemoryItem.h>
#include <editors/MemoryDesigner/MemoryDesignerConstants.h>

#include <QQueue>

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::findMasterSlavePaths()
//-----------------------------------------------------------------------------
QVector<QVector<QSharedPointer<ConnectivityInterface const> > > MasterSlavePathSearch::
    findMasterSlavePaths(QSharedPointer<const ConnectivityGraph> graph, bool allowOverlappingPaths) const
{
    QVector<MasterSlavePathSearch::Path> masterPaths;

    for (auto const& masterInterface : findInitialMasterInterfaces(graph))
    {
        masterPaths.append(findPaths(masterInterface,  graph));
    }

    masterPaths = findValidPathsIn(masterPaths, allowOverlappingPaths);

    for (auto const& edge : graph->getConnections())
    {
        if (edge->getName().contains(QStringLiteral("_to_local_memory_map_")) &&
            edge->getFirstInterface() == edge->getSecondInterface())
        {
            QVector<QSharedPointer<ConnectivityInterface const> > localConnection;
            localConnection.append(edge->getFirstInterface());
            localConnection.append(edge->getSecondInterface());
            masterPaths.append(localConnection);
        }
    }

    return masterPaths;
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::findInitialMasterInterfaces()
//-----------------------------------------------------------------------------
QVector<QSharedPointer<ConnectivityInterface> > MasterSlavePathSearch::findInitialMasterInterfaces(
    QSharedPointer<const ConnectivityGraph> graph) const
{
    QVector<QSharedPointer<ConnectivityInterface> > masterInterfaces;

    for (auto const& vertex : graph->getInterfaces())
    {
        if ((vertex->getMode() == General::MASTER || vertex->getMode() == General::INITIATOR) && vertex->getConnectedMemory())
        {
            masterInterfaces.append(vertex);
        }
    }

    return masterInterfaces;
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::findPaths()
//-----------------------------------------------------------------------------
QVector<MasterSlavePathSearch::Path> MasterSlavePathSearch::findPaths(
    QSharedPointer<ConnectivityInterface> startVertex,
    QSharedPointer<const ConnectivityGraph> graph) const
{
    QVector<MasterSlavePathSearch::Path> foundPaths;

    // Use breadth-first search (BFS) to find shortest paths and avoid loops.
    // The search spans a BFS tree in the graph using the parent-property of the vertices.
    QQueue<QSharedPointer<ConnectivityInterface> > verticesToSearch;
    QVector<QSharedPointer<ConnectivityInterface> > traversed;
    startVertex->setParent(nullptr);
    traversed.append(startVertex);
    verticesToSearch.enqueue(startVertex);

    while (verticesToSearch.isEmpty() == false)
    {
        auto const currentVertex = verticesToSearch.dequeue();
        auto const connections = graph->getConnectionsFor(currentVertex);

        bool isLeaf = true;
        for (auto const& nextEdge : connections)
        {
            auto nextVertex = findConnectedInterface(currentVertex, nextEdge);
            if (traversed.contains(nextVertex) == false && canConnectInterfaces(currentVertex, nextVertex))
            {
                traversed.append(nextVertex);
                nextVertex->setParent(currentVertex);
                verticesToSearch.enqueue(nextVertex);
                isLeaf = false;
            }
        }

        if (isLeaf)
        {
            foundPaths.append(findPathFromRoot(currentVertex));
        }
    }

    return foundPaths;
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::findConnectedInterface()
//-----------------------------------------------------------------------------
QSharedPointer<ConnectivityInterface> MasterSlavePathSearch::findConnectedInterface(
    QSharedPointer<ConnectivityInterface const> startInterface, QSharedPointer<ConnectivityConnection const> edge)
    const
{
    QSharedPointer<ConnectivityInterface> firstInterface = edge->getFirstInterface();
    if (startInterface == firstInterface)
    {
        return edge->getSecondInterface();
    }
    else
    {
        return firstInterface;
    }
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::canConnectInterfaces()
//-----------------------------------------------------------------------------
bool MasterSlavePathSearch::canConnectInterfaces(QSharedPointer<ConnectivityInterface const> startVertex,
    QSharedPointer<ConnectivityInterface const> endVertex) const
{
    if (startVertex == endVertex)
    {
        return false;
    }
    
    if (startVertex->getMode() == endVertex->getMode() && (startVertex->isHierarchical() || endVertex->isHierarchical()))
    {
        return true;
    }

    General::InterfaceMode initiatorMode = General::INITIATOR;
    General::InterfaceMode mirroredInitiatorMode = General::MIRRORED_INITIATOR;
    General::InterfaceMode targetMode = General::TARGET;
    General::InterfaceMode mirroredTargetMode = General::MIRRORED_TARGET;

    General::InterfaceMode startMode = startVertex->getMode();
    bool startModeIsRevision2022 = General::modeIsRevision2022(startMode);
    if (startModeIsRevision2022 == false)
    {
        initiatorMode = General::MASTER;
        mirroredInitiatorMode = General::MIRRORED_MASTER;
        targetMode = General::SLAVE;
        mirroredTargetMode = General::MIRRORED_SLAVE;
    }

    bool startIsInitiator = startMode == initiatorMode;
    bool startIsTarget = startMode == targetMode;
    bool startIsMirroredInitiator = startMode == mirroredInitiatorMode;
    bool startIsMirroredTarget = startMode == mirroredTargetMode;
    bool startIsBridged = startVertex->isBridged();

    General::InterfaceMode endMode = endVertex->getMode();
    bool endModeIsRevision2022 = General::modeIsRevision2022(endMode);
    bool endIsInitiator = endMode == initiatorMode;
    bool endIsTarget = endMode == targetMode;
    bool endIsMirroredInitiator = endMode == mirroredInitiatorMode;
    bool endIsMirroredTarget = endMode == mirroredTargetMode;
    bool endIsBridged = endVertex->isBridged();

    QSharedPointer<ConnectivityComponent const> startInstance = startVertex->getInstance();
    QSharedPointer<ConnectivityComponent const> endInstance = endVertex->getInstance();

    return
        (startModeIsRevision2022 == endModeIsRevision2022) && (
        (startIsInitiator && ((endIsTarget && startInstance != endInstance) || endIsMirroredInitiator)) ||
        (startIsTarget && (endIsMirroredTarget || (endIsInitiator && startIsBridged && endIsBridged && startInstance == endInstance))) ||
        (startIsMirroredInitiator && endIsMirroredTarget && startInstance == endInstance) ||
        (startIsMirroredTarget && (endIsTarget || (endIsMirroredInitiator && startInstance == endInstance)))
        );
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::findPathFromRoot()
//-----------------------------------------------------------------------------
MasterSlavePathSearch::Path MasterSlavePathSearch::findPathFromRoot(
    QSharedPointer<ConnectivityInterface const> endVertex) const
{
    QVector<QSharedPointer<ConnectivityInterface const> > path;
    path.append(endVertex);

    auto i = endVertex;
    while (i->getParent() != nullptr)
    {
        path.append(i->getParent());
        i = i->getParent();
    }

    std::reverse(path.begin(), path.end());
    return path;
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::removeDuplicatePaths()
//-----------------------------------------------------------------------------
QVector<MasterSlavePathSearch::Path> MasterSlavePathSearch::findValidPathsIn(QVector<MasterSlavePathSearch::Path> const& paths,
    bool allowOverlappingPaths) const
{
    QVector<QVector<QSharedPointer<ConnectivityInterface const> > > highPaths;

    for (auto const& currentPath : paths)
    {
        if ((currentPath.size() > 1 && pathEndsInMemoryMap(currentPath)) &&
            (allowOverlappingPaths == true || (allowOverlappingPaths == false && pathIsFullPath(currentPath, paths))))
        {
            highPaths.append(currentPath);
        }
    }

    return highPaths;
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::pathIsFullPath()
//-----------------------------------------------------------------------------
bool MasterSlavePathSearch::pathIsFullPath(MasterSlavePathSearch::Path const& currentPath, 
    QVector<MasterSlavePathSearch::Path> const& availablePaths) const
{
    for (int comparisonIndex = 0; comparisonIndex < availablePaths.size(); ++comparisonIndex)
    {
        QVector<QSharedPointer<ConnectivityInterface const> > comparisonPath =
            availablePaths.at(comparisonIndex);

        if (currentPath != comparisonPath && currentPath.last() == comparisonPath.last())
        {
            auto const pathLength = currentPath.size();
            auto const comparisonPathLength = comparisonPath.size();

            auto overlapLength = qMin(pathLength, comparisonPathLength);

            // Exclude path only if the start interface isn't bridged (e.g. if there is a CPU in middle 
            // of a path) or the path is contained within another path.
            if (pathLength < comparisonPathLength &&
                pathContainsAnotherPath(currentPath, comparisonPath, overlapLength))
            {
                return false;
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::pathContainsAnotherPath()
//-----------------------------------------------------------------------------
bool MasterSlavePathSearch::pathContainsAnotherPath(
    QVector<QSharedPointer<ConnectivityInterface const> > const& pathOne,
    QVector<QSharedPointer<ConnectivityInterface const> > const& pathTwo, int areaSize) const
{
    for (int areaIndex = 0; areaIndex < areaSize; areaIndex++)
    {
        auto const pathOneIndex = pathOne.size() - 1 - areaIndex;
        auto const pathTwoIndex = pathTwo.size() - 1 - areaIndex;
        if (pathOne.at(pathOneIndex) != pathTwo.at(pathTwoIndex))
        {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// Function: MasterSlavePathSearch::pathEndsInMemoryMap()
//-----------------------------------------------------------------------------
bool MasterSlavePathSearch::pathEndsInMemoryMap(QVector<QSharedPointer<ConnectivityInterface const> > const& path)
    const
{
    QSharedPointer<ConnectivityInterface const> lastInterface = path.last();

    return lastInterface && lastInterface->getConnectedMemory() &&
        lastInterface->getConnectedMemory()->getType().compare(MemoryDesignerConstants::MEMORYMAP_TYPE) == 0;
}
