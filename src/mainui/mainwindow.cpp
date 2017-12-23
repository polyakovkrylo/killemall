/*!
 * \file mainwindow.cpp
 *
 * MainWindow class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model{nullptr}
{
    ui->setupUi(this);
    tv = findChild<WorldTerminalView*>("terminalView");
    gv = findChild<WorldGraphicsView*>("graphicsView");
    tv->hide();

    setModel(new WorldModel());

    loadWorld = new Popup;
    connect(ui->actionLoad_world, SIGNAL(triggered(bool)), loadWorld, SLOT(exec()));
    connect(loadWorld, SIGNAL(accepted()), this, SLOT(onPopupClosed()));

    emit ui->actionLoad_world->triggered(true);
}

void MainWindow::setModel(WorldModel *m)
{
    model = m;
    tv->setModel(model);
    gv->setModel(model);
    connect(ui->speedSlider, SIGNAL(sliderMoved(int)), model->getController().get(), SLOT(setAnimationSpeed(int)));
    connect(model, &WorldModel::protagonistDead, this, [=](){
        QMessageBox msg;
        msg.setText("Protagonist is dead!\nGAME OVER");
        msg.setStandardButtons(QMessageBox::Close | QMessageBox::Retry);
        int ret = msg.exec();
        switch(ret) {
        case QMessageBox::Close: QCoreApplication::exit();
            break;
        case QMessageBox::Retry: loadWorld->exec();
        }
    });
    connect(model, &WorldModel::protagonistNoEnergy, this, [=](){
        QMessageBox msg;
        msg.setText("Protagonist ran out of energy!\nGAME OVER");
        msg.setStandardButtons(QMessageBox::Close | QMessageBox::Retry);
        int ret = msg.exec();
        switch(ret) {
        case QMessageBox::Close: QCoreApplication::exit();
            break;
        case QMessageBox::Retry: loadWorld->exec();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_switchViewBtn_clicked()
{
    if(gv->isHidden()) {
        tv->hide();
        gv->show();
    }
    else {
        gv->hide();
        tv->show();
    }
}

void MainWindow::onPopupClosed()
{
    Values vals = loadWorld->getValues();
    QString map(":/img/" + vals.map);
    model->init(map, vals.enemies, vals.healthpacks);
    model->getController().get()->setOptimizationLevel(vals.optimization);
}
