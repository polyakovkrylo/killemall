#ifndef UWORLD_H
#define UWORLD_H

#include "world.h"

class UHealthPack : public QObject, public Tile
{
    Q_OBJECT
public:
    UHealthPack(int x, int y, float healthPoints, int radius = 5);
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
    explicit UEnemy(int xPosition, int yPosition, float strength, int radius = 5);
    float attack();
    inline const QRect &area() const {return area_;}

private:
    QRect area_;

signals:
    void dead();
};


class UPEnemy : public PEnemy
{
    Q_OBJECT
public:
    UPEnemy(int x, int y, float strength,
            int radius = 5, int poisonRadius = 10);
    inline const QRect &area() {return area_;}
    inline const QRect &poisonArea() {return poisonArea_;}
private:
    QRect area_;
    QRect poisonArea_;
signals:
    void areaPoisoned(int value, QRect area);
};

class UProtagonist : public Protagonist
{
    Q_OBJECT
public:
    UProtagonist(int radius = 5);
    void updateHealth(float diff);
    void updateEnergy(float diff);
    inline const QRect &area() const {return area_;}

private:
    QRect area_;

signals:
    void healthLevelChanged(int value);
    void energyLevelChanged(int value);
    void dead();

public slots:
    void restoreEnergy();
};


class UWorld
{
public:
    UWorld(QString filename);
    inline const std::vector<std::unique_ptr<Tile>> &getMap() const {return map_;}
    std::vector<Enemy*> createEnemies(unsigned int enemies);
    std::vector<std::shared_ptr<UHealthPack> > createHealthpacks(unsigned int packs);
    std::unique_ptr<UProtagonist> createProtagonist();

private:
    World world_;
    std::vector<std::unique_ptr<Tile>> map_;
};

#endif // UWORLD_H
