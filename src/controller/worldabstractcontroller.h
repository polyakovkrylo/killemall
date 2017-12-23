/*!
 * \file worldabstractcontroller.h
 *
 * WorldAbstractController class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef WORLDABSTRACTCONTROLLER_H
#define WORLDABSTRACTCONTROLLER_H

#include <memory>
#include <cmath>
#include <QObject>
#include <QTimer>

class WorldModel;
class Tile;

/*!
 * \defgroup controller
 *
 * \brief Controller component implementation.
 *
 * The controller is only responsible for
 * path finding and animated movement of protagonist
 * in space. All other object interaction are
 * automatically binded in the model
 */

/*!
 * \brief Object type enumeration
 *
 * Defines the type of object to search for
 *
 * \sa findClosest
 */
enum ObjectType {
    HealthPack,
    RegularEnemy,
    PoisonedEnemy,
    AnyEnemy
};

/*!
 * \ingroup controller
 * \brief The Path struct
 *
 * Includes vector of steps to perform
 * and overall energy cost
 */
struct Path {
    double cost;
    std::vector<QPair<QPoint,float>> steps;
};

/*!
 * \ingroup controller
 * \brief Abstract controller class
 *
 * Defines the basic functionality of the world
 * model controller. In order to implement a new
 * controller, this class should be inherited,
 * its type should be added to ControllerType,
 * init and findPath should be re-implemented
 */
class WorldAbstractController : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param model owner of the controller
     */
    explicit WorldAbstractController(WorldModel *model);
    /*!
     * \brief Move protagonist to the position
     *
     * Finds the path to a given point and animates
     * the movement to it
     * \param from starting point
     * \param to destination
     */
    bool move(const QPoint &from, const QPoint& to);
    /*!
     * \brief Find the closest object of a given type
     *
     * Searches for the closest object, enemy or health pack,
     * with a given max and min values.
     *
     * \param type type of object
     * \param minValue minimal tile value(strength of the enemy or HP of health pack)
     * \param maxValue maximal tile value(strength of the enemy or HP of health pack)
     * \return Tile pointer to the object. If no object was found, returns nullptr
     *
     * \sa ObjectType
     */
    Tile* findClosest(ObjectType type, float minValue = 0.0f, float maxValue = 100.0f);
    /*!
     * \brief Find the path to the position
     * \note This is a virtual function to be implemented by successors
     * \param from starting point
     * \param to destination
     * \return true if the path was find, false otherwise
     */
    virtual bool findPath(const QPoint &from, const QPoint& to, float maxCost = INFINITY) = 0;
    /*!
     * \brief Returns last calculated path
     */
    inline const Path &currentPath() {return path_;}
    /*!
     * \brief Set level of optimization, e.g. h weight in A* controller
     * \param value level from 0 to 100
     */
    inline void setOptimizationLevel(float value){optimization_ = value/100.0f;}
    /*!
     * \brief Set minimal tile energy cost
     *
     * Defines the energy cost of white tile
     * \param value energy cost multiplied by 1000
     */
    virtual void setMinCost(float value);
    /*!
     * \brief Stop the animation and clear the last path
     */
    void stop();

protected:
    /*!
     * \brief Translate tile value to energy cost
     * \param tile tile value
     * \return energy cost
     */
    inline float calculateCost(float tile) {return !std::isinf(tile) ? (minCost_/tile) : 0;}
    /*!
     * \brief Owner of the controller
     */
    WorldModel* model_;
    /*!
     * \brief Last calculated path
     */
    Path path_;
    /*!
     * \brief Animation timer
     */
    QTimer animation_;
    /*!
     * \brief Optimization level
     */
    float optimization_;
    /*!
     * \brief Minimal energy cost of a tile (white tile energy cost)
     * \sa calculateCost
     */
    float minCost_{0.01f};

public slots:
    /*!
     * \brief Initialization of the controller
     * \note This is a virtual function to be implemented by successors
     */
    virtual void init()=0;
    /*!
     * \brief Set animation speed
     *
     * Set animation speed between 100(fastest) and 1(slowest)
     * \param value animation speed
     */
    void setAnimationSpeed(int value);

private slots:
    void animatePath();

signals:
    void animationDone();
};

#endif // WORLDABSTRACTCONTROLLER_H
