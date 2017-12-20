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
    connect(model_, SIGNAL(protagonistDead()), this,SLOT(stop()));
}

void WorldStrategy::run()
{
    connect(model_->getController().get(),SIGNAL(animationDone()),
                   this, SLOT(nextMove()));
    nextMove();
}

void WorldStrategy::run(bool b)
{
    if(b)
        run();
    else
        stop();
}

void WorldStrategy::stop()
{
    disconnect(model_->getController().get(),SIGNAL(animationDone()),
               this, SLOT(nextMove()));
}

void WorldStrategy::nextMove()
{
    auto hp = model_->getProtagonist()->getHealth();
    auto tile = model_->getController()->findClosest(AnyEnemy, 0.0f, hp);
    if(!tile) tile = model_->getController()->findClosest(HealthPack);
    if(tile)
        model_->move(tile->getXPos(), tile->getYPos());
    else{
        stop();
        emit finished();
    }
}
