#include "astarcontroller.h"
#include "model/worldmodel.h"

using std::vector;
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
        // working node
        Node* node = nodes_[from].get();
        openNodes.push(node);

        while(!openNodes.empty() && !pathFound)
        {
            // pick the node with lowest pathCost from openNodes
            node = openNodes.top();
            openNodes.pop();
            // break if the target is reached
            if(node->x==to.x() && node->y==to.y()){
                pathFound=true;
                break;
            }
            // update non-black neighbours and add them to open nodes
            addNeighbours(openNodes,node);
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
                {nullptr},      //neighbours
                nullptr         //prev
        };
        nodes_[QPoint(t->getXPos(),t->getYPos())] = make_shared<Node>(n);
    }

    for(auto &t: tiles) {
        int x = t->getXPos();
        int y = t->getYPos();
        nodes_.value(QPoint(x,y))->neighbours[0] = nodes_.value(QPoint(x+1,y)).get();
        nodes_.value(QPoint(x,y))->neighbours[1] = nodes_.value(QPoint(x-1,y)).get();
        nodes_.value(QPoint(x,y))->neighbours[2] = nodes_.value(QPoint(x,y+1)).get();
        nodes_.value(QPoint(x,y))->neighbours[3] = nodes_.value(QPoint(x,y-1)).get();
    }
}

void AStarController::clearNodes()
{
    // mark all nodes as non-visited
    for(auto &n: nodes_) {
        n->visited = false;
    }
}

void AStarController::addNeighbours(NodeQueue &openNodes, Node *node)
{
    for(int i = 0; i < 4; i++) {
        Node* nebr = node->neighbours[i];
        // if a neighbour exists
        if(nebr) {
            // update node if it is non-black and not visited
            if(nebr->nodeCost && !nebr->visited){
                nebr->pathCost = nebr->nodeCost + node->pathCost;
                nebr->visited = true;
                nebr->prev = node;
                // add node to open nodes
                openNodes.push(nebr);
            }
        }
    }
}
