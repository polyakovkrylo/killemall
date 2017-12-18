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
    if(model_) {
        disconnect(model_, SIGNAL(reload()),this, SLOT(reloadScene()));
        disconnect(model_->getProtagonist().get(), SIGNAL(posChanged(int,int)), this, SLOT(onProtagonistPositionChanged(int,int)));
        disconnect(model_->getProtagonist().get(),SIGNAL(healthLevelChanged(int)),healthBar_,SLOT(setValue(int)));
        disconnect(model_->getProtagonist().get(),SIGNAL(energyLevelChanged(int)),energyBar_,SLOT(setValue(int)));
    }
    model_ = model;
    if(model_->ready())
        reloadScene();
    // connect new model reload signal
    connect(model_, SIGNAL(reload()),this, SLOT(reloadScene()));
}

QGraphicsItem *WorldGraphicsView::itemAt(QPoint itemCenter)
{
    //looking for an item with that fits the rectangle
    QGraphicsItem* item = nullptr;
    for(QGraphicsItem* it: items(mapFromScene(itemCenter))){
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
        model_->move(x,--y);
        break;
    case Qt::Key_Down:
        model_->move(x,++y);
        break;
    case Qt::Key_Right:
        model_->move(++x,y);
        break;
    case Qt::Key_Left:
        model_->move(--x,y);
        break;
    }
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

void WorldGraphicsView::reloadScene()
{
    // reset scene
    if(scene_ != nullptr)
        scene_->deleteLater();

    // create scene and draw background
    QImage back(model_->getLevel());
    scene_ = new QGraphicsScene(QRectF(0,0,back.width(),back.height()),this);
    scene_->setBackgroundBrush(back);

    // draw enemies and connect them to lambda slot
    for(auto &e: model_->getEnemies()) {
        QGraphicsEllipseItem *eIt = scene_->addEllipse(e->area(),QPen(),QBrush(Qt::red));
        connect(e.get(), &UEnemy::dead, [=]() {
            // mark enemy as defeated
            eIt->setBrush(Qt::gray);
        } );
    }

    // draw enemies and connect them to lambda slot for dead poisonLevelChanged slots
    for(auto &pe: model_->getPEnemies()) {
        QGraphicsEllipseItem *peIt = scene_->addEllipse(pe->area(),QPen(),QBrush(Qt::red));
        connect(pe.get(), &UPEnemy::dead, [=]() {
            // mark enemy as defeated
            peIt->setBrush(Qt::gray);
        } );
        // draw poison area
        QGraphicsEllipseItem *pIt = scene_->addEllipse(pe->poisonArea(),
                                                       QPen(Qt::transparent),QBrush());
        connect(pe.get(), &UPEnemy::poisonLevelUpdated, [=](int value) {
            // Set alpha channel as doubled poison level
            pIt->setBrush(QColor(230,230,0,value*2));
        } );
    }

    //draw health packs with the center at tile's x and y
    for(auto &p: model_->getHealthpacks()) {
        QGraphicsEllipseItem *it = scene_->addEllipse(p->area(), QPen(), QBrush(Qt::green));
        connect(p.get(), &UHealthPack::used, [=]() {
            // Delete used  health pack
            scene_->removeItem(it);
        } );
    }

    //draw protagonist with the center at tile's x and y
    auto &p = model_->getProtagonist();
    protagonist_ = scene_->addEllipse(p->area(), QPen(), QBrush(Qt::blue));

    healthBar_->setValue(p->getHealth());
    energyBar_->setValue(p->getEnergy());

    connect(p.get(), SIGNAL(posChanged(int,int)), this, SLOT(onProtagonistPositionChanged(int,int)));
    connect(p.get(),SIGNAL(healthLevelChanged(int)),healthBar_,SLOT(setValue(int)));
    connect(p.get(),SIGNAL(energyLevelChanged(int)),energyBar_,SLOT(setValue(int)));

    setScene(scene_);
    centerOn(protagonist_);
}
