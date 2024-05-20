#include "adminwindow.h"
#include "login.h"
#include "ui_adminwindow.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QWidget>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalTableModel>
AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AdminWindow) {
  ui->setupUi(this);
  setWindowTitle("Admin Window");

  QSqlDatabase db =
      QSqlDatabase::database(); // Use the existing database connection
  qDebug() << "Is connection opened" << db.isOpen();

  if (!db.open()) {
    qDebug() << "Failed to open database!";
  }
  setWindowIcon(QIcon(":/polo"));
  setFixedSize(QSize(1000, 450));
  QFile styleSheetFile(
      "C:/Users/win10/Desktop/diplomski-rad-main/Diplomski_rad/Integrid.qss");
  styleSheetFile.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(styleSheetFile.readAll());
  this->setStyleSheet(styleSheet);

  QSqlQuery query("SELECT User FROM Account");
  while (query.next()) {
    QString user = query.value(0).toString();
    ui->userCombobox->addItem(user);
  }

  ui->view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->userCombobox->setEnabled(false);

  QObject::connect(ui->userCombobox,
                   QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                   &AdminWindow::updateTableView);
  QObject::connect(
      ui->combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      [=]() {
        QTableView *view = new QTableView();
        view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        QString table = ui->combo->currentText();
        QString selectedUser = ui->userCombobox->currentText();
        QSqlQuery query;

        if (table == "Cars_Account") {
          if (selectedUser != "ALL") {
            QSqlQueryModel *cars_Account = new QSqlQueryModel();
            QString queryString =
                "SELECT car_account_id,cr.Brand, cr.Fuel, cr.Mileage, cr.Age, "
                "cr.Engine, cr.Price,ac.User, ca.quantity "
                "FROM Cars AS cr "
                "INNER JOIN Cars_Account AS ca ON cr.ID = ca.car_id "
                "INNER JOIN Account AS ac ON ac.ID = ca.account_id "
                "WHERE ac.User = :user";
            QSqlQuery query;
            query.prepare(queryString);
            query.bindValue(":user", selectedUser);
            query.exec();
            cars_Account->setQuery(std::move(query));
            ui->view->setModel(cars_Account);

          } else {
            QSqlQueryModel *cars_Account = new QSqlQueryModel();
            QString queryString =
                "SELECT car_account_id,cr.Brand, cr.Fuel, cr.Mileage, cr.Age, "
                "cr.Engine, cr.Price,ac.User, ca.quantity "
                "FROM Cars AS cr "
                "INNER JOIN Cars_Account AS ca ON cr.ID = ca.car_id "
                "INNER JOIN Account AS ac ON ac.ID = ca.account_id ";

            QSqlQuery query;
            query.prepare(queryString);

            query.exec();
            cars_Account->setQuery(std::move(query));
            ui->view->setModel(cars_Account);
          }
          ui->userCombobox->setEnabled(true);
        } else {
          QSqlQueryModel *model = new QSqlQueryModel();
          QString queryString = "SELECT * FROM " + table;
          query.exec(queryString);
          model->setQuery(std::move(query));
          ui->view->setModel(model);

          ui->userCombobox->setEnabled(false);
        }
      });

  QObject::connect(ui->exitButton, &QPushButton::clicked, this, [=]() {
    logIn *log = new logIn;
    log->show();
    close();
  });

  connect(ui->view, SIGNAL(clicked(const QModelIndex &)), this,
          SLOT(onTableViewClicked(const QModelIndex &)));
}

AdminWindow::~AdminWindow() { delete ui; }

void AdminWindow::updateTableView() {
  QString table = ui->combo->currentText();
  QString selectedUser = ui->userCombobox->currentText();

  if (table == "Cars_Account") {
    if (selectedUser != "ALL") {
      QSqlQueryModel *cars_Account = new QSqlQueryModel();
      QString queryString =
          "SELECT car_account_id,cr.Brand, cr.Fuel, cr.Mileage, cr.Age, "
          "cr.Engine, cr.Price,ac.User, ca.quantity "
          "FROM Cars AS cr "
          "INNER JOIN Cars_Account AS ca ON cr.ID = ca.car_id "
          "INNER JOIN Account AS ac ON ac.ID = ca.account_id "
          "WHERE ac.User = :user";
      QSqlQuery query;
      query.prepare(queryString);
      query.bindValue(":user", selectedUser);
      query.exec();
      cars_Account->setQuery(std::move(query));
      ui->view->setModel(cars_Account);
    } else {
      QSqlQueryModel *model = new QSqlQueryModel();
      QString queryString =
          "SELECT car_account_id,cr.Brand, cr.Fuel, cr.Mileage, cr.Age, "
          "cr.Engine, cr.Price,ac.User, ca.quantity "
          "FROM Cars AS cr "
          "INNER JOIN Cars_Account AS ca ON cr.ID = ca.car_id "
          "INNER JOIN Account AS ac ON ac.ID = ca.account_id";
      QSqlQuery query;
      query.exec(queryString);
      model->setQuery(std::move(query));
      ui->view->setModel(model);
    }
    ui->userCombobox->setEnabled(true);
  } else {
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryString = "SELECT * FROM " + table;
    QSqlQuery query;
    query.exec(queryString);
    model->setQuery(std::move(query));
    ui->view->setModel(model);
    ui->userCombobox->setEnabled(false);
  }
}

