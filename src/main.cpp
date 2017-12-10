#include <QApplication>
#include "model/worldmodel.h"
#include "graphicsview/worldgraphicsview.h"
#include "terminalview/worldterminalview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WorldModel m;
    m.init(":/img/level4.png");

    WorldGraphicsView v;
    v.resize(300,300);
    v.setModel(&m);

    v.show();

    WorldTerminalView t;
    t.resize(640, 480);
    t.setModel(&m);
    t.show();

    return a.exec();
}
