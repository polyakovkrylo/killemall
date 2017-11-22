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
    level_ = filename;
    world_ = w.createWorld(level_);
    rows_ = w.getRows();
    columns_ = w.getCols();
    enemies_ = w.getEnemies(enemies);
    healthpacks_ = w.getHealthPacks(healthpacks);
    protagonist_ = w.getProtagonist();
    emit reload();
}

void WorldModel::attackEnemy(const std::unique_ptr<Enemy> &enemy)
{
    Q_UNUSED(enemy);
}

void WorldModel::useHealthpack(const std::unique_ptr<Tile> &pack)
{
    Q_UNUSED(pack);
}

void WorldModel::move(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
}

void WorldModel::move(const QPoint &pos)
{
    move(pos.x(), pos.y());
}

