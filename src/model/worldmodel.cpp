#include "worldmodel.h"

using std::vector;
using std::unique_ptr;

WorldModel::WorldModel(QObject *parent) : QObject(parent)
{
    controller_ = unique_ptr<WorldAbstractController>(WorldControllerFactory::createController(this));
}

void WorldModel::init(const QString &filename, int enemies, int healthpacks)
{
    World w;
    level_ = QImage(filename);
    world_ = w.createWorld(filename);
    rows_ = w.getRows();
    columns_ = w.getCols();
    enemies_ = w.getEnemies(enemies);
    healthpacks_ = w.getHealthPacks(healthpacks);
    protagonist_ = w.getProtagonist();

    for(auto &e: enemies_) {
        Enemy* re = e.get();
        PEnemy* pe = dynamic_cast<PEnemy*>(re);
        if(pe) {
            connect(pe,SIGNAL(poisonLevelUpdated(int)), this, SLOT(onPEnemyPoisonLevelChanged(int)));
        }
    }

    // optional implementation is to attack enemies and get health packs only
    // while standing (when movement is finished)
    connect(protagonist_.get(), SIGNAL(posChanged(int,int)), this, SLOT(attackEnemy(int,int)));
    connect(protagonist_.get(), SIGNAL(posChanged(int,int)), this, SLOT(useHealthpack(int,int)));

    emit reload();
}

void WorldModel::setHealthLevel(float value)
{
    // TODO: if HP is below 0 emit a signal that game is lost
    if(value<0)
        value=0;
    // increase not higher than 100 hp
    else if(value>100)
        value=100;
    protagonist_->setHealth(value);
    emit healthLevelChanged(value);

}

void WorldModel::setEnergyLevel(float value)
{
    // TODO: if energy is below 0 emit a signal
    if(value<0)
        value=0;
    // increase not higher than 100 hp
    else if(value>100)
        value=100;
    protagonist_->setEnergy(value);
    emit energyLevelChanged(value);
}

void WorldModel::attackEnemy(int x, int y)
{
    // get protagonist area
    QRect rect(QPoint(x-activeRadius_,y-activeRadius_),
               QPoint(x+activeRadius_,y+activeRadius_));
    for(auto &e: enemies_){
        int x = e->getXPos();
        int y = e->getYPos();
        if(rect.contains(x,y)) {
            // if the enemy is within the area, attack him
            e->setDefeated(true);
            emit enemyDefeated(x,y);
            // if the enemy is poisoned, start posion() slot
            Enemy* re = e.get();
            PEnemy* pe = dynamic_cast<PEnemy*>(re);
            if(pe) {
                pe->poison();
            } else {
                // otherwise just decrease the health level
                setHealthLevel(protagonist_->getHealth() - e->getValue());
            }
            // TODO: add removing of defeated enemy
        }
    }
}

void WorldModel::useHealthpack(int x, int y)
{
    // get protagonist area
    QRect rect(QPoint(x-activeRadius_,y-activeRadius_),
               QPoint(x+activeRadius_,y+activeRadius_));
    for(auto &h: healthpacks_){
        // if the health pack is within the area, use it
        int x = h->getXPos();
        int y = h->getYPos();
        if(rect.contains(x,y)) {
            setHealthLevel(protagonist_->getHealth() + h->getValue());
            emit healthpackUsed(x,y);
            // TODO: add removing of used healthpack
        }
    }
}

void WorldModel::onPEnemyPoisonLevelChanged(int value)
{
    // get poison area
    PEnemy *p = qobject_cast<PEnemy*>(sender());
    QRect rect(QPoint(p->getXPos()-poisonRadius_,p->getYPos()-poisonRadius_),
                 QPoint(p->getXPos()+poisonRadius_,p->getYPos()+poisonRadius_));
    // if the hero is inside the area, decrease his health level
    if(rect.contains(protagonist_->getXPos(),protagonist_->getYPos())) {
        setHealthLevel(protagonist_->getHealth() - value);
    }
}

void WorldModel::move(int x, int y)
{
    move(QPoint(x,y));
}

void WorldModel::move(const QPoint &pos)
{
    QPoint from(protagonist_->getXPos(),protagonist_->getYPos());
    controller_->move(from,pos);
}

