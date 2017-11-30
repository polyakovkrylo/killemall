#ifndef WORLDABSTRACTCONTROLLER_H
#define WORLDABSTRACTCONTROLLER_H

#include <QObject>

#include "path.h"

class WorldModel;

class WorldAbstractController
{
public:
    explicit WorldAbstractController(WorldModel *model);
    virtual Path findPath(int x,int y) = 0;

protected:
    WorldModel* model_;
};

#endif // WORLDABSTRACTCONTROLLER_H
