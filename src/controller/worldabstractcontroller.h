#ifndef WORLDABSTRACTCONTROLLER_H
#define WORLDABSTRACTCONTROLLER_H

#include <QObject>

class WorldModel;

class WorldAbstractController
{
public:
    explicit WorldAbstractController(WorldModel *model);
    virtual void findPath(int x,int y) = 0;

private:
    WorldModel* model_;
};

#endif // WORLDABSTRACTCONTROLLER_H
