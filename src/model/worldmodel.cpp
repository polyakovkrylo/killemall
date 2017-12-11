#include "worldmodel.h"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;

WorldModel::WorldModel(QObject *parent) : QObject(parent)
{
    controller_ = unique_ptr<WorldAbstractController>(WorldControllerFactory::createController(this));
}

void WorldModel::init(const QString &filename, int enemies, int healthpacks)
{
    world_ = unique_ptr<UWorld>(new UWorld(filename));
    level_ = QImage(filename);

    for(auto &e: world_->createEnemies(enemies)) {
        // separate regular and posioned enemies and stroe them in different vectors
        UPEnemy* pe = dynamic_cast<UPEnemy*>(e);
        if(pe != nullptr) {
            pEnemies_.push_back(shared_ptr<UPEnemy>(pe));
            connect(pe,SIGNAL(areaPoisoned(int,QRect)), this, SLOT(poisonArea(int,QRect)));
            connect(pe,&UPEnemy::dead,[=](){
                protagonist_->restoreEnergy();
                emit enemyDefeated(pe->getXPos(),pe->getYPos());
            });
        } else {
            UEnemy* re = dynamic_cast<UEnemy*>(e);
            enemies_.push_back(shared_ptr<UEnemy>(re));
            connect(re,&UEnemy::dead,[=](){
                protagonist_->restoreEnergy();
                emit enemyDefeated(re->getXPos(),re->getYPos());
            });
        }
    }

    healthpacks_ = world_->createHealthpacks(healthpacks);
    // connect each healthpack to  healthpackUsed() signal
    for(auto &h: healthpacks_) {
        UHealthPack* hp = h.get();
        connect(hp,&UHealthPack::used,[=](){
            protagonist_->restoreEnergy();
            emit healthpackUsed(hp->getXPos(),hp->getYPos());
        });
    }
    protagonist_ = world_->createProtagonist();

    // optional implementation is to attack enemies and get health packs only
    // while standing (when movement is finished)
    connect(protagonist_.get(), SIGNAL(posChanged(int,int)), this, SLOT(attackEnemy(int,int)));
    connect(protagonist_.get(), SIGNAL(posChanged(int,int)), this, SLOT(useHealthpack(int,int)));

    emit reload();
}

void WorldModel::attackEnemy(int x, int y)
{
    for(auto &e: enemies_){
        if(e->area().contains(x,y)) {
            // if the enemy is within the area, attack him
            float dmg = e->attack();
            protagonist_->updateHealth(-dmg);
        }
    }

    for(auto &pe: pEnemies_){
        if(pe->area().contains(x,y)) {
            // if the enemy is within the area, attack him
            pe->poison();
        }
    }
}

void WorldModel::useHealthpack(int x, int y)
{
    for(auto &h: healthpacks_){
        // if the health pack is within the area, use it
        if(h->area().contains(x,y)) {
            protagonist_->updateHealth(h->use());
        }
    }
}

void WorldModel::poisonArea(int value, QRect rect)
{
    // if the hero is inside the area, decrease his health level
    if(rect.contains(protagonist_->getXPos(),protagonist_->getYPos())) {
        protagonist_->updateHealth(-value);
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

