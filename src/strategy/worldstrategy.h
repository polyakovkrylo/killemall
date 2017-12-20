#ifndef WORLDSTRATEGY_H
#define WORLDSTRATEGY_H

#include <QObject>

#include "model/worldmodel.h"

class WorldStrategy : public QObject
{
    Q_OBJECT
public:
    explicit WorldStrategy(QObject *parent = 0);
    void setModel(WorldModel* model);

private:
    WorldModel* model_;

public slots:
    virtual void run();
    virtual void stop();

signals:
    void finished();
};

#endif // WORLDSTRATEGY_H
