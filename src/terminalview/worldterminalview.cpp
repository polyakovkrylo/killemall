#include "worldterminalview.h"

WorldTerminalView::WorldTerminalView(QWidget *parent) : QWidget(parent)
{    
    //proc = new QProcess(this);
    output = new QTextEdit;
    output->setReadOnly(true);

    QLabel* label = new QLabel("&Command:");

    cmdLine = new QLineEdit("");
    label->setBuddy(cmdLine);
    cmdLine->setFocusPolicy(Qt::StrongFocus);


    QPushButton* enterBtn = new QPushButton("&Enter");

    /*connect(proc,
            SIGNAL(readyReadStandardOutput()),
            this,
            SLOT(slotDataOnStdout())
           );*/
    connect(cmdLine,
            SIGNAL(returnPressed()),
            this,
            SLOT(slotReturnPressed())
           );
    connect(enterBtn, SIGNAL(clicked()), this, SLOT(slotReturnPressed()));

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
}

/*void WorldTerminalView::slotDataOnStdout()
{
    output->append(proc->readAllStandardOutput());
}*/


void WorldTerminalView::slotReturnPressed()
{
    std::string strCommand = "";
    strCommand += cmdLine->text().toStdString();
    executeCmd(strCommand);
    cmdLine->clear();
}

void WorldTerminalView::executeCmd(std::string &cmd)
{
    auto &p = model->getProtagonist();
    output->setTextColor(QColor(0,0,0));
    output->append(QString::fromStdString(cmd));
    output->setTextColor(QColor(255,0,0));
    if(cmd == "help") output->append(">List of commands:\n"
                                     ">find protag\n"
                                     ">status protag\n"
                                     ">locate nearest enemy\n"
                                     ">locate nearest healthpack");
    else if(cmd == "find protag") output->append(">Protagonist is located at (" +
                                                 QString::number(model->getProtagonist()->getXPos()) + "," +
                                                 QString::number(model->getProtagonist()->getYPos()) + ")");
    else if(cmd == "status protag") output->append(">Health: " + QString::number(p->getHealth()) +
                                                   " Energy: " + QString::number(p->getEnergy()));
    else output->append(">invalid cmd, type 'help' to see all commands");

}
