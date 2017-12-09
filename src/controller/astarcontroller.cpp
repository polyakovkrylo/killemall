#include "astarcontroller.h"
#include "model/worldmodel.h"

AStarController::AStarController(WorldModel *model) :
    WorldAbstractController(model)
{

}

bool AStarController::findPath(const QPoint &from, const QPoint &to)
{
    Q_UNUSED(from);
    int targetValue=model_->getLevel().pixelColor(to).lightness();

    bool scs = false;

    if(targetValue>0)
    {
        // AStar implementation
        // Dummy implementation for testing only
        path_.steps.clear();
        path_.steps.push_back(to);
        scs = true;
    }

    return scs;
}
