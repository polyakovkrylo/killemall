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
        Node* node = nodes_.at(from.x()).at(from.y()).get();
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
    // clear vector from previous map nodes
    nodes_.clear();
    nodes_.reserve(model_->getLevel().width());

    // iterate through columns
    for(int i = 0; i < model_->getLevel().width(); ++i) {
        // add column vector
        vector<shared_ptr<Node>> vec;
        vec.reserve(model_->getLevel().height());
        nodes_.push_back(std::move(vec));
        for(int j = 0; j < model_->getLevel().height(); ++j) {
            // create node for each map tile
            nodes_.back().push_back(make_shared<Node>(Node()));
        }
    }

#pragma omp parallel for num_threads(8)
    for(auto it = tiles.begin(); it < tiles.end(); ++it) {
        // update node parameters
        Node* n = nodes_.at((*it)->getXPos()).at((*it)->getYPos()).get();
        n->visited = false;
        n->x = (*it)->getXPos();
        n->y = (*it)->getYPos();
        n->nodeCost = 1.0f/(*it)->getValue();
        // check and add neighbours
        try{n->neighbours[0] = nodes_.at(n->x+1).at(n->y).get();}
        catch(std::out_of_range){n->neighbours[3] = nullptr;}
        try{n->neighbours[1] = nodes_.at(n->x-1).at(n->y).get();}
        catch(std::out_of_range){n->neighbours[3] = nullptr;}
        try{n->neighbours[2] = nodes_.at(n->x).at(n->y+1).get();}
        catch(std::out_of_range){n->neighbours[3] = nullptr;}
        try{n->neighbours[3] = nodes_.at(n->x).at(n->y-1).get();}
        catch(std::out_of_range){n->neighbours[3] = nullptr;}
    }
}

void AStarController::clearNodes()
{
    // mark all nodes as non-visited
    for(auto &vec: nodes_) {
        for(auto &n: vec) {
            n->visited = false;
        }
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
