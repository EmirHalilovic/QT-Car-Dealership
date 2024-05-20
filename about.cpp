#include "about.h"
#include "mainwindow.h"
#include "ui_about.h"
#include <QApplication>
#include <QFile>
#include <QLabel>
#include <QPixmap>
#include <qpixmap.h>

#include <login.h>
About::About(QString tablename, QWidget *parent)
    : QDialog(parent), ui(new Ui::About) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":/polo"));
  setFixedSize(QSize(810, 550));
  QFile styleSheetFile(
      "C:/Users/win10/Desktop/diplomski-rad-main/Diplomski_rad/Integrid.qss");
  styleSheetFile.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(styleSheetFile.readAll());
  this->setStyleSheet(styleSheet);

  this->tablename = tablename;
  setWindowTitle(tablename);
}

About::~About() { delete ui; }

void About::on_logout_clicked() {
  logIn *log = new logIn;
  log->show();
  hide();
}

void About::on_homeb_clicked() {
  MainWindow *MainW = new MainWindow(tablename);
  MainW->show();
  hide();
}
