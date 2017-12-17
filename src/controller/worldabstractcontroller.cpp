#include "worldabstractcontroller.h"
#include "model/worldmodel.h"

using std::shared_ptr;
using std::vector;

WorldAbstractController::WorldAbstractController(WorldModel *model) :
    QObject(model), model_{model}, path_{0,QVector<QPoint>()}
{
    animation_.setSingleShot(true);
    animation_.setInterval(10);
    connect(&animation_,SIGNAL(timeout()),SLOT(animatePath()));
}

void WorldAbstractController::move(const QPoint &from, const QPoint &to)
{
    bool scs = false;
    if(path_.steps.back() == to)
        // if it is the same path as last time, then just move
        scs = true;
    else
        // otherwise try to find a path
        scs =findPath(from, to);
    if(scs) {
        model_->getProtagonist()->updateEnergy(-path_.cost);
        animatePath();
    }
}

const Tile WorldAbstractController::findClosest(ObjectType type, float minValue, float maxValue)
{
    // clear path and get starting pos
    path_.cost = INFINITY;
    QPoint from(model_->getProtagonist()->getXPos(),
                model_->getProtagonist()->getYPos());
    // vector of objects under investigation
    vector<Tile> objs;

    // adding objects to vector
    switch(type) {
    case HealthPack:
        for(auto &h: model_->getHealthpacks()) {
            objs.push_back(*h);
        }
        break;
    case RegularEnemy:
        for(auto &e: model_->getEnemies()) {
            objs.push_back(*e);
        }
        break;
    case PoisonedEnemy:
        for(auto &e: model_->getPEnemies()) {
            objs.push_back(*e);
        }
        break;
    case AnyEnemy:
        // adding both types for AnyEnemy
        for(auto &e: model_->getEnemies()) {
            objs.push_back(*e);
        }
        for(auto &pe: model_->getPEnemies()) {
            objs.push_back(*pe);
        }
        break;
    default: break;
    }

    Tile *closest = nullptr;
    for(auto &obj: objs) {
        // check if tile's value is within range
        float val = obj.getValue();
        if(val > maxValue || val < minValue)
            continue;

        // check if the object can ever be better than the closest found
        QPoint to(obj.getXPos(), obj.getYPos());
        QPoint p = from - to;
        if(path_.cost < p.manhattanLength()*minCost_)
            continue;

        // check if the object is closer than the closest so far
        bool s = findPath(from,to,path_.cost);
        if(s)
            closest = &obj;
    }

    return *closest;
}

void WorldAbstractController::animatePath()
{
    // move protagonist along the path till the path is done
    if(!path_.steps.empty()) {
        QPoint pos(path_.steps.takeFirst());
        model_->getProtagonist()->setPos(pos.x(),pos.y());
        animation_.start();
    }
}
