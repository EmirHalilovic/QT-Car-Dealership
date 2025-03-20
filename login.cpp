#include "login.h"
#include "adminwindow.h"
#include "mainwindow.h"
#include "registracija.h"
#include "ui_login.h"
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QtCore>
#include <QtGui>
#include <QtSql/QSqlDatabase>

logIn::logIn(QWidget *parent) : QDialog(parent), ui(new Ui::logIn) {
  ui->setupUi(this);
  setWindowTitle("Welcome");
  setWindowIcon(QIcon(":/polo"));
  setFixedSize(QSize(530, 500));

  // Use relative path for stylesheet
  QFile styleSheetFile(":/styles/Integrid.qss");
  styleSheetFile.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(styleSheetFile.readAll());
  this->setStyleSheet(styleSheet);

  // Use QSettings for database location and last user
  QString defaultDbLocation = "autosalon.db";
  QSettings settings("AutoSalon", "MyApp");
  dbLocation = settings.value("dbLocation", defaultDbLocation).toString();
  ui->dbLocationLineEdit->setText(dbLocation);
  ui->user->setText(settings.value("lastUser", "").toString());

  mydatabase = QSqlDatabase::addDatabase("QSQLITE");
  mydatabase.setDatabaseName(dbLocation);
  if (!mydatabase.open()) {
    ui->stanje->setText("Error");
  } else {
    ui->stanje->setText("Connected");
  }
}

void logIn::on_Login_clicked() {
  QString user = ui->user->text();
  QString pass = ui->pass->text();

  if (!mydatabase.isOpen()) {
    qDebug() << "Failed to open the database";
    return;
  }
  QSqlQuery query;

  if (query.exec("SELECT * FROM Account WHERE User='" + user +
                 "' AND Password='" + pass + "'")) {
    int i = 0;
    QString role;

    while (query.next()) {
      i++;
      role = query.value("Roles").toString();
    }

    if (i == 1) {
      QSettings settings("AutoSalon", "MyApp");
      settings.setValue("lastUser", user); // Remember last user

      if (role == "admin") {
        ui->stanje->setText("Admin Login Successful");
        AdminWindow *adminW = new AdminWindow();
        adminW->show();
      } else {
        ui->stanje->setText("Login Successful");
        MainWindow *MainW = new MainWindow(user);
        MainW->show();
      }
      this->close();
    } else {
      QMessageBox::warning(this, "login",
                           "Username and password is not correct");
    }
  }
}

logIn::~logIn() { delete ui; }

void logIn::on_pokazi_stateChanged() {
  ui->pass->setEchoMode(ui->pokazi->checkState() == Qt::Checked
                            ? QLineEdit::Normal
                            : QLineEdit::Password);
  ui->pokazi->checkState() == Qt::Checked ? ui->pokazi->setText("HIDE ")
                                          : ui->pokazi->setText("SHOW");
}

void logIn::on_Register_clicked() {
  this->hide();
  Registracija *regi = new Registracija(this);
  regi->show();
}

void logIn::on_Confirmdb_clicked() {
  // Update the database location
  dbLocation = ui->dbLocationLineEdit->text();
  mydatabase.setDatabaseName(dbLocation);

  // Reopen the database
  if (!mydatabase.open()) {
    QMessageBox::warning(this, "Error", "Failed to open the database");
  } else {
    QMessageBox::information(this, "Success",
                             "Database location updated successfully");

    // Save the database location to the configuration file
    QSettings settings("AutoSalon", "MyApp");
    settings.setValue("dbLocation", dbLocation);
  }
}
