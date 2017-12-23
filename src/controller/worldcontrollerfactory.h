/*!
 * \file worldcontrollerfactory.h
 *
 * WorldControllerFactory class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef WORLDCONTROLLERFACTORY_H
#define WORLDCONTROLLERFACTORY_H

#include "astarcontroller.h"

class WorldModel;

/*!
 * \ingroup controller
 * \brief The ControllerType enum
 *
 * Defines the type of controller, i.e
 * the path finding algorithm standing
 * behind the controller
 */
enum ControllerType {
    AStar
};

/*!
 * \ingroup controller
 * \brief Controller factory class
 *
 * This class is an implementation of factory pattern.
 * Factory is needed since different algorythms can be
 * used for path finding, thus it is sufficient to have
 * a unified interface, in this case WorldAbstractController
 */
class WorldControllerFactory
{
public:
    WorldControllerFactory();
    /*!
     * \brief Create a controller for the model
     *
     * \param model pointer to owner of the controller
     * \param type type of controller
     * \return created controller
     */
    static WorldAbstractController* createController(WorldModel* model, ControllerType type = AStar);
};

#endif // WORLDCONTROLLERFACTORY_H
