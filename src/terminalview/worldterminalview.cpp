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
    model = m;
    connect(model, SIGNAL(enemyDefeated(int,int)), this, SLOT(onEnemyDefeated(int,int)));
    connect(model, SIGNAL(healthpackUsed(int,int)), this, SLOT(onHealthpackUsed(int,int)));

}

std::vector<float> WorldTerminalView::findNearestEnemy()
{
    //for testing
    std::vector<float> enemy = {45, 39, 13.6};    
    return enemy;
}

std::vector<float> WorldTerminalView::findNearestHealth()
{
    //for testing
    std::vector<float> hp = {103, 8, 55.5};    
    return hp;
}

void WorldTerminalView::clearOutput()
{
    output->clear();
}

void WorldTerminalView::onReturnPressed()
{
    std::string strCommand = "";
    strCommand += cmdLine->text().toStdString();
    executeCmd(strCommand);
    cmdLine->clear();
}

void WorldTerminalView::onEnemyDefeated(int x, int y)
{
    output->setTextColor(QColor(255,0,0));
    output->append("Enemy defeated at (" +
                   QString::number(x) + "," +
                   QString::number(y) +
                   "), protagonist health: " +
                   QString::number(model->getProtagonist()->getHealth()));
}

void WorldTerminalView::onHealthpackUsed(int x, int y)
{
    output->setTextColor(QColor(255,0,0));
    output->append("Healthpack picked up at (" +
                   QString::number(x) + "," +
                   QString::number(y) +
                   "), protagonist health: " +
                   QString::number(model->getProtagonist()->getHealth()));
}

void WorldTerminalView::executeCmd(std::string &cmd)
{    
    output->setTextColor(QColor(0,0,0));
    output->append(">" + QString::fromStdString(cmd));
    output->setTextColor(QColor(255,0,0));
    if(cmd == "help") help();
    else if(cmd == "find protag") findProtag();
    else if(cmd == "status protag") statusProtag();
    else if(cmd.compare(0, 5, "move(", 5) == 0) move(cmd);
    else if(cmd == "find enemy") findEnemy();
    else if(cmd == "find healthpack") findHealth();
    else output->append("Invalid cmd, type 'help' to see all commands");

}

void WorldTerminalView::help()
{
    output->append("List of commands:\n"
                   "find protag\n"
                   "status protag\n"
                   "find enemy\n"
                   "find healthpack\n"
                   "move(x,y)");
}

void WorldTerminalView::findProtag()
{
    output->append("Protagonist is located at (" +
                   QString::number(model->getProtagonist()->getXPos()) + "," +
                   QString::number(model->getProtagonist()->getYPos()) + ")");
}

void WorldTerminalView::statusProtag()
{
    output->append("Health: " + QString::number(model->getProtagonist()->getHealth()) +
                   " Energy: " + QString::number(model->getProtagonist()->getEnergy()));
}

void WorldTerminalView::findHealth()
{
    std::vector<float> hp = findNearestHealth();
    output->append("Nearest healthpack found at (" +
                   QString::number(hp.at(0)) + "," +
                   QString::number(hp.at(1)) + "), " +
                   "with value " +
                   QString::number(hp.at(2)));
}

void WorldTerminalView::findEnemy()
{
    std::vector<float> enemy = findNearestEnemy();
    output->append("Nearest enemy found at (" +
                   QString::number(enemy.at(0)) + "," +
                   QString::number(enemy.at(1)) + "), " +
                   "with strength " +
                   QString::number(enemy.at(2)));
}

void WorldTerminalView::move(std::string &cmd)
{
    cmd.erase(0, 5);
    std::stringstream ss(cmd);
    std::vector<int> coords;
    int i;
    while (ss >> i)
    {
        coords.push_back(i);

        if (ss.peek() == ',' || ss.peek() == ')')
            ss.ignore();
    }
    if(coords.size() != 2) output->append("Invalid cmd, type 'help' to see all commands");
    else /*if(model->move(coords.at(0), coords.at(1)))*/ output->append("Moved to (" +
                                                                        QString::number(coords.at(0)) + "," +
                                                                        QString::number(coords.at(1)) + ")");
    /*else output->append("Location unreachable");*/
}
