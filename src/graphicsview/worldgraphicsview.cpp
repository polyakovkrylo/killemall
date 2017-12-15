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
    // disconnect previous model's signals
    if(model_) {
        disconnect(model_, SIGNAL(reload()),this, SLOT(reloadScene()));
        disconnect(model_->getProtagonist().get(), SIGNAL(posChanged(int,int)), this, SLOT(onProtagonistPositionChanged(int,int)));
        disconnect(model_->getProtagonist().get(),SIGNAL(healthLevelChanged(int)),healthBar_,SLOT(setValue(int)));
        disconnect(model_->getProtagonist().get(),SIGNAL(energyLevelChanged(int)),energyBar_,SLOT(setValue(int)));
    }
    model_ = model;
    reloadScene();
    // connect new model reload signal
    connect(model_, SIGNAL(reload()),this, SLOT(reloadScene()));
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
}
