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

protected:
    WorldModel* model_;
    Path path_;
    QTimer animation_;

public slots:
    virtual void init()=0;

private slots:
    void animatePath();
};

#endif // WORLDABSTRACTCONTROLLER_H
