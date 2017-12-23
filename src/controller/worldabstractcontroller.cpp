/*!
 * \file worldabstractcontroller.cpp
 *
 * WorldAbstractController class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "worldabstractcontroller.h"
#include "model/worldmodel.h"

using std::shared_ptr;
using std::vector;

WorldAbstractController::WorldAbstractController(WorldModel *model) :
    QObject(model), model_{model}, path_{0,std::vector<QPair<QPoint,float>>()}
{
    animation_.setSingleShot(true);
    animation_.setInterval(10);
    connect(&animation_,SIGNAL(timeout()),SLOT(animatePath()));
}

bool WorldAbstractController::move(const QPoint &from, const QPoint &to)
{
    bool scs = false;
    // check if 'to' point from the previous pathfinding is the same
    QPoint prev = (!path_.steps.empty()) ? path_.steps.at(0).first : QPoint();
    if(prev == to)
        // if it is the same path as last time, then just move
        scs = true;
    else
        // otherwise try to find a path such that protagonist has enough energy
        scs =findPath(from, to, model_->getProtagonist()->getEnergy());
    if(scs) {
        animatePath();
    }
    return scs;
}

Tile *WorldAbstractController::findClosest(ObjectType type, float minValue, float maxValue)
{
    stop();
    // clear path and get starting pos
    path_.cost = model_->getProtagonist()->getEnergy();
    QPoint from(model_->getProtagonist()->getXPos(),
                model_->getProtagonist()->getYPos());
    // vector of objects under investigation
    vector<Tile*> objs;

    // adding objects to vector
    switch(type) {
    case HealthPack:
        for(auto &h: model_->getHealthpacks()) {
            if(h->getValue())
                objs.push_back(h.get());
        }
        break;
    case RegularEnemy:
        for(auto &e: model_->getEnemies()) {
            if(!e->getDefeated())
                objs.push_back(e.get());
        }
        break;
    case PoisonedEnemy:
        for(auto &e: model_->getPEnemies()) {
            if(!e->getDefeated())
                objs.push_back(e.get());
        }
        break;
    case AnyEnemy:
        // adding both types for AnyEnemy
        for(auto &e: model_->getEnemies()) {
            if(!e->getDefeated())
                objs.push_back(e.get());
        }
        for(auto &pe: model_->getPEnemies()) {
            if(!pe->getDefeated())
                objs.push_back(pe.get());
        }
        break;
    default: break;
    }

    Tile *closest = nullptr;
    for(auto &obj: objs) {
        // check if tile's value is within range
        float val = obj->getValue();
        if(val >= maxValue || val <= minValue)
            continue;

        // check if the object can ever be better than the closest found
        QPoint to(obj->getXPos(), obj->getYPos());
        QPoint p = from - to;
        if(path_.cost < p.manhattanLength()*minCost_)
            continue;

        // check if the object is closer than the closest so far
        bool s = findPath(from,to,path_.cost);
        if(s)
            closest = obj;
    }

    return closest;
}

void WorldAbstractController::setMinCost(float value)
{
    if(minCost_ != value){
        minCost_ = value/1000.0f;
        init();
    }
}

void WorldAbstractController::stop()
{
    animation_.stop();
    path_.steps.clear();
    path_.cost = 0.0f;
}

void WorldAbstractController::setAnimationSpeed(int value)
{
    animation_.setInterval(300/value);
}

void WorldAbstractController::animatePath()
{
    // move protagonist along the path till the path is done
    if(!path_.steps.empty()) {
        // the path vector is reversed('to' point is the first element in vector)
        QPoint pos(path_.steps.back().first);
        model_->getProtagonist()->setPos(pos.x(),pos.y());
        // update energy after each step
        model_->getProtagonist()->updateEnergy(-path_.steps.back().second);
        path_.steps.pop_back();
        animation_.start();
    }
    // check for health packs and enemies when the movement is done
    else {
        model_->useHealthpack();
        model_->attackEnemy();
        emit animationDone();

        // hero can only die if the deadly damage was done at
        // moment of coming to the point. If the hero was poisoned,
        // he still has one move to search for a health pack
        if(model_->getProtagonist()->getHealth() <= 0) emit model_->protagonistDead();

        // when the hero runs out of energy, he can no longer move.
        // This means he can't attack any more enemies or pick up any
        // more health packs
        if(model_->getProtagonist()->getEnergy() <= minCost_) emit model_->protagonistNoEnergy();
    }
}
