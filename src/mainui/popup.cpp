/*!
 * \file popup.cpp
 *
 * Startup dialog class definition
 *
 * \version 1.0
 *
 * \author Vladimir Poliakov
 * \author Brian Segers
 * \author Kasper De Volder
 */

#include "popup.h"
#include "ui_popup.h"

Popup::Popup(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Popup)
{
    ui->setupUi(this);
    QDir dir(":/img/");
    for(QString f : dir.entryList()) {
        ui->chooseWorld->addItem(f);
    }
}

Values Popup::getValues()
{
    vals.map = ui->chooseWorld->currentText();
    vals.healthpacks = ui->nrHealth->value();
    vals.enemies = ui->nrEnemies->value();
    vals.optimization = ui->optimizationSlider->value();
    return vals;
}
