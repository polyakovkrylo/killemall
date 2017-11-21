#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <world.h>
#include <QObject>

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
    void attackEnemy(const std::unique_ptr<Enemy> &enemy);
    void useHealthpack(const std::unique_ptr<Tile> &pack);

private:
    std::vector<std::unique_ptr<Tile>> world_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Tile>> healthpacks_;
    std::unique_ptr<Protagonist> protagonist_;

signals:
    void reload();
    void healthpackUsed(const std::unique_ptr<Tile> &pack);
    void enemyDefeated(const std::unique_ptr<Enemy> &enemy);
    void healthLevelChanged(float value);
    void energyLevelChanged(float value);


};

#endif // WORLDMODEL_H
