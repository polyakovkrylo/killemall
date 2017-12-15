#include "worldterminalview.h"

WorldTerminalView::WorldTerminalView(QWidget *parent) : QWidget(parent)
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
    output->clear();
    model = m;
    connect(model, SIGNAL(enemyDefeated(int,int)), this, SLOT(onEnemyDefeated(int,int)));
    connect(model, SIGNAL(healthpackUsed(int,int)), this, SLOT(onHealthpackUsed(int,int)));
    connect(model->getProtagonist().get(), SIGNAL(healthLevelChanged(int)), this, SLOT(onHealthLevelChanged(int)));
    connect(model->getProtagonist().get(), SIGNAL(energyLevelChanged(int)), this, SLOT(onEnergyLevelChanged(int)));
    connect(model->getProtagonist().get(), SIGNAL(posChanged(int,int)), this, SLOT(onPositionChanged(int,int)));

}

void WorldTerminalView::onReturnPressed()
{
    QString txt = cmdLine->text();
    output->setTextColor(Qt::darkGray);
    output->append("\n>" + txt + "\n");
    QStringList list = txt.split(' ', QString::SkipEmptyParts);
    QString cmd = list.takeFirst();
    executeCmd(cmd,list);
    cmdLine->clear();
}

void WorldTerminalView::onEnemyDefeated(int x, int y)
{
    output->setTextColor(QColor(255,0,0));
    output->append("Enemy defeated at (" +
                   QString::number(x) + "," +
                   QString::number(y) +
                   ")");
}

void WorldTerminalView::onHealthpackUsed(int x, int y)
{
    output->setTextColor(QColor(255,0,0));
    output->append("Healthpack picked up at (" +
                   QString::number(x) + "," +
                   QString::number(y) +
                   ")");
}

void WorldTerminalView::onHealthLevelChanged(int value)
{
    output->setTextColor(QColor(0,155,0));
    message = QString("Health updated: %1 HP").arg(value);
    output->append(message);
}

void WorldTerminalView::onEnergyLevelChanged(int value)
{
    output->setTextColor(QColor(155,155,0));
    message = QString("Energy updated: %1 EP").arg(value);
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

void WorldTerminalView::executeCmd(QString &cmd, QStringList args)
{    
    output->setTextColor(Qt::black);
    if(cmd == "help") help(args.value(0));
    else if(cmd == "find") find(args.value(0),args.value(1).toFloat());
    else if(cmd == "info") printInfo(args.value(0));
    else if(cmd == "move") model->move(args.value(0).toInt(),args.value(1).toInt());
    else output->append("Invalid cmd, type 'help' to see all commands");

}

void WorldTerminalView::help(QString command)
{
    QString msg;
    if(command.isEmpty()){
        msg = QString("List of commands (type 'help [command]' to get more information):\n\n"
                      "find <object> [ value ] \t Find enemy or health pack\n"
                      "info <object> \t Get information about health packs, enemies and protagonist\n"
                      "move <x> <y> \t Move protagonist to the position");
    }
    else if(command == "find") {
        msg = QString("Search for the closest enemy or health pack:\n\n"
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
        msg = QString("Print information about the object(objects):\n\n"
                      "Syntax: status <object> \n\n"
                      "Objects:\n"
                      "p    Protagonist\n"
                      "e    All enemies\n"
                      "re   Regular enemies\n"
                      "pe   Posioned enemies\n"
                      "h    Health packs"
                      );
    }
    else if(command == "move") {
        msg = QString("Move protagonist to the position:\n\n"
                      "Syntax: move <x> <y> \n\n"
                      "Options:\n"
                      "x    Horizontal position\n"
                      "y    Vertical position\n"
                      );
    }
    output->setTextColor(Qt::black);
    output->append(msg);
}

void WorldTerminalView::find(QString object, float value)
{
    QString msg;
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
            t = model->getController()->findClosest(AnyEnemy,0.0f,value).get();
        } else if(object == "re"){
            obj = QString("regular enemy");
            t = model->getController()->findClosest(RegularEnemy,0.0f,value).get();
        } else if(object == "pe"){
            obj = QString("poisoned enemy");
            t = model->getController()->findClosest(PoisonedEnemy,0.0f,value).get();
        }
    }
    //if looking for a health pack
    else if(object == "h") {
        obj = QString("health pack");
        param = QString("HP");
        t = model->getController()->findClosest(PoisonedEnemy,value,0.0f).get();
    }

    // print info if an object was find
    if(t) {
        msg = QString("Closest %1 with %2 %3 at position (%4,%5)")
                .arg(obj).arg(param).arg(t->getValue()).arg(t->getXPos()).arg(t->getYPos());
    }
    else {
        msg = QString("Could not find a suitible object");
    }

    output->setTextColor(Qt::black);
    output->append(msg);
}

void WorldTerminalView::printInfo(QString object)
{
    QString msg;
    if(object == "p"){
        auto &p = model->getProtagonist();
        msg = QString("Protagonist is at (%1,%2)\n"
                      "Health level: %3\n"
                      "Energy level: %4")
                .arg(p->getXPos()).arg(p->getYPos())
                .arg(p->getHealth()).arg(p->getEnergy());
    }
    else if(object == "e") {
        msg = QString("Number of enemies alive: %1")
                .arg(model->getEnemies().size() + model->getPEnemies().size());
    }
    else if(object == "pe") {
        msg = QString("Number of poisoned enemies alive: %1").arg(model->getPEnemies().size());
    }
    else if(object == "re") {
        msg = QString("Number of regular enemies alive: %1").arg(model->getEnemies().size());
    }
    else if(object == "h") {
        msg = QString("Number of health packs left: %1").arg(model->getHealthpacks().size());
    }
    else {
        printInfo("p");
        printInfo("e");
        printInfo("h");
    }
    output->setTextColor(Qt::black);
    output->append(msg);
}
