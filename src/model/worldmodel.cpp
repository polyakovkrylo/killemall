#include "worldmodel.h"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::dynamic_pointer_cast;

WorldModel::WorldModel(QObject *parent) : QObject(parent)
{
    controller_ = unique_ptr<WorldAbstractController>(WorldControllerFactory::createController(this));
}

void WorldModel::init(const QString &filename, int enemies, int healthpacks)
{
    world_ = unique_ptr<UWorld>(new UWorld(filename));
    level_ = QImage(filename);
    controller_->init();

    for(auto &e: world_->createEnemies(enemies)) {
        // separate regular and posioned enemies and stroe them in different vectors
        auto pe = dynamic_pointer_cast<UPEnemy>(e);
        if(pe != nullptr) {
            pEnemies_.push_back(std::move(pe));
            connect(pe.get(),SIGNAL(areaPoisoned(int,QRect)), this, SLOT(poisonArea(int,QRect)));
            connect(pe.get(),&UPEnemy::dead,[=](){
                protagonist_->restoreEnergy();
                emit enemyDefeated(pe->getXPos(),pe->getYPos());
                pEnemies_.removeOne(pe);
            });
        } else {
            auto re = dynamic_pointer_cast<UEnemy>(e);
            enemies_.push_back(std::move(re));
            connect(re.get(),&UEnemy::dead,[=](){
                protagonist_->restoreEnergy();
                emit enemyDefeated(re->getXPos(),re->getYPos());
                enemies_.removeOne(re);
            });
        }
    }

    healthpacks_ = (world_->createHealthpacks(healthpacks)).toList();
    // connect each healthpack to  healthpackUsed() signal
    for(auto &h: healthpacks_) {
        connect(h.get(),&UHealthPack::used,[=](){
            protagonist_->restoreEnergy();
            emit healthpackUsed(h->getXPos(),h->getYPos());
            healthpacks_.removeOne(h);
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
            pe->attack();
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

