#include "worldabstractcontroller.h"
#include "model/worldmodel.h"

WorldAbstractController::WorldAbstractController(WorldModel *model) :
    QObject(model), model_{model}, path_{0,QVector<QPoint>()}
{
    animation_.setSingleShot(true);
    animation_.setInterval(10);
    connect(&animation_,SIGNAL(timeout()),SLOT(animatePath()));
}

void WorldAbstractController::move(const QPoint &from, const QPoint &to)
{
    bool scs = findPath(from, to);
    if(scs)
        animatePath();
}

void WorldAbstractController::animatePath()
{
    // move protagonist along the path till the path is done
    if(!path_.steps.empty()) {
        QPoint pos(path_.steps.takeFirst());
        model_->getProtagonist()->setPos(pos.x(),pos.y());
        animation_.start();
    }
}
