/*!
 * \file worldcontrollerfactory.cpp
 *
 * WorldControllerFactory class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "worldcontrollerfactory.h"

WorldControllerFactory::WorldControllerFactory()
{

}

WorldAbstractController* WorldControllerFactory::createController(WorldModel *model, ControllerType type)
{
    WorldAbstractController* ctrl = nullptr;
    switch(type) {
    case AStar:
        ctrl = new AStarController(model);
        break;
    }
    return ctrl;
}