void AdminWindow::onTableViewClicked(const QModelIndex &index) {
  QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->view->model());
  if (!model) {
    return;
  }

  int row = index.row();
  int columnCount = model->columnCount();

  // Hide extra labels and line edits
  for (int i = columnCount; i < ui->verticalLayout->count(); ++i) {
    QLabel *label = findChild<QLabel *>(QString("label%1").arg(i + 1));
    QLineEdit *lineEdit =
        findChild<QLineEdit *>(QString("lineEdit%1").arg(i + 1));

    if (label) {
      label->setVisible(false);
    }

    if (lineEdit) {
      lineEdit->setVisible(false);
      lineEdit->clear();
    }
  }

  for (int i = 0; i < columnCount; ++i) {
    QString headerName = model->headerData(i, Qt::Horizontal).toString();
    QString value = model->data(model->index(row, i)).toString();

    QLabel *label = findChild<QLabel *>(QString("label%1").arg(i + 1));
    QLineEdit *lineEdit =
        findChild<QLineEdit *>(QString("lineEdit%1").arg(i + 1));

    if (label) {
      label->setVisible(true);
      label->setText(headerName);
    }

    if (lineEdit) {
      lineEdit->setVisible(true);
      lineEdit->setText(value);

      if (ui->combo->currentText() == "Cars_Account") {
        if (headerName != "quantity") {
          lineEdit->setReadOnly(true);
        } else {
          lineEdit->setReadOnly(false);
        }
      } else {
        lineEdit->setReadOnly(false);
      }
      if (headerName == "ID" && (ui->combo->currentText() == "Cars" ||
                                 ui->combo->currentText() == "Account")) {
        lineEdit->setReadOnly(true);
      }
    }
  }
}

void AdminWindow::on_updateButton_clicked() {
  QTableView *view = new QTableView();
  view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  QModelIndex currentIndex = ui->view->currentIndex();
  int row = currentIndex.row();
  int columnCount = ui->view->model()->columnCount();

  if (!currentIndex.isValid() || columnCount == 0) {
    QMessageBox::information(this, "Update", "No row selected.");
    return;
  }

  QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->view->model());
  if (!model) {
    QMessageBox::information(this, "Update", "Unable to access the model.");
    return;
  }

  QSqlRecord record = model->record(row);
  QString queryString = model->query().lastQuery();
  QStringList parts = queryString.split(" ");
  int fromIndex = parts.indexOf("FROM");
  QString tableName = parts.value(fromIndex + 1);
  QString table = ui->combo->currentText();
  QString selectedUser = ui->userCombobox->currentText();
  QStringList setValues;
  QStringList primaryKeys;
  QSqlQuery query;
  query.prepare(queryString);

  // Build the SET clause and collect primary key values
  for (int column = 0; column < columnCount; ++column) {
    QString columnName = model->headerData(column, Qt::Horizontal).toString();
    QString value =
        qobject_cast<QLineEdit *>(ui->verticalLayout->itemAt(column)->widget())
            ->text();
    int fieldIndex = record.indexOf(columnName);
    QSqlField field = record.field(fieldIndex);
    field.setValue(value);

    if (fieldIndex >= 0 && fieldIndex < record.count()) {
      if (columnName == "ID") {
        primaryKeys << QString("%1 = %2").arg(columnName).arg(value);

      } else if (columnName == "car_account_id") {

        primaryKeys << QString("%1 = %2").arg(columnName).arg(value);

      }

      else {
        bool isStringType = field.metaType().id() == QMetaType::QString;
        if (!value.isEmpty() || table == "Account") {
          if (isStringType && table == "Cars_Account") {
            if (columnName != "quantity") {

              setValues.clear();
            }
          } else {
            if (isStringType) {

              setValues << QString("%1 = '%2'").arg(columnName).arg(value);

            } else {
              setValues << QString("%1 = %2").arg(columnName).arg(value);
            }
          }
        }
      }
    }
  }

  // Build the WHERE clause using primary key values
  QString whereClause = primaryKeys.join(" AND ");

  // Build the SET clause
  QString setClause = setValues.join(", ");

  // Prepare the update query
  queryString = QString("UPDATE %1 SET %2 WHERE %3")
                    .arg(table)
                    .arg(setClause)
                    .arg(whereClause);
  query.prepare(queryString);

  if (query.exec()) {
    model->setQuery(std::move(query));
    updateTableView();
    QMessageBox::information(this, "Update", "Row updated successfully.");
  } else {
    QMessageBox::critical(this, "Update",
                          "Failed to update row: " + query.lastError().text());
  }

  // Clear the line edits
  for (int i = 0; i < columnCount; ++i) {
    QLineEdit *lineEdit =
        qobject_cast<QLineEdit *>(ui->verticalLayout->itemAt(i)->widget());
    if (lineEdit) {
      lineEdit->clear();
    }
  }
}

