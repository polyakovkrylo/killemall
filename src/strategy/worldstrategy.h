#ifndef WORLDSTRATEGY_H
#define WORLDSTRATEGY_H

#include <QObject>

#include "model/worldmodel.h"
#include "controller/worldabstractcontroller.h"

class WorldStrategy : public QObject
{
    Q_OBJECT
public:
    explicit WorldStrategy(QObject *parent = 0);
    void setModel(WorldModel* model);

private:
    WorldModel* model_;

public slots:
    void run();
    void run(bool b);
    void stop();
    virtual void nextMove();

signals:
    void finished();
};

#endif // WORLDSTRATEGY_H
