#ifndef ASTARCONTROLLER_H
#define ASTARCONTROLLER_H

#include "worldabstractcontroller.h"

class AStarController : public WorldAbstractController
{
public:
    AStarController(WorldModel* model);
    virtual void findPath(int x,int y);
};

#endif // ASTARCONTROLLER_H
