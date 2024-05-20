#include "registracija.h"
#include "ui_registracija.h"
#include <QFile>

Registracija::Registracija(QWidget *parent)
    : QDialog(parent), ui(new Ui::Registracija) {
  ui->setupUi(this);
  setWindowTitle("Registration");
  setWindowIcon(QIcon(":/polo"));
  setFixedSize(QSize(530, 400));
  QFile styleSheetFile(
      "C:/Users/win10/Desktop/diplomski-rad-main/Diplomski_rad/Integrid.qss");
  styleSheetFile.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(styleSheetFile.readAll());
  this->setStyleSheet(styleSheet);

  ui->user->setMaxLength(10);

  ui->pass->setMaxLength(20);
  ui->user->setPlaceholderText("Username (min 4 characters)");
  ui->pass->setPlaceholderText("Password (min 5 characters)");
}

Registracija::~Registracija() { delete ui; }
void Registracija::on_Create_clicked() {
  QString user = ui->user->text();
  QString pass = ui->pass->text();
  QString fname = ui->fname->text();
  QString lname = ui->lname->text();
  QString email = ui->email->text();

  // Check if username and password meet requirements
  if (user.length() < 4 || user.length() > 10) {
    QMessageBox::warning(this, "Invalid Username",
                         "Username must be between 4 and 10 characters.");
    return;
  }
  if (pass.length() < 5) {
    QMessageBox::warning(this, "Invalid Password",
                         "Password must be at least 5 characters.");
    return;
  }
  if (fname.isEmpty()) {
    QMessageBox::warning(this, "Missing First Name",
                         "Please enter your first name.");
    return;
  }
  if (lname.isEmpty()) {
    QMessageBox::warning(this, "Missing Last Name",
                         "Please enter your last name.");
    return;
  }
  if (!email.contains('@') || !email.contains('.')) {
    QMessageBox::warning(this, "Invalid Email",
                         "Please enter a valid email address.");
    return;
  }

  // Check if username already exists
  QSqlQuery checkQuery;
  checkQuery.prepare("SELECT User FROM Account WHERE User = (:user)");
  checkQuery.bindValue(":user", user);
  if (!checkQuery.exec()) {
    qDebug() << "Check query failed: " << checkQuery.lastError();
    return;
  }
  if (checkQuery.next()) {
    QMessageBox::warning(
        this, "Duplicate Username",
        "Username already exists. Please choose a different username.");
    return;
  }

  // Insert new user into database
  QSqlQuery query;
  query.prepare(
      "INSERT INTO Account (User, Password, Firstname, Lastname, Email, Roles) "
      "VALUES (:user, :pass, :fname, :lname, :email, 'user')");
  query.bindValue(":user", user);
  query.bindValue(":pass", pass);
  query.bindValue(":fname", fname);
  query.bindValue(":lname", lname);
  query.bindValue(":email", email);

  if (!query.exec()) {
    qDebug() << "Failed to store data: " << query.lastError();
    return;
  }

  // Create a new table for the user's sold cars
  QString tablename = user;

  qDebug() << "Data stored successfully";
  logIn *log = new logIn;
  log->show();
  close();
}

void Registracija::on_pokazi_stateChanged() {

  ui->pass->setEchoMode(ui->pokazi->checkState() == Qt::Checked
                            ? QLineEdit::Normal
                            : QLineEdit::Password);
  ui->pokazi->checkState() == Qt::Checked ? ui->pokazi->setText("HIDE ")
                                          : ui->pokazi->setText("SHOW");
}

void Registracija::on_Back_clicked() {
  logIn *log = new logIn;
  log->show();
  hide();
}
