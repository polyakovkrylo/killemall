/*!
 * \file worldscene.cpp
 *
 * WorldGraphicsScene class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "worldgraphicsscene.h"

WorldGraphicsScene::WorldGraphicsScene(QImage back, QRectF rect, QObject* parent):
    QGraphicsScene(rect,parent), back_{back}
{

}

void WorldGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    painter->setBrushOrigin(0, 0);
    painter->drawImage(0,0,back_);
}
