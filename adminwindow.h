#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit AdminWindow(QWidget *parent = nullptr);
  ~AdminWindow();
  QSqlQueryModel *model;
  QSqlTableModel *modelTable;
  QString table2;
private slots:
  void updateTableView();
  void onTableViewClicked(const QModelIndex &index);
  void on_updateButton_clicked();
  void on_deleteButton_clicked();
  QString generateUniqueUsername();

  void on_addButton_clicked();

private:
  Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H
