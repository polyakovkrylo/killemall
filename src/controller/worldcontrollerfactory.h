#ifndef WORLDCONTROLLERFACTORY_H
#define WORLDCONTROLLERFACTORY_H

#include "astarcontroller.h"

class WorldModel;

enum ControllerType {
    AStar
};


class WorldControllerFactory
{
public:
    WorldControllerFactory();
    static WorldAbstractController* createController(WorldModel* model, ControllerType type = AStar);
};

#endif // WORLDCONTROLLERFACTORY_H
