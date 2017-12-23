#ifndef WORLDTERMINALVIEW_H
#define WORLDTERMINALVIEW_H

/*!
 * \file worldterminalview.h
 *
 * WorldTerminalView class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include <QWidget>
#include <QApplication>
#include <string>
#include <QtWidgets>
#include <iostream>
#include <QString>
#include <sstream>

#include "model/worldmodel.h"

/*!
 * \defgroup terminalview
 * \brief Terminal view implementation
 */

/*!
  \ingroup terminalview
 * \brief The WorldTerminalView class
 */
class WorldTerminalView : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Class constructor
     * \param parent parent object
     */
    WorldTerminalView(QWidget *parent = 0);
    /*!
     * \brief Set model for the view
     * \param m model
     */
    void setModel(WorldModel *m);
    /*!
     * \brief Execute a command
     * \param cmd Command string
     * \param args Arguments
     */
    void executeCmd(QString &cmd, QStringList args);
    /*!
     * \brief Print help on a command
     * \param command command name(print overall help if no command is sepcified)
     */
    void help(QString command = "");
    /*!
     * \brief Print information about a closest object
     *
     * Print information about the closest object of a given type
     * (e = any enemy, pe = poisoned enemy, re = regular enemy, h = health pack).
     * \param object type of object
     * \param value threshold value of the object(minimal for health pack, maximal for enemy)
     */
    void find(QString object, float value);
    /*!
     * \brief Print info about all objects of a given type
     *
     * * Print information about all objects of a given type
     * (e = all enemies, pe = poisoned enemies, re = regular enemies, h = health packs).
     * \param object type of object
     */
    void printInfo(QString object);

private:
    QLineEdit* cmdLine;
    QTextEdit* output;
    WorldModel* model;
    QString message;

public slots:
    /*!
     * \brief Read and execute the typed command
     */
    void onReturnPressed();
    /*!
     * \brief Print info on enemy defeated signal
     * \param x horizontal position of the enemy
     * \param y vertical position of the enemy
     */
    void onEnemyDefeated(int x,int y);
    /*!
     * \brief Print info on areaPoisoned signal
     * \param value poison level
     * \param rect poisoned area
     */
    void onAreaPosioned(int value, QRect rect);
    /*!
     * \brief Print info on healthPackUsed signal
     * \param x horizontal position of the health pack
     * \param y vertical position of the health pack
     */
    void onHealthpackUsed(int x,int y);
    /*!
     * \brief Print info on healthLevelChanged signal
     * \param value health level
     */
    void onHealthLevelChanged(int value);
    /*!
     * \brief Print info on protagonist position changed
     * \param x horizontal position of the protagonist
     * \param y vertical position of the protagonist
     */
    void onPositionChanged(int x, int y);
    /*!
     * \brief Reload the view slot
     * Reloads the view every time the model is reloaded
     */
    void reloadView();
};

#endif // WORLDTERMINALVIEW_H
