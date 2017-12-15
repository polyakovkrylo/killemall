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

void WorldTerminalView::setModel(const WorldModel *m)
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
    output->setTextColor(Qt::darkGray);
    output->setTextColor(Qt::black);
    if(cmd == "help") help(args.value(0));
    else output->append("Invalid cmd, type 'help' to see all commands");

}

void WorldTerminalView::help(QString command)
{
    QString msg;
    if(command.isEmpty()){
        msg = QString("List of commands (type 'help [command]' to get more information):\n\n"
                      "find <object> [ value ] \t Find enemy or health pack\n"
                      "status <object> \t Get information about health packs, enemies and protagonist\n"
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
    else if(command == "status") {
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
