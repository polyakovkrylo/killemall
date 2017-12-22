/*!
 * \file worldstrategy.cpp
 *
 * WorldStrategy class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "worldstrategy.h"

WorldStrategy::WorldStrategy(QObject *parent) :
    QObject(parent), model_{nullptr}
{

}

void WorldStrategy::setModel(WorldModel *model)
{
    model_ = model;
    // stop the strategy if protagonist is dead
    connect(model_, SIGNAL(protagonistDead()), this,SLOT(stop()));
}

void WorldStrategy::run()
{
    // every time the move is done, do the next move
    connect(model_->getController().get(),SIGNAL(animationDone()),
            this, SLOT(nextMove()));
    // recalculate next move each time protagonist is poisoned
    connect(model_->getProtagonist().get(), SIGNAL(poisoned()),
            this, SLOT(nextMove()));
    // make first move
    nextMove();
}

void WorldStrategy::run(bool b)
{
    // start/stop the strategy
    if(b)
        run();
    else
        stop();
}

void WorldStrategy::stop()
{
    // see WorldStrategy::run()
    disconnect(model_->getController().get(),SIGNAL(animationDone()),
               this, SLOT(nextMove()));
    disconnect(model_->getProtagonist().get(), SIGNAL(poisoned()),
               this, SLOT(nextMove()));
}

void WorldStrategy::nextMove()
{
    // maximal strengh of the enemy should be less than health level
    auto hp = model_->getProtagonist()->getHealth();
    // try to find an enemty with strength less than health level
    auto tile = model_->getController()->findClosest(AnyEnemy, 0.0f, hp);
    // if no such enemy, search for a health pack
    if(!tile) tile = model_->getController()->findClosest(HealthPack);
    // move to the object
    if(tile)
        model_->move(tile->getXPos(), tile->getYPos());
    // if no health pack was found, stop the strategy
    else{
        stop();
        emit finished();
    }
}
