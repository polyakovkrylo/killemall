#include <QApplication>
#include "model/worldmodel.h"
#include "graphicsview/worldgraphicsview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WorldModel m;
    m.init(":/img/level1.png");

    WorldGraphicsView v;
    v.resize(300,300);
    v.setModel(&m);

    v.show();

    m.getProtagonist()->setPos(200,200);

    return a.exec();
}
