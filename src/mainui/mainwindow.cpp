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
    strategy = new WorldStrategy(this);
    setModel(new WorldModel(this));

    loadWorld = new Popup;
    connect(ui->actionLoad_world, SIGNAL(triggered(bool)), loadWorld, SLOT(exec()));
    connect(loadWorld, SIGNAL(accepted()), this, SLOT(onPopupClosed()));
    connect(ui->enlargeBtn, SIGNAL(clicked()), gv, SLOT(enlarge()));
    connect(ui->shrinkBtn, SIGNAL(clicked()), gv, SLOT(shrink()));
    connect(ui->strategyBtn, SIGNAL(clicked(bool)), strategy, SLOT(run(bool)));
    connect(strategy, SIGNAL(finished()), ui->strategyBtn, SLOT(toggle()));

    emit ui->actionLoad_world->triggered(true);
}

void MainWindow::setModel(WorldModel *m)
{
    model = m;
    tv->setModel(model);
    gv->setModel(model);
    strategy->setModel(model);
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
    connect(model, &WorldModel::win, this, [=](){
        QMessageBox msg;
        msg.setText("YOU WON!");
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
    ui->strategyBtn->setChecked(false);
    Values vals = loadWorld->getValues();
    QString map(":/img/maps/" + vals.map);
    model->getController()->setMinCost(vals.energyCost);
    model->init(map, vals.enemies, vals.healthpacks);
    model->getController().get()->setOptimizationLevel(vals.optimization);
}
