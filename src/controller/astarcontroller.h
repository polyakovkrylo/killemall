#ifndef ASTARCONTROLLER_H
#define ASTARCONTROLLER_H

#include <omp.h>
#include <queue>
#include <QHash>

#include "worldabstractcontroller.h"
#include "world.h"

struct Node
{
    bool visited;
    float nodeCost;
    float pathCost;
    int x;
    int y;
    Node* prev;
};

class CompareCost{
public:
    inline bool operator()(Node* &t1, Node* &t2) const{
        return t1->pathCost > t2->pathCost;
    }
};

inline uint qHash (const QPoint & key)
{
    return qHash (static_cast <qint64> (key.x () ) << 32 | key.y () );
}

class AStarController : public WorldAbstractController
{
    using NodeQueue = std::priority_queue<Node *, std::vector<Node *>, CompareCost>;
public:
    AStarController(WorldModel* model);
    virtual bool findPath(const QPoint &from, const QPoint &to) override;
    virtual void init() override;
    void clearNodes();
private:
    QHash<const QPoint,std::shared_ptr<Node>> nodes_;
    void findNeighbours(Node *node,
                        std::vector<Node *> &neighbours);
    void addNeighbours(NodeQueue &openNodes,
                       Node *node,
                       std::vector<Node *> &neighbours);
};

#endif // ASTARCONTROLLER_H
