#include "worldgraphicsview.h"

WorldGraphicsView::WorldGraphicsView(QWidget *parent) :
    QGraphicsView(parent), model_{nullptr}, scene_{nullptr}
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

void WorldGraphicsView::setModel(WorldModel *model)
{
    model_ = model;
    //reset scene
    if(scene_ != nullptr) {
        scene_->deleteLater();
    }

    //create scene and draw background
    QImage back(model->getLevel());
    scene_ = new QGraphicsScene(QRectF(0,0,back.width(),back.height()),this);
    scene_->setBackgroundBrush(back);

    //draw enemies with the center at tile's x and y
    for(auto &e: model->getEnemies()) {
        // draw poison area if the enemy is of type PEnemy
        Enemy* en = e.get();
        PEnemy* pe= dynamic_cast<PEnemy*>(en);
        if(pe != nullptr){
            QGraphicsEllipseItem *it = scene_->addEllipse(QRectF(e->getXPos()-itemSize_*1.5,
                                                                 e->getYPos()-itemSize_*1.5,
                                                                 itemSize_*3,itemSize_*3),
                                                          QPen(Qt::transparent),QBrush());
            connect(pe, &PEnemy::poisonLevelUpdated, [=](int value) {
                // Set alpha channel as doubled poison level
                it->setBrush(QColor(230,230,0,value*2));
             } );
        }

        //draw enemy with the center at tile's x and y
        scene_->addEllipse(QRectF(e->getXPos()-itemSize_/2,e->getYPos()-itemSize_/2,
                                  itemSize_,itemSize_),
                           QPen(), QBrush(Qt::red));
    }

    //draw health packs with the center at tile's x and y
    for(auto &p: model->getHealthpacks()) {
        scene_->addEllipse(QRectF(p->getXPos()-itemSize_/2,p->getYPos()-itemSize_/2,
                                  itemSize_,itemSize_),
                           QPen(), QBrush(Qt::green));
    }

    //draw protagonist with the center at tile's x and y
    auto &p = model->getProtagonist();
    protagonist_ = scene_->addEllipse(QRectF(p->getXPos()-itemSize_/2,
                                             p->getYPos()-itemSize_/2,
                                             itemSize_,itemSize_),
                                      QPen(), QBrush(Qt::blue));

    healthBar_->setValue(p->getHealth());
    energyBar_->setValue(p->getEnergy());

    connect(p.get(), SIGNAL(posChanged(int,int)),
            this, SLOT(onProtagonistPositionChanged(int,int)));
    connect(model, SIGNAL(reload()),this, SLOT(onReload()));
    connect(model, SIGNAL(healthpackUsed(int,int)), this, SLOT(onHealthpackUsed(int,int)));
    connect(model, SIGNAL(enemyDefeated(int,int)), this, SLOT(onEnemyDefeated(int,int)));
    connect(model,SIGNAL(healthLevelChanged(int)),healthBar_,SLOT(setValue(int)));
    connect(model,SIGNAL(energyLevelChanged(int)),energyBar_,SLOT(setValue(int)));

    setScene(scene_);
    centerOn(protagonist_);
}

QGraphicsItem *WorldGraphicsView::itemAt(QPoint itemCenter)
{
    //looking for an item with that fits the rectangle
    QGraphicsItem* item = nullptr;
    for(QGraphicsItem* it: items(itemCenter)){
        if(it == protagonist_) {
            continue;
        }
        if(it->boundingRect().center() == itemCenter) {
            item = it;
            break;
        }
    }
    return item;
}

void WorldGraphicsView::keyPressEvent(QKeyEvent *e)
{
    int x = model_->getProtagonist()->getXPos();
    int y = model_->getProtagonist()->getYPos();
    switch(e->key()) {
    case Qt::Key_Up:
        y--;
        break;
    case Qt::Key_Down:
        y++;
        break;
    case Qt::Key_Right:
        x++;
        break;
    case Qt::Key_Left:
        x--;
        break;
    }
    model_->move(x,y);
}

void WorldGraphicsView::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        model_->move(mapToScene(e->pos()).toPoint());
    }
}

void WorldGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    mousePressEvent(e);
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
    qgraphicsitem_cast<QGraphicsEllipseItem*>(itemAt(QPoint(x,y)))->setBrush(Qt::gray);
}

void WorldGraphicsView::onHealthpackUsed(int x, int y)
{
    //find an enemy with the center at the given position and remove it
    scene_->removeItem(itemAt(QPoint(x,y)));
}

void WorldGraphicsView::onReload()
{
    //reset the scene
    setModel(qobject_cast<WorldModel*>(sender()));
}
