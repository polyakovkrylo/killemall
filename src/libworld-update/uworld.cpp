#include "uworld.h"

using std::unique_ptr;
using std::vector;

//=============== Health pack =============================
UHealthPack::UHealthPack(int x, int y, float healthPoints, int radius):
    Tile(x,y,healthPoints), area_(x-radius, y-radius, radius*2, radius*2)
{

}

float UHealthPack::use()
{
    emit used();
    return getValue();
}

//===================== Enemy =============================
UEnemy::UEnemy(int x, int y, float strength, int radius) :
    Enemy(x,y,strength), area_(x-radius, y-radius, radius*2, radius*2)
{

}

float UEnemy::attack()
{
    emit dead();
    return getValue();
}

//==================== PEnemy =============================
UPEnemy::UPEnemy(int x, int y, float strength,
                 int radius, int poisonRadius) :
    PEnemy(x,y,strength),
    area_(x-radius, y-radius, radius*2, radius*2),
    poisonArea_(x-poisonRadius, y-poisonRadius, poisonRadius*2, poisonRadius*2)
{
    connect(this, &PEnemy::poisonLevelUpdated, [=] (int value) {
        emit areaPoisoned(value,poisonArea_);
    });
}

//================== Protagonist ==========================
UProtagonist::UProtagonist(int radius):
    area_(-radius, -radius, radius*2, radius*2)
{

}

void UProtagonist::updateHealth(int diff)
{
    float hp = getHealth() + diff;
    // health should be between 0 and 100
    if(hp < 0){
        hp = 0;
        emit dead();
    } else if(hp > 100) {
        hp = 100;
    }
    setHealth(hp);
    emit healthLevelChanged(hp);
}

void UProtagonist::updateEnergy(int diff)
{
    float en = getEnergy() + diff;
    setEnergy(en);
    emit healthLevelChanged(en);
}

//===================== World =============================
UWorld::UWorld(QString filename)
{
    map_ = world_.createWorld(filename);
}

vector<Enemy*> UWorld::createEnemies(unsigned int enemies)
{
    vector<Enemy*> v;
    v.reserve(enemies);
    for(auto &e: world_.getEnemies(enemies)) {
        Enemy* ptr;
        Enemy* re = e.release();
        PEnemy* pe = dynamic_cast<PEnemy*>(re);
        if(pe != nullptr) {
            // create UPEnemy for each PEnemy
            ptr = new UPEnemy(pe->getXPos(), pe->getYPos(), pe->getValue());
        } else {
            // create UEnemy for each Enemy
            ptr = new UEnemy(re->getXPos(), re->getYPos(), re->getValue());
        }
        v.push_back(ptr);
    }
    return v;
}

vector<unique_ptr<UHealthPack>> UWorld::createHealthpacks(unsigned int packs)
{
    vector<unique_ptr<UHealthPack>> v;
    v.reserve(packs);
    for(auto &h: world_.getHealthPacks(packs)) {
        // create UHealthPack for each health pack
        v.push_back(unique_ptr<UHealthPack>(new UHealthPack(h->getXPos(), h->getYPos(), h->getValue())));
    }
    return v;
}

unique_ptr<UProtagonist> UWorld::createProtagonist()
{
    auto p = unique_ptr<UProtagonist>(new UProtagonist);
    // find first point on the map with non-zero value and move the protagonist there
    for(auto &t: map_) {
        if(t->getValue()) {
            p->setPos(t->getXPos(), t->getYPos());
            break;
        }
    }
    return p;
}
