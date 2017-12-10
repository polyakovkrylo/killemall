#ifndef WORLDABSTRACTCONTROLLER_H
#define WORLDABSTRACTCONTROLLER_H

#include <memory>
#include <QObject>
#include <QVector>
#include <QTimer>

class WorldModel;
class Tile;

enum ObjectType {
    HealthPack,
    RegularEnemy,
    PoisonedEnemy,
    AnyEnemy
};

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
    const std::shared_ptr<Tile> findClosest(ObjectType type, float minValue = 0.0f, float maxValue = 100.0f);
    virtual bool findPath(const QPoint &from, const QPoint& to, float maxCost = INFINITY) = 0;
    inline const Path &currentPath() {return path_;}

protected:
    WorldModel* model_;
    Path path_;
    QTimer animation_;
    const float costOffset_{0.01f};

public slots:
    virtual void init()=0;

private slots:
    void animatePath();
};

#endif // WORLDABSTRACTCONTROLLER_H
