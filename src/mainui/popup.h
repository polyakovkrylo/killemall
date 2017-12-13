#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QDialog>
#include <string>

struct Values {
    int map;
    int healthpacks;
    int enemies;
};

namespace Ui {
class Popup;
}

class Popup : public QDialog
{
    Q_OBJECT
public:
    explicit Popup(QDialog *parent = nullptr);

public:
    Values getValues();

private:
    Ui::Popup *ui;
    Values vals;

signals:

public slots:
private slots:    
};

#endif // POPUP_H
