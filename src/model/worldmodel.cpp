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

    // optional implementation is to attack enemies and get health packs only
    // while standing (when movement is finished)
    connect(protagonist_.get(), SIGNAL(posChanged(int,int)), this, SLOT(attackEnemy(int,int)));
    connect(protagonist_.get(), SIGNAL(posChanged(int,int)), this, SLOT(useHealthpack(int,int)));

    emit reload();
}

void WorldModel::attackEnemy(int x, int y)
{
    QRect rect(QPoint(x-activeRadius_,y-activeRadius_),
               QPoint(x+activeRadius_,y+activeRadius_));
    for(auto &e: enemies_){
        int x = e->getXPos();
        int y = e->getYPos();
        if(rect.contains(x,y)) {
            e->setDefeated(true);
            emit enemyDefeated(x,y);
            // TODO: add removing of defeated enemy
        }
    }
}

void WorldModel::useHealthpack(int x, int y)
{
    QRect rect(QPoint(x-activeRadius_,y-activeRadius_),
               QPoint(x+activeRadius_,y+activeRadius_));
    for(auto &h: healthpacks_){
        int x = h->getXPos();
        int y = h->getYPos();
        if(rect.contains(x,y)) {
            emit healthpackUsed(x,y);
            // TODO: add removing of used healthpack
        }
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

