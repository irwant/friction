﻿#ifndef SMARTPATHCONTAINER_H
#define SMARTPATHCONTAINER_H
#include "simplemath.h"
#include "skia/skiaincludes.h"
#include "exceptions.h"
#include "pointhelpers.h"

#include "framerange.h"
#include "smartPointers/stdselfref.h"
#include "smartPointers/stdpointer.h"

class SmartPath : public StdSelfRef {
    friend class StdSelfRef;
    enum Neighbour { NONE, NEXT, PREV, BOTH = NEXT | PREV };
public:
    void actionRemoveNormalNode(const int& nodeId);

    void actionAddFirstNode(const QPointF& c0,
                            const QPointF& p1,
                            const QPointF& c2);

    void actionInsertNodeBetween(const int &prevId,
                                 const int& nextId,
                                 const QPointF &c0,
                                 const QPointF &p1,
                                 const QPointF &c2);
    void actionInsertNodeBetween(const int &prevId,
                                 const int& nextId,
                                 const qreal& t);

    void actionPromoteDissolvedNodeToNormal(const int& nodeId);

    void actionDisconnectNodes(const int& node1Id, const int& node2Id);

    void actionConnectNodes(const int& node1Id, const int& node2Id);

    int dissolvedOrDummyNodeInsertedToNeigh(const int &targetNodeId,
                                            const Neighbour &neigh);
    void normalOrMoveNodeInsertedToNeigh(const int &targetNodeId,
                                         const Neighbour& neigh);

    void removeNodeWithIdAndTellPrevToDoSame(const int& nodeId);

    void removeNodeWithIdAndTellNextToDoSame(const int& nodeId);

    const Node::Type& nodeType(const int& nodeId) const;

    bool isNodeNormal(const int& nodeId) const;

    bool isNodeDissolved(const int& nodeId) const;

    bool isNodeDummy(const int& nodeId) const;

    QList<int> updateAllNodesTypeAfterNeighbourChanged();

    //! @brief Returns true if changed.
    bool updateNodeTypeAfterNeighbourChanged(const int& nodeId);

    void setPrev(SmartPath * const prev);

    void setNext(SmartPath * const next);

    const QList<Node>& getNodes() const;

    SkPath getPathAt() const;
    SkPath getPathForPrev() const;
    SkPath getPathForNext() const;

    SkPath interpolateWithNext(const qreal& nextWeight) const;
    SkPath interpolateWithPrev(const qreal& nextWeight) const;
protected:
    SmartPath();
    SmartPath(const QList<Node>& nodes);
private:
    void setNodeType(const int& nodeId, const Node::Type& type) {
        if(nodeId < 0 || nodeId >= mNodes.count()) return;
        setNodeType(nodeId, mNodes[nodeId], type);
    }

    void setNodeType(const int& nodeId, Node& node,
                     const Node::Type& type) {
        node.setType(type);
        updateAfterNodeChanged(nodeId);
    }

    void setNodeNextId(const int& nodeId, const int& nextId) {
        if(nodeId < 0 || nodeId >= mNodes.count()) return;
        setNodeNextId(nodeId, mNodes[nodeId], nextId);
    }

    void setNodeNextId(const int& nodeId, Node& node,
                       const int& nextId) {
        node.setNextNodeId(nextId);
        updateAfterNodeChanged(nodeId);
    }

    void setNodePrevId(const int& nodeId, const int& prevId) {
        if(nodeId < 0 || nodeId >= mNodes.count()) return;
        setNodePrevId(nodeId, mNodes[nodeId], prevId);
    }

    void setNodePrevId(const int& nodeId, Node& node,
                       const int& prevId) {
        node.setPrevNodeId(prevId);
        updateAfterNodeChanged(nodeId);
    }

    void setNodePrevAndNextId(const int& nodeId,
                              const int& prevId, const int& nextId) {
        if(nodeId < 0 || nodeId >= mNodes.count()) return;
        setNodePrevAndNextId(nodeId, mNodes[nodeId], prevId, nextId);
    }

    void setNodePrevAndNextId(const int& nodeId, Node& node,
                              const int& prevId, const int& nextId) {
        node.setPrevNodeId(prevId);
        node.setNextNodeId(nextId);
        updateAfterNodeChanged(nodeId);
    }

    void updateAfterNodeChanged(const int& nodeId) {
        updateNodeTypeAfterNeighbourChanged(nodeId);
        if(mPrev) mPrev->updateNodeTypeAfterNeighbourChanged(nodeId);
        if(mNext) mNext->updateNodeTypeAfterNeighbourChanged(nodeId);
    }

    SkPath getPathFor(SmartPath * const neighbour) const;
    void insertNodeBetween(const int &prevId, const int &nextId,
                           const Node &nodeBlueprint);
    void moveNodeBefore(const int &moveNodeId, Node &moveNode,
                        const int &beforeNodeId, Node &beforeNode);
    void moveNodeAfter(const int &moveNodeId, Node &moveNode,
                       const int &afterNodeId, Node &afterNode);

    stdptr<SmartPath> mPrev;
    stdptr<SmartPath> mNext;
    QList<Node> mNodes;
};

#endif // SMARTPATHCONTAINER_H
