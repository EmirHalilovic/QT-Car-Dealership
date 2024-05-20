#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QSettings>
#include <QtCore>
#include <QtDebug>
#include <QtGui>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql>

namespace Ui {
class logIn;
}

class logIn : public QDialog {
  Q_OBJECT

public:
  explicit logIn(QWidget *parent = nullptr);
  ~logIn();

signals:
  void loggedIn(QString tablename);

private slots:

  void on_Login_clicked();

  void on_pokazi_stateChanged();

  void on_Register_clicked();

  void on_Confirmdb_clicked();

private:
  Ui::logIn *ui;
  QSqlDatabase mydatabase;
  QString tablename;
  QString dbLocation;
};

#endif // LOGIN_H
