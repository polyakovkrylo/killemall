#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tv = findChild<WorldTerminalView*>("terminalView");
    gv = findChild<WorldGraphicsView*>("graphicsView");
    tv->hide();

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
    Maps map = Maps(vals.map);

    switch(map) {
    case maze1: filename = ":/img/level1.png"; break;
    case maze2: filename = ":/img/maze2.png"; break;
    case maze3: filename = ":/img/maze3.png"; break;
    case worldmap: filename = ":/img/worldmap4.png"; break;
    default: filename = ":/img/worldmap4.png"; break;
    }    
    //Destroy previous model?
    WorldModel *m = new WorldModel();
    m->init(filename, vals.enemies, vals.healthpacks);
    setModel(m);
    model->getController().get()->setOptimizationLevel(vals.optimization);
    tv->clearOutput();

}
