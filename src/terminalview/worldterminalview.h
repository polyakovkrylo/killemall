#ifndef WORLDTERMINALVIEW_H
#define WORLDTERMINALVIEW_H

#include <QWidget>
#include <QApplication>
#include <string>
#include <QtWidgets>
#include <iostream>
#include <QString>
#include <sstream>

#include "model/worldmodel.h"

class WorldTerminalView : public QWidget
{
    Q_OBJECT
public:
    WorldTerminalView(QWidget *parent = 0);
    void setModel(const WorldModel *m);
    std::vector<float> findNearestEnemy();    //return vector with (x,y,strength) of nearest enemy to protagonist
    std::vector<float> findNearestHealth();   //return vector with (x,y,value) of nearest healthpack to protagonist
    void executeCmd(std::string &cmd);
    void help();
    void findProtag();
    void statusProtag();
    void findHealth();
    void findEnemy();
    void move(std::string &cmd);

private:    
    QLineEdit* cmdLine;
    QTextEdit* output;
    const WorldModel* model;
    QString message;

public slots:    
    void onReturnPressed();
    void onEnemyDefeated(int x,int y);
    void onHealthpackUsed(int x,int y);
    void onHealthLevelChanged(int value);
    void onEnergyLevelChanged(int value);
    void onPositionChanged(int x, int y);
};

#endif // WORLDTERMINALVIEW_H
