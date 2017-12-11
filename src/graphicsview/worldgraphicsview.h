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

    virtual void keyPressEvent(QKeyEvent *e) override;

    virtual void mousePressEvent(QMouseEvent *e) override;

    virtual void mouseMoveEvent(QMouseEvent *e) override;

private:
    WorldModel* model_;
    QGraphicsScene* scene_;
    QGraphicsEllipseItem* protagonist_;
    QProgressBar* healthBar_;
    QProgressBar* energyBar_;

public slots:
    void onProtagonistPositionChanged(int x, int y);
    void onReload();

};

#endif // WORLDGRAPHICSVIEW_H
