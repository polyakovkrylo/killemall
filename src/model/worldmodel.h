#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include "libworld-update/uworld.h"
#include "controller/worldcontrollerfactory.h"

class WorldModel : public QObject
{
    Q_OBJECT
public:
    explicit WorldModel(QObject *parent = 0);

    void init(QString filename = "", int enemies = 0, int healthpacks = 0);
    inline const std::unique_ptr<UWorld> & getWorld() const {return world_;}
    inline const QList<std::shared_ptr<UEnemy>> & getEnemies() const {return enemies_;}
    inline const QList<std::shared_ptr<UPEnemy>> & getPEnemies() const {return pEnemies_;}
    inline const QList<std::shared_ptr<UHealthPack>> & getHealthpacks() const {return healthpacks_;}
    inline const std::unique_ptr<UProtagonist> & getProtagonist() const {return protagonist_;}
    inline const std::unique_ptr<WorldAbstractController> & getController() const {return controller_;}
    inline const QString & getLevel() const {return level_;}

private:
    std::unique_ptr<UWorld> world_;
    QList<std::shared_ptr<UEnemy>> enemies_;
    QList<std::shared_ptr<UPEnemy>> pEnemies_;
    QList<std::shared_ptr<UHealthPack>> healthpacks_;
    std::unique_ptr<UProtagonist> protagonist_;
    std::unique_ptr<WorldAbstractController> controller_;
    QString level_;
    int numOfEnemies_;
    int numOfHealthpacks_;

signals:
    void reload();
    void healthpackUsed(int x, int y);
    void enemyDefeated(int x, int y);

public slots:
    void attackEnemy(int x, int y);
    void useHealthpack(int x, int y);
    void poisonArea(int value, QRect rect);
    void move(int x,int y);
    void move(const QPoint &pos);
};

#endif // WORLDMODEL_H
