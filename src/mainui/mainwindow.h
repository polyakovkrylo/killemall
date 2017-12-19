/*!
 * \file mainwindow.h
 *
 * MainWindow class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "terminalview/worldterminalview.h"
#include "graphicsview/worldgraphicsview.h"
#include "model/worldmodel.h"
#include "popup.h"

/*!
 * \defgroup mainui
 * \brief UI implementation
 */

namespace Ui {
class MainWindow;
}

/*!
  \ingroup
 * \brief Main window of the application
 *
 * The most part of implementation is done in Designer
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /*!
     * \brief Set model for both views
     * \param m model
     */
    void setModel(WorldModel *m);


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
