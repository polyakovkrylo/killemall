/*!
 * \file worldterminalview.cpp
 *
 * WorldTerminalView class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "worldterminalview.h"

WorldTerminalView::WorldTerminalView(QWidget *parent) :
    QWidget(parent), model{nullptr}
{        
    output = new QTextEdit;
    output->setReadOnly(true);

    QLabel* label = new QLabel("&Command:");

    cmdLine = new QLineEdit("");
    label->setBuddy(cmdLine);
    cmdLine->setFocusPolicy(Qt::StrongFocus);

    QPushButton* enterBtn = new QPushButton("&Enter");

    connect(cmdLine, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
    connect(enterBtn, SIGNAL(clicked()), this, SLOT(onReturnPressed()));

    //Layout setup
    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(label);
    phbxLayout->addWidget(cmdLine);
    phbxLayout->addWidget(enterBtn);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(output);
    pvbxLayout->addLayout(phbxLayout);
    setLayout(pvbxLayout);
    cmdLine->setFocus();
}

void WorldTerminalView::setModel(WorldModel *m)
{
    // disconnect old model's signals
    if(model) {
        disconnect(model,SIGNAL(reload()),this,SLOT(reloadView()));
        disconnect(model, SIGNAL(enemyDefeated(int,int)), this, SLOT(onEnemyDefeated(int,int)));
        disconnect(model, SIGNAL(healthpackUsed(int,int)), this, SLOT(onHealthpackUsed(int,int)));
        disconnect(model, SIGNAL(areaPoisoned(int,QRect)), this, SLOT(onAreaPosioned(int,QRect)));
        disconnect(model->getProtagonist().get(), SIGNAL(healthLevelChanged(int)), this, SLOT(onHealthLevelChanged(int)));
        disconnect(model->getProtagonist().get(), SIGNAL(posChanged(int,int)), this, SLOT(onPositionChanged(int,int)));
    }
    model = m;
    connect(model, SIGNAL(enemyDefeated(int,int)), this, SLOT(onEnemyDefeated(int,int)));
    connect(model, SIGNAL(healthpackUsed(int,int)), this, SLOT(onHealthpackUsed(int,int)));
    connect(model, SIGNAL(areaPoisoned(int,QRect)), this, SLOT(onAreaPosioned(int,QRect)));
    if(model->ready())
        reloadView();
    connect(model,SIGNAL(reload()),this,SLOT(reloadView()));
}

void WorldTerminalView::onReturnPressed()
{
    message = cmdLine->text();
    // if no command was specified, print help
    if(message.isEmpty()) {
        help();
    }
    else {
        // echo input
        output->setTextColor(Qt::darkGray);
        output->append("\n>" + message + "\n");
        // split the command and its arguments and execute it
        QStringList list = message.split(' ', QString::SkipEmptyParts);
        QString cmd = list.takeFirst();
        executeCmd(cmd,list);
        cmdLine->clear();
    }
}

void WorldTerminalView::onEnemyDefeated(int x, int y)
{
    output->setTextColor(QColor(255,0,0));
    message = QString("Enemy defeated at (" +
                      QString::number(x) + "," +
                      QString::number(y) +
                      ")");
    output->append(message);
}

void WorldTerminalView::onAreaPosioned(int value, QRect rect)
{
    output->setTextColor(Qt::darkYellow);
    message = QString("Area ((%1,%2),(%3,%4)) is poisoned with %5 value")
            .arg(rect.topLeft().x()).arg(rect.topLeft().y())
            .arg(rect.bottomLeft().x()).arg(rect.bottomLeft().y())
            .arg(value);
    output->append(message);
}

void WorldTerminalView::onHealthpackUsed(int x, int y)
{
    output->setTextColor(QColor(255,0,0));
    message = QString("Healthpack picked up at (" +
                      QString::number(x) + "," +
                      QString::number(y) +
                      ")");
    output->append(message);
}

void WorldTerminalView::onHealthLevelChanged(int value)
{
    output->setTextColor(QColor(0,155,0));
    message = QString("Health updated: %1 HP").arg(value);
    output->append(message);
}

void WorldTerminalView::onPositionChanged(int x, int y)
{
    // clear the previous line if it's about the protagonist position
    if(message.contains("Protagonist position changed:")) {
        output->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        output->moveCursor(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        output->textCursor().removeSelectedText();
        output->textCursor().deletePreviousChar();
    }
    output->setTextColor(QColor(0,0,255));
    message = QString("Protagonist position changed: (%1,%2)").arg(x).arg(y);
    output->append(message);
}

void WorldTerminalView::reloadView()
{
    output->clear();
    connect(model->getProtagonist().get(), SIGNAL(healthLevelChanged(int)), this, SLOT(onHealthLevelChanged(int)));
    connect(model->getProtagonist().get(), SIGNAL(posChanged(int,int)), this, SLOT(onPositionChanged(int,int)));
}

void WorldTerminalView::executeCmd(QString &cmd, QStringList args)
{    
    output->setTextColor(Qt::black);
    if(cmd == "help") help(args.value(0));
    else if(cmd == "find") find(args.value(0),args.value(1).toFloat());
    else if(cmd == "info") printInfo(args.value(0));
    else if(cmd == "move") model->move(args.value(0).toInt(),args.value(1).toInt());
    else {
        message = QString("Invalid cmd, type 'help' to see all commands");
        output->append(message);
    }

}

void WorldTerminalView::help(QString command)
{
    if(command.isEmpty()){
        message = QString("List of commands (type 'help [command]' to get more information):\n\n"
                      "find <object> [ value ] \t Find enemy or health pack\n"
                      "info [ object ] \t Get information about health packs, enemies and protagonist\n"
                      "move <x> <y> \t Move protagonist to the position");
    }
    else if(command == "find") {
        message = QString("Search for the closest enemy or health pack:\n\n"
                      "Syntax: find <object> [ value ]\n\n"
                      "Option value is minimal(maximal in case of enemy) value of the object\n\n"
                      "Objects:\n"
                      "e    Any enemy\n"
                      "re   Regular enemy\n"
                      "pe   Posioned enemy\n"
                      "h    Health pack"
                      );
    }
    else if(command == "info") {
        message = QString("Print information about the object(objects):\n\n"
                      "Syntax: info [ object ] \n\n"
                      "Objects:\n"
                      "p    Protagonist\n"
                      "e    All enemies\n"
                      "re   Regular enemies\n"
                      "pe   Posioned enemies\n"
                      "h    Health packs"
                      );
    }
    else if(command == "move") {
        message = QString("Move protagonist to the position:\n\n"
                      "Syntax: move <x> <y> \n\n"
                      "Options:\n"
                      "x    Horizontal position\n"
                      "y    Vertical position\n"
                      );
    }
    output->setTextColor(Qt::black);
    output->append(message);
}

void WorldTerminalView::find(QString object, float value)
{
    Tile* t = nullptr;
    QString obj;
    QString param;

    // if looking for an enemy
    if(object.endsWith("e")) {
        // set strength to max if it was not set
        param = QString("strength");
        if(value == 0)
            value = 100.0f;
        if(object == "e"){
            obj = "enemy";
            t = model->getController()->findClosest(AnyEnemy,0.0f,value);
        } else if(object == "re"){
            obj = QString("regular enemy");
            t = model->getController()->findClosest(RegularEnemy,0.0f,value);
        } else if(object == "pe"){
            obj = QString("poisoned enemy");
            t = model->getController()->findClosest(PoisonedEnemy,0.0f,value);
        }
    }
    //if looking for a health pack
    else if(object == "h") {
        obj = QString("health pack");
        param = QString("HP");
        t = model->getController()->findClosest(HealthPack,value,100.0f);
    }

    // print info if an object was find
    if(t) {
        message = QString("Closest %1 with %2 %3 at position (%4,%5)")
                .arg(obj).arg(param).arg(t->getValue()).arg(t->getXPos()).arg(t->getYPos());
    }
    else {
        message = QString("Could not find a suitible object");
    }

    output->setTextColor(Qt::black);
    output->append(message);
}

void WorldTerminalView::printInfo(QString object)
{
    if(object == "p"){
        auto &p = model->getProtagonist();
        message = QString("Protagonist is at (%1,%2)\n"
                      "Health level: %3\n"
                      "Energy level: %4")
                .arg(p->getXPos()).arg(p->getYPos())
                .arg(p->getHealth()).arg(p->getEnergy());
    }
    else if(object == "e") {
        message = QString("Number of enemies alive: %1")
                .arg(model->getEnemies().size() + model->getPEnemies().size());
    }
    else if(object == "pe") {
        message = QString("Number of poisoned enemies alive: %1").arg(model->getPEnemies().size());
    }
    else if(object == "re") {
        message = QString("Number of regular enemies alive: %1").arg(model->getEnemies().size());
    }
    else if(object == "h") {
        message = QString("Number of health packs left: %1").arg(model->getHealthpacks().size());
    }
    else {
        printInfo("p");
        printInfo("e");
        printInfo("h");
    }
    output->setTextColor(Qt::black);
    output->append(message);
}
