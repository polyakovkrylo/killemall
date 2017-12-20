/*!
 * \file astarcontroller.h
 *
 * AStarController class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */
#ifndef ASTARCONTROLLER_H
#define ASTARCONTROLLER_H

#include <omp.h>
#include <queue>

#include "worldabstractcontroller.h"
#include "world.h"

/*!
 * \ingroup controller
 * \brief The Node struct
 *
 * Contains information about tiles visit flag,
 * energy costs, postion, pointers to its neigbours
 * and predicessors
 */
struct Node
{
    bool visited;
    float nodeCost;
    float g;        // path cost
    float h;        // min destination cost
    float f;        // path rate
    int x;
    int y;
    std::array<Node*,4> neighbours;
    Node* prev;
};

/*!
 * \ingroup controller
 * \brief Node comparison functor
 *
 * Defines a more preferable node among two to
 * prioritize in a queue
 */
class CompareCost{
public:
    inline bool operator()(Node* &t1, Node* &t2) const{
        return t1->f > t2->f;
    }
};

/*!
 * \ingroup controller
 * \brief A* controller
 *
 * This is the controller based on A* path finding
 * algorithm
 */
class AStarController : public WorldAbstractController
{
    using NodeQueue = std::priority_queue<Node *, std::vector<Node *>, CompareCost>;
public:
    /*!
     * \brief Class constructor
     * \param model owner of the controller
     */
    AStarController(WorldModel* model);
    /*!
     * \brief Find the path to the position
     *
     * Find a path to the position using A* algorithm
     * \param from starting point
     * \param to destination
     * \return true if the path was find, false otherwise
     */
    virtual bool findPath(const QPoint &from, const QPoint &to, float maxCost = INFINITY) override;
    /*!
     * \brief Initialization function
     *
     * Creates a node vector matrix from world's tiles
     */
    virtual void init() override;
    /*!
     * \brief Marks each node in node vector matrix as not visited
     */
    void clearNodes();
private:
    std::vector<std::vector<std::unique_ptr<Node>>> nodes_;
    void addNeighbours(NodeQueue &openNodes, Node *node, const QPoint &destination);
};

#endif // ASTARCONTROLLER_H
