#include "astarcontroller.h"
#include "model/worldmodel.h"

using std::vector;
using std::unique_ptr;
using std::priority_queue;
using std::shared_ptr;
using std::make_shared;

AStarController::AStarController(WorldModel *model) :
    WorldAbstractController(model)
{

}

bool AStarController::findPath(const QPoint &from, const QPoint &to)
{
    // stop animation, clear path steps and nodes from last pathfining
    animation_.stop();
    path_.steps.clear();
    clearNodes();

    int targetValue=model_->getLevel().pixelColor(to).lightness();

    bool pathFound = false;

    // if point 'to' is not black
    if(targetValue>0)
    {
        // declare queue of open nodes and neighbours vector
        NodeQueue openNodes;
        openNodes.push(nodes_[from]);
        vector<shared_ptr<Node>> nebrs;
        nebrs.reserve(4);
        // working node
        Node* node = nullptr;

        while(!openNodes.empty() && !pathFound)
        {
            // pick the node with lowest pathCost from openNodes
            node = openNodes.top().get();
            openNodes.pop();
            // break if the target is reached
            if(node->x==to.x() && node->y==to.y()){
                pathFound=true;
                break;
            }
            // find neighbours of the node and store them in nebrs
            findNeighbours(node,nebrs);
            // update non-black neighbours and add them to open nodes
            addNeighbours(openNodes,node,nebrs);
            // clear nebrs for next iteration
            nebrs.clear();
        }

        if(pathFound) {
            path_.cost = node->pathCost;
            // re-create path from the last node
            while(node->x!=from.x() || node->y!=from.y()){
                path_.steps.push_front(QPoint(node->x,node->y));
                node = node->prev;
            }
        }
    }

    return pathFound;
}

void AStarController::init()
{
    auto &tiles = model_->getWorld()->getMap();
    // clear from the previous map nodes
    nodes_.clear();
    nodes_.reserve(tiles.size());
    for(auto &t: tiles) {
        // create node for each map tile
        Node n = {
                false,  //visited
                1.0f/t->getValue(),  //nodeCost
                0.0f,           //pathCost
                t->getXPos(),   //x
                t->getYPos(),   //y
                nullptr         //prev
        };
        nodes_[QPoint(t->getXPos(),t->getYPos())] = make_shared<Node>(n);
    }
}

void AStarController::clearNodes()
{
    // mark all nodes as non-visited
    for(auto &n: nodes_) {
        n->visited = false;
    }
}

void AStarController::findNeighbours(Node *node, std::vector<std::shared_ptr<Node> > &neighbours)
{
    // add neigbour if one exists
    auto &right = nodes_.value(QPoint(node->x+1,node->y));
    if(right)
        neighbours.push_back(right);

    auto &left = nodes_.value(QPoint(node->x-1,node->y));
    if(left)
        neighbours.push_back(left);

    auto &bottom = nodes_.value(QPoint(node->x,node->y+1));
    if(bottom)
        neighbours.push_back(bottom);

    auto &top = nodes_.value(QPoint(node->x,node->y-1));
    if(top)
        neighbours.push_back(top);
}

void AStarController::addNeighbours(NodeQueue &openNodes,
                                    Node *node, vector<shared_ptr<Node>> &neighbours)
{
    for(auto &n: neighbours) {
        // update node if it is non-black and not visited
        if(n->nodeCost && !n->visited){
            n->pathCost = n->nodeCost + node->pathCost;
            n->visited = true;
            n->prev = node;
            // add node to open nodes
            openNodes.push(n);
        }
    }
}
