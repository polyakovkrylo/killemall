#ifndef WORLDABSTRACTCONTROLLER_H
#define WORLDABSTRACTCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QTimer>

class WorldModel;

struct Path {
    double cost;
    QVector<QPoint> steps;
};

class WorldAbstractController : public QObject
{
    Q_OBJECT
public:
    explicit WorldAbstractController(WorldModel *model);
    void move(const QPoint &from, const QPoint& to);
    virtual bool findPath(const QPoint &from, const QPoint& to) = 0;
    inline const Path &currentPath() {return path_;}
    void setOptimizationLevel(int);

protected:
    WorldModel* model_;
    Path path_;

private slots:
    void animatePath();
    void setAnimationSpeed(int);

};

#endif // WORLDABSTRACTCONTROLLER_H
