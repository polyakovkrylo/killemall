#include "astarcontroller.h"
#include "model/worldmodel.h"

AStarController::AStarController(WorldModel *model) :
    WorldAbstractController(model)
{

}

Path AStarController::findPath(int x, int y)
{
    QPoint protPlace(model_->getProtagonist()->getXPos(),
                     model_->getProtagonist()->getYPos());
    QPoint targetPlace(x,y);

    int targetValue=model_->getLevel().pixelColor(x,y).lightness();

    Path p;
    if(targetValue>0)
    {
        p = aStarAlgorithm(protPlace,targetPlace);
    }
    else
    {
        p = Path();
    }

    return p;
}

Path AStarController::aStarAlgorithm(QPoint start, QPoint target)
{
    return Path();
}
