#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "terminalview/worldterminalview.h"
#include "graphicsview/worldgraphicsview.h"
#include "model/worldmodel.h"
#include "popup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setModel(WorldModel *m);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    Popup *loadWorld;
    WorldModel *model;
    WorldTerminalView *tv;
    WorldGraphicsView *gv;

private slots:
    void on_switchViewBtn_clicked();
    void onPopupClosed();
};

#endif // MAINWINDOW_H
