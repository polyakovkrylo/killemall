#include "worldgraphicsview.h"

WorldGraphicsView::WorldGraphicsView(QWidget *parent) :
    QGraphicsView(parent), scene_{nullptr}
{
    healthBar_ = new QProgressBar(this);
    energyBar_ = new QProgressBar(this);
    QLabel *lh =  new QLabel("Health", this);
    QLabel *le =  new QLabel("Energy", this);

    healthBar_->setStyleSheet("QProgressBar::chunk{background-color:green;}");
    energyBar_->setStyleSheet("QProgressBar::chunk{background-color:yellow;}");
    healthBar_->setTextVisible(false);
    energyBar_->setTextVisible(false);

    lh->setAlignment(Qt::AlignCenter);
    le->setAlignment(Qt::AlignCenter);

    healthBar_->setGeometry(10,10,100,20);
    energyBar_->setGeometry(10,40,100,20);
    lh->setGeometry(healthBar_->geometry());
    le->setGeometry(energyBar_->geometry());
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
    connect(model,SIGNAL(healthLevelChanged(float)),healthBar_,SLOT(setValue(int)));
    connect(model,SIGNAL(energyLevelChanged(float)),energyBar_,SLOT(setValue(int)));

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
