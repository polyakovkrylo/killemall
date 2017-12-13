#include "uworld.h"

using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::isinf;

//=============== Health pack =============================
UHealthPack::UHealthPack(int x, int y, float healthPoints, int radius):
    Tile(x,y,healthPoints), area_(x-radius, y-radius, radius*2, radius*2)
{

}

float UHealthPack::use()
{
    float hp = getValue();
    if(hp)
        emit used();
    setValue(0.0);
    return hp;
}

//===================== Enemy =============================
UEnemy::UEnemy(int x, int y, float strength, int radius) :
    Enemy(x,y,strength), area_(x-radius, y-radius, radius*2, radius*2)
{

}

float UEnemy::attack()
{
    if(!getDefeated()) {
        setDefeated(true);
        emit dead();
        return getValue();
    } else {
        return 0.0f;
    }
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

void UProtagonist::updateHealth(float diff)
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

void UProtagonist::updateEnergy(float diff)
{
    float en = getEnergy() + diff;
    if(en < 0){
        en = 0;
        emit dead();
    } else if(en > 100) {
        en = 100;
    }
    setEnergy(en);
    emit energyLevelChanged(en);
}

void UProtagonist::restoreEnergy()
{
    updateEnergy(100);
}

//===================== World =============================
UWorld::UWorld(QString filename)
{
    for(auto &t: world_.createWorld(filename)) {
        map_.push_back(std::move(t));
    }
}

vector<Enemy*> UWorld::createEnemies(unsigned int enemies)
{
    vector<Enemy*> v;
    v.reserve(enemies);
    for(auto &e: world_.getEnemies(enemies)) {
        Enemy* ptr;
        // get not release, these instances should be deleted
        Enemy* re = e.get();
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

vector<shared_ptr<UHealthPack>> UWorld::createHealthpacks(unsigned int packs)
{
    vector<shared_ptr<UHealthPack>> v;
    v.reserve(packs);
    for(auto &h: world_.getHealthPacks(packs)) {
        // create UHealthPack for each health pack
        v.push_back(shared_ptr<UHealthPack>(new UHealthPack(h->getXPos(), h->getYPos(), h->getValue())));
    }
    return v;
}

unique_ptr<UProtagonist> UWorld::createProtagonist()
{
    auto p = unique_ptr<UProtagonist>(new UProtagonist);
    // find first point on the map with non-zero value and move the protagonist there
    for(auto &t: map_) {
        if(!isinf(t->getValue())) {
            p->setPos(t->getXPos(), t->getYPos());
            break;
        }
    }
    return p;
}
