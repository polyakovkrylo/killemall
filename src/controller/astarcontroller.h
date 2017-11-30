#ifndef ASTARCONTROLLER_H
#define ASTARCONTROLLER_H

#include "worldabstractcontroller.h"

class AStarController : public WorldAbstractController
{
public:
    AStarController(WorldModel* model);
    virtual Path findPath(int x,int y) override;

private:
    Path aStarAlgorithm(QPoint start, QPoint target);
};

#endif // ASTARCONTROLLER_H
