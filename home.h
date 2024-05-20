#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include "login.h"
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui {class home;}
QT_END_NAMESPACE

class home : public QDialog

{
    Q_OBJECT
public:
    explicit home(QWidget *parent = nullptr);
    ~home();

private slots:
    void on_aboutb_clicked();

private:
    Ui::home *ui;
};

#endif // HOME_H

