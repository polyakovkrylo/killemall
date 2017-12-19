#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QDialog>
#include <QDir>
#include <string>

struct Values {
    QString map;
    int healthpacks;
    int enemies;
    int optimization;
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
