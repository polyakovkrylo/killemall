/*!
 * \file uworld.h
 *
 * libworld-update classes declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef UWORLD_H
#define UWORLD_H

#include <cmath>

#include "world.h"

/*!
 * \defgroup libworld-update
 *
 * \brief Update of libworld
 * Evolves classes given in libworld by
 * adding new functionality and members
 */

/*!
 * \ingroup libworld-update
 * \brief Updated health pack class
 */
class UHealthPack : public QObject, public Tile
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param x horizontal position
     * \param y vertical position
     * \param healthPoints HP to be restored
     * \param radius radius of occupied area in pixels
     */
    UHealthPack(int x, int y, float healthPoints, int radius = 5);
    /*!
     * \brief Use healthpack
     *
     * Emits the signal that the health pack is used and returns
     * HP to be restored
     * \return HP to be restored
     */
    float use();
    /*!
     * \brief Returns area occupied by the object
     */
    inline const QRect &area() const{return area_;}

private:
    QRect area_;

signals:
    void used();
};

/*!
 * \ingroup libworld-update
 * \brief Updated enemy class
 */
class UEnemy : public QObject, public Enemy
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param x horizontal position
     * \param y vertical position
     * \param strength damage to be caused
     * \param radius radius of occupied area in pixels
     */
    explicit UEnemy(int xPosition, int yPosition, float strength, int radius = 5);
    /*!
     * \brief Attack the enemy
     *
     * Emits dead signal and returns the damage to be caused
     * \return damage to be caused
     */
    float attack();
    /*!
     * \brief Returns area occupied by the object
     */
    inline const QRect &area() const {return area_;}

private:
    QRect area_;

signals:
    /*!
     * \brief Emited once enemy is defeated
     */
    void dead();
};

/*!
 * \ingroup libworld-update
 * \brief Updated poisoned enemy class
 */
class UPEnemy : public PEnemy
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param x horizontal position
     * \param y vertical position
     * \param strength damage to be caused
     * \param radius radius of occupied area in pixels
     */
    UPEnemy(int x, int y, float strength,
            int radius = 5, int poisonRadius = 10);
    /*!
     * \brief Returns area occupied by the object
     */
    inline const QRect &area() {return area_;}
    /*!
     * \brief Returns poison area of the enemy
     */
    inline const QRect &poisonArea() {return poisonArea_;}
    void attack();
private:
    QRect area_;
    QRect poisonArea_;
signals:
    /*!
     * \brief Poison the area signal
     * \param value poison level
     * \param area poisoned area
     */
    void areaPoisoned(int value, QRect area);
};

/*!
 * \ingroup libworld-update
 * \brief Updated protagonist class
 */
class UProtagonist : public Protagonist
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param radius radius occupied by the object
     */
    UProtagonist(int radius = 5);
    /*!
     * \brief Update health
     * \sa healthLevelChanged
     * \param diff HP to be added
     */
    void updateHealth(float diff);
    /*!
     * \brief Update energy
     * \sa energyLevelChanged
     * \param diff EP to be added
     */
    void updateEnergy(float diff);
    /*!
     * \brief Returns area occupied by the object
     */
    inline const QRect &area() const {return area_;}

private:
    QRect area_;

signals:
    /*!
     * \brief Health changed signal
     * \sa updateHealth
     * \param value current health level
     */
    void healthLevelChanged(int value);
    /*!
     * \brief Energy changed signal
     * \sa updateEnergy
     * \param value current energy level
     */
    void energyLevelChanged(int value);

public slots:
    void restoreEnergy();
};

/*!
 * \ingroup libworld-update
 * \brief Updated world class
 */
class UWorld
{
public:
    /*!
     * \brief Class constructor
     * \param filename path to map image
     */
    UWorld(QString filename);
    /*!
     * \brief Return map tiles
     */
    inline const std::vector<std::unique_ptr<Tile>> &getMap() const {return map_;}
    /*!
     * \brief Return number of columns
     */
    inline int getCols() const {return world_.getCols();}
    /*!
     * \brief Return number of rows
     */
    inline int getRows() const {return world_.getRows();}
    /*!
     * \brief Generate enemies
     *
     * Generate enemies using World::getEnemies
     * \param enemies number of enemies
     * \return vector of unique pointers to enemies
     */
    std::vector<std::unique_ptr<Enemy>> createEnemies(unsigned int enemies);
    /*!
     * \brief Generate health packs
     *
     * Generate packs using World::getHealthPacks
     * \param packs number of packs
     * \return vector of unique pointers to health packs
     */
    std::vector<std::unique_ptr<UHealthPack> > createHealthpacks(unsigned int packs);
    /*!
     * \brief Create protagonist
     *
     * Create protagonist and moves it to a first available
     * point in space
     * \return unique pointer to protagonist
     */
    std::unique_ptr<UProtagonist> createProtagonist();

private:
    World world_;
    std::vector<std::unique_ptr<Tile>> map_;
};

#endif // UWORLD_H
