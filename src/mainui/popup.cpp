#include "popup.h"
#include "ui_popup.h"

Popup::Popup(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Popup)
{
    ui->setupUi(this);
}

Values Popup::getValues()
{
    vals.map = ui->chooseWorld->currentIndex();
    vals.healthpacks = ui->nrHealth->value();
    vals.enemies = ui->nrEnemies->value();
    vals.optimization = ui->optimizationSlider->value();
    return vals;
}
