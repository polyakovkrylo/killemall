#ifndef UWORLD_H
#define UWORLD_H

#include "world.h"

class UHealthPack : public QObject, public Tile
{
    Q_OBJECT
public:
    UHealthPack(int x, int y, float healthPoints, int radius = 10);
    float use();
    inline const QRect &area() const{return area_;}

private:
    QRect area_;

signals:
    void used();
};


class UEnemy : public QObject, public Enemy
{
    Q_OBJECT
public:
    explicit UEnemy(int xPosition, int yPosition, float strength, int radius = 10);
    float attack();
    inline const QRect &area() const {return area_;}

private:
    QRect area_;

signals:
    void dead();
};


class UPEnemy : public PEnemy
{
public:
    UPEnemy(int x, int y, float strength,
            int radius = 10, int poisonRadius = 20);
    inline const QRect &area() {return area_;}
    inline const QRect &poisonArea() {return poisonArea_;}
    float attack();
private:
    QRect area_;
    QRect poisonArea_;
};

class UProtagonist : public Protagonist
{
    Q_OBJECT
public:
    UProtagonist();
    void updateHealth(int diff);
    void updateEnergy(int diff);

signals:
    void healthLevelChanged(int value);
    void energyLevelChanged(int value);
    void dead();

};


class UWorld
{
public:
    UWorld(QString filename);
    std::vector<std::unique_ptr<Enemy>> createEnemies(unsigned int enemies);
    std::vector<std::unique_ptr<UHealthPack> > createHealthpacks(unsigned int packs);
    std::unique_ptr<UProtagonist> createProtagonist();

private:
    World world_;
    std::vector<std::unique_ptr<Tile>> map_;
};

#endif // UWORLD_H
