#include "worldstrategy.h"

WorldStrategy::WorldStrategy(QObject *parent) :
    QObject(parent), model_{nullptr}
{

}

void WorldStrategy::setModel(WorldModel *model)
{
    model_ = model;
}

void WorldStrategy::run()
{
    if(!model_)
        return;
}

void WorldStrategy::stop()
{
    if(!model_)
        return;
}
