#include <QApplication>
#include "model/worldmodel.h"
#include "graphicsview/worldgraphicsview.h"
#include "terminalview/worldterminalview.h"
#include "mainui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mw;
    mw.show();

    return a.exec();
}
