#include "worldgraphicsview.h"

WorldGraphicsView::WorldGraphicsView(QWidget *parent) :
    QGraphicsView(parent), scene_{nullptr}
{
}

void WorldGraphicsView::setModel(const WorldModel *model)
{
    //reset scene
    if(scene_ != nullptr) {
        setScene(nullptr);
        scene_->deleteLater();
    }

    //create scene and draw background
    QImage back(model->getLevel());
    scene_ = new QGraphicsScene(QRectF(0,0,back.width(),back.height()),this);
    scene_->setBackgroundBrush(back);

    //draw enemies with the center at tile's x and y
    for(auto &e: model->getEnemies()) {
        scene_->addEllipse(QRectF(e->getXPos()-itemSize/2,e->getYPos()-itemSize/2,
                                  itemSize,itemSize),
                           QPen(), QBrush(Qt::red));
    }

    //draw health packs with the center at tile's x and y
    for(auto &p: model->getHealthpacks()) {
        scene_->addEllipse(QRectF(p->getXPos()-itemSize/2,p->getYPos()-itemSize/2,
                                  itemSize,itemSize),
                           QPen(), QBrush(Qt::green));
    }

    //draw protagonist with the center at tile's x and y
    auto &p = model->getProtagonist();
    protagonist_ = scene_->addEllipse(QRectF(p->getXPos()-itemSize/2,
                                             p->getYPos()-itemSize/2,
                                             itemSize,itemSize),
                                      QPen(), QBrush(Qt::blue));

    connect(p.get(), SIGNAL(posChanged(int,int)),
            this, SLOT(onProtagonistPositionChanged(int,int)));
    connect(model, SIGNAL(reload()),this, SLOT(onReload()));
    connect(model, SIGNAL(healthpackUsed(int,int)), this, SLOT(onHealthpackUsed(int,int)));
    connect(model, SIGNAL(enemyDefeated(int,int)), this, SLOT(onEnemyDefeated(int,int)));

    setScene(scene_);
    centerOn(protagonist_);
}

QGraphicsItem *WorldGraphicsView::itemAt(QRect itemRect)
{
    //looking for an item with that fits the rectangle
    QGraphicsItem* item = nullptr;
    for(QGraphicsItem* it: items(itemRect.x(),item->y())){
        if(it == protagonist_) {
            continue;
        }
        if(it->boundingRect() == itemRect) {
            item = it;
            break;
        }
    }
    return item;
}

void WorldGraphicsView::onProtagonistPositionChanged(int x, int y)
{
    //move protagonist and center view
    protagonist_->setPos(x,y);
    centerOn(protagonist_);
}

void WorldGraphicsView::onEnemyDefeated(int x, int y)
{
    //find an enemy with the center at the given position and paint it gray
    QRect itemRect = QRect(x-itemSize/2, y-itemSize/2,itemSize,itemSize);
    qgraphicsitem_cast<QGraphicsEllipseItem*>(itemAt(itemRect))->setBrush(Qt::gray);
}

void WorldGraphicsView::onHealthpackUsed(int x, int y)
{
    //find an enemy with the center at the given position and remove it
    QRect itemRect = QRect(x-itemSize/2, y-itemSize/2,itemSize,itemSize);
    scene_->removeItem(itemAt(itemRect));
}

void WorldGraphicsView::onReload()
{
    //reset the scene
    setModel(qobject_cast<WorldModel*>(sender()));
}
