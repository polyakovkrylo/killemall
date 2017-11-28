/*!
 * \file worldmodel.h
 *
 * WorldModel class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include "libworld-update/uworld.h"
#include "controller/worldcontrollerfactory.h"

class WorldModel : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param parent parent object, passed to QObject constructor
     */
    explicit WorldModel(QObject *parent = 0);

    /*!
     * \brief Initialization of the model
     *
     * Creates the world, enemies, health packs and protagonist and
     * stores it in separate containers, assignes level name to level_
     *
     * \param filename level image filename
     * \param enemies number of enemies to create
     * \param healthpacks number of healthpacks to create
     */
    void init(QString filename = "", int enemies = 0, int healthpacks = 0);
    inline const std::unique_ptr<UWorld> & getWorld() const {return world_;}
    inline const std::vector<std::unique_ptr<UEnemy>> & getEnemies() const {return enemies_;}
    inline const std::vector<std::unique_ptr<UPEnemy>> & getPEnemies() const {return pEnemies_;}
    inline const std::vector<std::unique_ptr<UHealthPack>> & getHealthpacks() const {return healthpacks_;}
    inline const std::unique_ptr<UProtagonist> & getProtagonist() const {return protagonist_;}
    inline const std::unique_ptr<WorldAbstractController> & getController() const {return controller_;}
    inline const QString & getLevel() const {return level_;}
    inline bool ready() const {return ready_;}

private:
    std::unique_ptr<UWorld> world_;
    std::vector<std::unique_ptr<UEnemy>> enemies_;
    std::vector<std::unique_ptr<UPEnemy>> pEnemies_;
    std::vector<std::unique_ptr<UHealthPack>> healthpacks_;
    std::unique_ptr<UProtagonist> protagonist_;
    std::unique_ptr<WorldAbstractController> controller_;
    QString level_;
    int numOfEnemies_;
    int numOfHealthpacks_;
    bool ready_;

signals:
    /*!
     * \brief Reload signal
     * Emited every time init() is called
     */
    void reload();
    /*!
     * \brief Health pack is used signal
     * Emited every time one of health packs is used
     * \param x horizontal position of the used health pack
     * \param y vertical position of the used health pack
     */
    void healthpackUsed(int x, int y);
    /*!
     * \brief Enemy is defeated signal
     * Emited every time one of enemies is defeated
     * \param x horizontal position of the defeated enemy
     * \param y vertical position of the defeated enemy
     */
    void enemyDefeated(int x, int y);
    void areaPoisoned(int value, QRect rect);
    void protagonistDead();

public slots:
    void attackEnemy();
    void useHealthpack();
    void poisonArea(int value, QRect rect);
    void move(int x,int y);

    /*!
     * \brief Move the protagonist to a certain position
     *
     * This is an overloaded function
     *
     * \sa move(int x,int y)
     *
     * \param x vertical coordinate of a new position
     * \param y horizontal coordinate of a new position
     */
    void move(const QPoint &pos);
};

#endif // WORLDMODEL_H
