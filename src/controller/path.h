#ifndef PATH_H
#define PATH_H

#include <memory>
#include <QPoint>

class Path
{
public:
    Path();
    const std::vector<QPoint> &getSteps();
    double getCost();

private:
    std::vector<QPoint> steps_;
    double cost_;
};

#endif // PATH_H