void AdminWindow::on_deleteButton_clicked() {
  QModelIndex currentIndex = ui->view->currentIndex();
  int row = currentIndex.row();
  QString table = ui->combo->currentText();

  if (row >= 0) {
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->view->model());
    if (!model) {
      QMessageBox::information(this, "Delete", "Unable to access the model.");
      return;
    }
    QString primaryKeyColumn;
    if (table == "Cars_Account") {
      primaryKeyColumn = "car_account_id";
    } else {
      primaryKeyColumn = "ID";
    }
    QVariant primaryKeyValue = model->data(
        model->index(row, model->record().indexOf(primaryKeyColumn)));

    QSqlQuery query;
    query.prepare("DELETE FROM " + table + " WHERE " + primaryKeyColumn +
                  " = ?");
    query.addBindValue(primaryKeyValue);

    if (query.exec()) {
      // Refresh the table view
      updateTableView();
      QMessageBox::information(this, "Delete", "Row deleted successfully");
      model->setQuery(model->query());
    } else {
      QMessageBox::critical(
          this, "Delete", "Failed to delete row: " + query.lastError().text());
    }
  }
}

void AdminWindow::on_addButton_clicked() {
  QString table = ui->combo->currentText();

  if (table == "Cars") {
    // Create a QSqlTableModel for Cars table
    QSqlTableModel *modelTable = new QSqlTableModel(this);
    modelTable->setTable("Cars");
    modelTable->select(); // Fetch the data

    // Insert a new empty row
    int newRow = modelTable->rowCount();
    modelTable->insertRow(newRow);

    // Set default values for each field in the new row
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Brand")),
        "Default Brand");
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Fuel")),
        "Default Fuel");
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Mileage")), 0);
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Age")), 0);
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Engine")), 0.0);
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Price")), 0.0);
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("SuperCar")), 0);

    // Submit the changes to the database
    if (modelTable->submitAll()) {
      QMessageBox::information(this, "Add", "Row added successfully.");
      updateTableView();
    } else {
      QMessageBox::critical(
          this, "Add", "Failed to add row: " + modelTable->lastError().text());
      modelTable->revertAll(); // Revert changes if submission failed
    }

    delete modelTable; // Clean up the model
  } else if (table == "Account") {
    // Create a QSqlTableModel for Account table
    QSqlTableModel *modelTable = new QSqlTableModel(this);
    modelTable->setTable("Account");
    modelTable->select(); // Fetch the data

    // Insert a new empty row
    int newRow = modelTable->rowCount();
    modelTable->insertRow(newRow);

    // Generate a unique default username
    QString defaultUser = generateUniqueUsername();

    // Set default values for each field in the new row
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("User")), defaultUser);
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Password")),
        "Default Password");
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("FirstName")),
        "Default First Name");
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("LastName")),
        "Default Last Name");
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Email")),
        "Default Email");
    modelTable->setData(
        modelTable->index(newRow, modelTable->fieldIndex("Roles")), "user");

    // Submit the changes to the database
    if (modelTable->submitAll()) {
      updateTableView();
      QMessageBox::information(this, "Add", "Row added successfully.");

    } else {
      QMessageBox::critical(
          this, "Add", "Failed to add row: " + modelTable->lastError().text());
      modelTable->revertAll(); // Revert changes if submission failed
    }

    delete modelTable; // Clean up the model
  } else {
    QMessageBox::information(this, "Add",
                             "Adding rows to this table is not supported.");
  }
}

QString AdminWindow::generateUniqueUsername() {
  QString usernameBase = "DefaultUser";
  int counter = 1;
  QString defaultUser = usernameBase;

  QSqlQuery query;
  while (true) {
    query.prepare("SELECT COUNT(*) FROM Account WHERE User = ?");
    query.addBindValue(defaultUser);
    query.exec();
    if (query.next()) {
      int count = query.value(0).toInt();
      if (count == 0) {
        break; // Found a unique username
      }
    }

    defaultUser = usernameBase + QString::number(counter);
    counter++;
  }

  return defaultUser;
}
