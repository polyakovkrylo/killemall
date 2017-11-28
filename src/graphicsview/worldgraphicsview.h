#ifndef WORLDGRAPHICSVIEW_H
#define WORLDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include <QLabel>
#include <QKeyEvent>

#include "model/worldmodel.h"

class WorldGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    WorldGraphicsView(QWidget *parent = 0);

    void setModel(WorldModel *model);

    QGraphicsItem *itemAt(QPoint itemCenter);

    virtual void keyPressEvent(QKeyEvent *e) override;

private:
    WorldModel* model_;
    QGraphicsScene* scene_;
    QGraphicsEllipseItem* protagonist_;
    QProgressBar* healthBar_;
    QProgressBar* energyBar_;
    const int itemSize_{10};

public slots:
    void onProtagonistPositionChanged(int x, int y);
    void onEnemyDefeated(int x,int y);
    void onHealthpackUsed(int x,int y);
    void onReload();

};

#endif // WORLDGRAPHICSVIEW_H
