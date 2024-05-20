#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QString tablename, QWidget *parent = nullptr);
  ~MainWindow();
  int quantity = 0;

private slots:
  void on_logout_clicked();

  void on_aboutb_clicked();

  void on_superb_clicked();

  void on_buttonLoad_clicked();

  void on_kupiBtn_clicked();

  void on_refreshbutton_clicked();

  void on_useMileage_stateChanged(int arg1);

  void on_useAge_stateChanged(int arg1);

private:
  Ui::MainWindow *ui;
  QString tablename;
  int buttonValue;
};
#endif // MAINWINDOW_H
