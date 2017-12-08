#include "worldabstractcontroller.h"
#include "model/worldmodel.h"

WorldAbstractController::WorldAbstractController(WorldModel *model) :
    QObject(model), model_{model}, path_{0,QVector<QPoint>()}
{

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
        QTimer::singleShot(10, this, SLOT(animatePath()));
    }
}
