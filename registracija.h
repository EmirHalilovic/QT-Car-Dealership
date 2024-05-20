#ifndef REGISTRACIJA_H
#define REGISTRACIJA_H

#include "login.h"
#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
namespace Ui {
class Registracija;
}

class Registracija : public QDialog {
  Q_OBJECT

public:
  explicit Registracija(QWidget *parent = nullptr);
  ~Registracija();

private slots:
  void on_Create_clicked();

  void on_pokazi_stateChanged();

  void on_Back_clicked();

private:
  Ui::Registracija *ui;
};

#endif // REGISTRACIJA_H
