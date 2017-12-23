#ifndef WORLDGRAPHICSSCENE_H
#define WORLDGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QPainter>

class WorldGraphicsScene : public QGraphicsScene
{
public:
    WorldGraphicsScene(QImage back, QRectF rect, QObject* parent = nullptr);
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
private:
    QImage back_;
};

#endif // WORLDGRAPHICSSCENE_H
