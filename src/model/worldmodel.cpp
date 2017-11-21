#include "worldmodel.h"

using std::vector;
using std::unique_ptr;

WorldModel::WorldModel(QObject *parent) : QObject(parent)
{

}

void WorldModel::init(const QString &filename, int enemies, int healthpacks)
{
    World w;
    world_ = w.createWorld(filename);
    enemies_ = w.getEnemies(enemies);
    healthpacks_ = w.getHealthPacks(healthpacks);
    protagonist_ = w.getProtagonist();
}

void WorldModel::attackEnemy(const std::unique_ptr<Enemy> &enemy)
{
    Q_UNUSED(enemy);
}

void WorldModel::useHealthpack(const std::unique_ptr<Tile> &pack)
{
    Q_UNUSED(pack);
}

