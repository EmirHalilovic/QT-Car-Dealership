#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog {
  Q_OBJECT

public:
  explicit About(QString tablename, QWidget *parent = nullptr);
  ~About();

private slots:
  void on_logout_clicked();

  void on_homeb_clicked();

private:
  Ui::About *ui;
  QString tablename;
};

#endif // ABOUT_H
