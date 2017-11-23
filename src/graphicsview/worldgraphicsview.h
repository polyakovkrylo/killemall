#ifndef WORLDGRAPHICSVIEW_H
#define WORLDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QProgressBar>
#include <QLabel>

#include "model/worldmodel.h"

class WorldGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    WorldGraphicsView(QWidget *parent = 0);

    void setModel(const WorldModel *model);
    QGraphicsItem *itemAt(QRect itemRect);

    const int itemSize{10};

private:
    QGraphicsScene* scene_;
    QGraphicsEllipseItem* protagonist_;
    QProgressBar* healthBar_;
    QProgressBar* energyBar_;

public slots:
    void onProtagonistPositionChanged(int x, int y);
    void onEnemyDefeated(int x,int y);
    void onHealthpackUsed(int x,int y);
    void onReload();

};

#endif // WORLDGRAPHICSVIEW_H
