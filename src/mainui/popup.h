/*!
 * \file popup.h
 *
 * Startup dialog class declaration
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QDialog>
#include <QDir>
#include <string>

/*!
 * \ingroup mainui
 * \brief Current values of Popup controls
 */
struct Values {
    QString map;
    int healthpacks;
    int enemies;
    int optimization;
    int energyCost;
};

namespace Ui {
class Popup;
}

/*!
 * \ingroup mainui
 * \brief Stratup dialog class
 *
 * Allows user to pick the map, number of enemies and health packs
 * and optimization level of the controller
 */
class Popup : public QDialog
{
    Q_OBJECT
public:
    explicit Popup(QDialog *parent = nullptr);

public:
    /*!
     * \brief Returns values of Popup controls
     */
    Values getValues();

private:
    Ui::Popup *ui;
    Values vals;
};

#endif // POPUP_H
