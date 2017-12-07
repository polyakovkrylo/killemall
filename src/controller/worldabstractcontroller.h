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
#include <QObject>
#include <QVector>
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
    QVector<QPoint> steps;
};

/*!
 * \ingroup controller
 * \brief Abstract controller class
 *
 * Defines the basic functionality of the world
 * model controller. In order to implement a new
 * controller, this class should be inherited,
 * its type should be added to ControllerType,
 * and findPath should be re-implemented
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
    inline void setOptimizationLevel(float value){optimization_ = value/100.0f;}
    void stop();

protected:
    inline float calculateCost(float tile) {return !std::isinf(tile) ? (minCost_/tile) : 0;}
    /*!
     * \brief Owner of the controller
     */
    WorldModel* model_;
    /*!
     * \brief Last calculated path
     */
    Path path_;
    QTimer animation_;
    float optimization_;
    const float minCost_{0.1f};

public slots:
    virtual void init()=0;
    void setAnimationSpeed(int value);

private slots:
    void animatePath();

};

#endif // WORLDABSTRACTCONTROLLER_H
