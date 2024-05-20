#include "login.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  logIn w;

  QFile styleSheetFile(".Integrid.qss");
  styleSheetFile.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(styleSheetFile.readAll());
  a.setStyleSheet(styleSheet);

  w.show();
  return a.exec();
}
