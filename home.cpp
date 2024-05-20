#include "home.h"
#include "ui_home.h"
#include "about.h"
#include "login.h"
#include <QtWidgets>
#include <QScrollArea>
home::home(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::home)
{
    ui->setupUi(this);


}

home::~home()
{
    delete ui;
}




void home::on_aboutb_clicked()
{
  home *ho = new home;
    ho->show();
     hide();
}

