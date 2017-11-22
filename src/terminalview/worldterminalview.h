#ifndef WORLDTERMINALVIEW_H
#define WORLDTERMINALVIEW_H

#include <QWidget>
#include <QApplication>
#include <string>
#include <QtWidgets>
#include <iostream>
#include <QString>

#include "model/worldmodel.h"

class WorldTerminalView : public QWidget
{
    Q_OBJECT
public:
    WorldTerminalView(QWidget *parent = 0);
    void setModel(const WorldModel *m);

private:
    //QProcess* proc;
    QLineEdit* cmdLine;
    QTextEdit* output;
    const WorldModel* model;

public:
    void executeCmd(std::string &cmd);

public slots:
    //void slotDataOnStdout();
    void slotReturnPressed();
};

#endif // WORLDTERMINALVIEW_H
