#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <QObject>

#include <world.h>
#include "controller/worldcontrollerfactory.h"

class WorldModel : public QObject
{
    Q_OBJECT
public:
    explicit WorldModel(QObject *parent = 0);

    void init(const QString &filename, int enemies = 20, int healthpacks = 20);
    inline const std::vector<std::unique_ptr<Tile>> & getWorld() const {return world_;}
    inline const std::vector<std::unique_ptr<Enemy>> & getEnemies() const {return enemies_;}
    inline const std::vector<std::unique_ptr<Tile>> & getHealthpacks() const {return healthpacks_;}
    inline const std::unique_ptr<Protagonist> & getProtagonist() const {return protagonist_;}
    inline const std::unique_ptr<WorldAbstractController> & getController() const {return controller_;}
    inline const QImage & getLevel() const {return level_;}
    inline int getRows() const {return rows_;}
    inline int getColumns() const {return columns_;}
    void attackEnemy(const std::unique_ptr<Enemy> &enemy);
    void useHealthpack(const std::unique_ptr<Tile> &pack);

private:
    std::vector<std::unique_ptr<Tile>> world_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Tile>> healthpacks_;
    std::unique_ptr<Protagonist> protagonist_;
    std::unique_ptr<WorldAbstractController> controller_;
    QImage level_;
    int rows_;
    int columns_;

signals:
    void reload();
    void healthpackUsed(int x, int y);
    void enemyDefeated(int x, int y);
    void healthLevelChanged(int value);
    void energyLevelChanged(int value);

public slots:
    void move(int x,int y);
    void move(const QPoint &pos);
};

#endif // WORLDMODEL_H
