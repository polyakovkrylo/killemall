/*!
 * \file worldmodel.cpp
 *
 * WorldModel class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "worldmodel.h"

using std::vector;
using std::unique_ptr;

WorldModel::WorldModel(QObject *parent) :
    QObject(parent), level_{":/img/level1.png"},
    numOfEnemies_{20}, numOfHealthpacks_{20}, ready_{false}
{
    controller_ = unique_ptr<WorldAbstractController>(WorldControllerFactory::createController(this));
}

void WorldModel::init(QString filename, int enemies, int healthpacks)
{
    // clear previous objects
    enemies_.clear();
    pEnemies_.clear();
    healthpacks_.clear();
    controller_->stop();

    // If args were not set then set them to last values
    if(filename.isEmpty()) filename = level_;
    if(!enemies) enemies = numOfEnemies_;
    if(!healthpacks) healthpacks = numOfHealthpacks_;

    // Re-init the map if it's first time or if the map has changed
    if(level_ != filename || !world_.get()) {
        world_.reset(new UWorld(filename));
        level_ = filename;
        controller_->init();
    }

    // remember amount of enemies and healthpack for further restart
    numOfEnemies_ = enemies;
    numOfHealthpacks_ = healthpacks;

    // separate regular and posioned enemies and store them in different vectors
    for(auto &e: world_->createEnemies(enemies)) {
        Enemy *ptr = e.release();
        UPEnemy *pe = dynamic_cast<UPEnemy*>(ptr);
        if(pe != nullptr) {
            pEnemies_.push_back(std::move(unique_ptr<UPEnemy>(pe)));
        }
        else {
            UEnemy *re = dynamic_cast<UEnemy*>(ptr);
            enemies_.push_back(std::move(unique_ptr<UEnemy>(re)));
        }
    }

    // connect poisoned enemies
    for(auto &e: pEnemies_) {
        int x = e->getXPos();
        int y = e->getYPos();
        connect(e.get(),SIGNAL(areaPoisoned(int,QRect)), this, SLOT(poisonArea(int,QRect)));
        connect(e.get(),&UPEnemy::dead,[=](){
            emit enemyDefeated(x,y);
        });
    }

    // connect regular enemies
    for(auto &e: enemies_) {
        int x = e->getXPos();
        int y = e->getYPos();
        connect(e.get(),&UEnemy::dead,[=](){
            protagonist_->restoreEnergy();
            emit enemyDefeated(x,y);
        });
    }

    // connect each healthpack to  healthpackUsed() signal
    healthpacks_ = world_->createHealthpacks(healthpacks);
    for(auto &h: healthpacks_) {
        int x = h->getXPos();
        int y = h->getYPos();
        connect(h.get(),&UHealthPack::used,[=](){
            protagonist_->restoreEnergy();
            emit healthpackUsed(x,y);
        });
    }
    protagonist_.reset();
    protagonist_ = world_->createProtagonist();

    ready_ = true;
    emit reload();
}

void WorldModel::attackEnemy()
{
    int x = protagonist_->getXPos();
    int y = protagonist_->getYPos();
    for(auto it = enemies_.begin(); it < enemies_.end(); ++it){
        if(it->get()->area().contains(x,y)) {
            // garbage collection
            if(it->get()->getDefeated()) {
                enemies_.erase(it);
                // return back cause the vector was shifted after erase
                --it;
                continue;
            }
            // if the enemy is within the area, attack him
            float dmg = it->get()->attack();
            protagonist_->updateHealth(-dmg);
        }
    }

    for(auto it = pEnemies_.begin(); it < pEnemies_.end(); ++it){
        // garbage collection
        if(it->get()->getDefeated()) {
            pEnemies_.erase(it);
            // return back cause the vector was shifted after erase
            --it;
            continue;
        }
        // if the enemy is within the area, attack him
        if(it->get()->area().contains(x,y)) {
            it->get()->attack();
            protagonist_->restoreEnergy();
        }
    }
}

void WorldModel::useHealthpack()
{
    int x = protagonist_->getXPos();
    int y = protagonist_->getYPos();
    for(auto it = healthpacks_.begin(); it < healthpacks_.end(); ++it){
        // garbage collection
        if(!it->get()->getValue()) {
            healthpacks_.erase(it);
            // return back cause the vector was shifted after erase
            --it;
            continue;
        }
        // if the health pack is within the area, use it
        if(it->get()->area().contains(x,y)) {
            protagonist_->updateHealth(it->get()->use());
        }
    }
}

void WorldModel::poisonArea(int value, QRect rect)
{
    // if the hero is inside the area, decrease his health level
    if(rect.contains(protagonist_->getXPos(),protagonist_->getYPos())) {
        protagonist_->updateHealth(-value);
    }
    emit areaPoisoned(value, rect);
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
