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
