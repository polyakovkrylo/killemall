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

/*!
 * \defgroup model
 *
 * \brief Model component implementation
 */

/*!
 * \ingroup model
 * \brief Model component implementation
 *
 * Stores all the objects created in the world
 * and enables their interactions.
 */
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
     * Creates the world, enemies, poisoned enemies,
     * health packs and protagonist and stores it in separate
     * containers, create level image and stores it in level_
     *
     * \param filename level image filename
     * \param enemies number of enemies to create
     * \param healthpacks number of healthpacks to create
     */
    void init(QString filename = "", int enemies = 0, int healthpacks = 0);
    /*!
     * \brief Return World instance of the model
     */
    inline const std::unique_ptr<UWorld> & getWorld() const {return world_;}
    /*!
     * \brief Returns vector of unique pointers to regular enemies
     */
    inline const std::vector<std::unique_ptr<UEnemy>> & getEnemies() const {return enemies_;}
    /*!
     * \brief Returns vector of unique pointers to poisoned enemies
     */
    inline const std::vector<std::unique_ptr<UPEnemy>> & getPEnemies() const {return pEnemies_;}
    /*!
     * \brief Returns vector of unique pointers to health packs
     */
    inline const std::vector<std::unique_ptr<UHealthPack>> & getHealthpacks() const {return healthpacks_;}
    /*!
     * \brief Returns unique pointer to protagonist
     */
    inline const std::unique_ptr<UProtagonist> & getProtagonist() const {return protagonist_;}
    /*!
     * \brief Returns unique pointer to controller
     */
    inline const std::unique_ptr<WorldAbstractController> & getController() const {return controller_;}
    /*!
     * \brief Returns level map image
     */
    inline const QString & getLevel() const {return level_;}
    /*!
     * \brief Check if the model is initialized
     */
    inline bool ready() const {return ready_;}
    /*!
     * \brief Check if all enemies are dead
     */
    void checkIfWin();

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
    /*!
     * \brief Area is poisoned signal
     *
     * Emited every time a certain area is poisoned
     *
     * \param value level of poison
     * \param rect poisoned area
     * \sa poisonArea
     */
    void areaPoisoned(int value, QRect rect);
    /*!
     * \brief Win signal
     *
     * Emitted once all enemies are defeated
     */
    void win();
    /*!
     * \brief Protagonist is dead signal
     *
     * Protagonist can only die when attacking an enemy,
     * or if he was deadly poisoned(level of poison became
     * more than health level) on the last move. That means
     * the hero still has one move to find a health pack even
     * if he was killed by the poison
     */
    void protagonistDead();
    /*!
     * \brief Protagonist ran out of energy signal
     *
     * When the protagonist runs out of energy, he can
     * no longer move. This means game over.
     */
    void protagonistNoEnergy();

public slots:
    /*!
     * \brief Attack an enemy at the position
     *
     * Attacks any enemy that occupies the given position
     */
    void attackEnemy();
    /*!
     * \brief Use a health pack at the position
     *
     * Uses any health pack that occupies the given position
     */
    void useHealthpack();
    /*!
     * \brief Spead the poison slot
     *
     * Decreases protagonist health if he is within
     * connected to each poisoned enemy
     * the poisoned area. This slot is automatically
     * \param value damage
     * \param rect poisoned area
     */
    void poisonArea(int value, QRect rect);
    /*!
     * \brief Move the protagonist to a certain position
     *
     * This is an overloaded function.
     *
     * \sa move(const QPos &pos)
     *
     * \param x vertical coordinate of a new position
     * \param y horizontal coordinate of a new position
     */
    void move(int x,int y);
    /*!
     * \brief Move the protagonist to a certain position
     *
     * \sa move(int x,int y)
     *
     * \param pos target position
     */
    void move(const QPoint &pos);
};

#endif // WORLDMODEL_H
