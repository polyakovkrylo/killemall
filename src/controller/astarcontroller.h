#ifndef ASTARCONTROLLER_H
#define ASTARCONTROLLER_H

#include "worldabstractcontroller.h"

class AStarController : public WorldAbstractController
{
public:
    AStarController(WorldModel* model);
    virtual bool findPath(const QPoint &from, const QPoint &to) override;
};

#endif // ASTARCONTROLLER_H
