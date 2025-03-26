#include "mainwindow.h"
#include "about.h"
#include "login.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QDir>

// Constructor
MainWindow::MainWindow(QString tablename, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), tablename(tablename), buttonValue(0) {
    ui->setupUi(this);
    connect(ui->backupButton, &QPushButton::clicked, this, &MainWindow::backupDatabase);
    connect(ui->restoreButton, &QPushButton::clicked, this, &MainWindow::restoreDatabase);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportDataToCSV);
    connect(ui->importButton, &QPushButton::clicked, this, &MainWindow::importDataFromCSV);

    setWindowIcon(QIcon(":/polo"));
    setFixedSize(QSize(1100, 700));

    // Load stylesheet
    QFile styleSheetFile(":/Integrid.qss");
    if (styleSheetFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleSheetFile.readAll());
        this->setStyleSheet(styleSheet);
    }

    // Setup system tray icon for notifications
    setupTrayIcon();

    // ... (Existing code to setup tables, execute queries, etc.)

    // Example query to fill main table (you might adjust as needed)
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();
    query.exec("SELECT Brand, Fuel, Mileage, Age, Engine, Price FROM Cars");
    model->setQuery(std::move(query));
    ui->dbtable->setModel(model);

    // Populate the sold table using your existing logic...
    // (Make sure to use prepared statements to protect against SQL injection.)
}

// Destructor
MainWindow::~MainWindow() {
    delete ui;
}

// ======================
// Notification Functions
// ======================
void MainWindow::setupTrayIcon() {
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(windowIcon());
    trayIcon->show();
}

void MainWindow::showNotification(const QString &title, const QString &message) {
    if (trayIcon) {
        trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
    }
}

// ======================
// Database Backup & Restore
// ======================
void MainWindow::backupDatabase() {
    // Get the current database file path
    QString dbFilePath = QSqlDatabase::database().databaseName();
    // Ask user for a location to save the backup file
    QString backupPath = QFileDialog::getSaveFileName(this, "Select Backup File", "", "Database Files (*.db);;All Files (*)");
    if (backupPath.isEmpty())
        return;

    // Make a copy of the database file
    if (QFile::copy(dbFilePath, backupPath)) {
        showNotification("Backup Successful", "Database backup created successfully.");
    } else {
        QMessageBox::warning(this, "Backup Failed", "Failed to create backup.");
    }
}

void MainWindow::restoreDatabase() {
    // Get the current database file path
    QString dbFilePath = QSqlDatabase::database().databaseName();

    // Ask user for the backup file to restore from
    QString restorePath = QFileDialog::getOpenFileName(this, "Select Backup File", "", "Database Files (*.db);;All Files (*)");
    if (restorePath.isEmpty())
        return;

    // (Optional: Close the database connection if needed before copying)

    // Copy the backup file over the current database file
    if (QFile::copy(restorePath, dbFilePath)) {
        showNotification("Restore Successful", "Database restored successfully.");
        // (Optional: Reopen or refresh the database connection here.)
    } else {
        QMessageBox::warning(this, "Restore Failed", "Failed to restore backup.");
    }
}

// ======================
// Export & Import CSV Functions
// ======================
void MainWindow::exportDataToCSV() {
    // Ask the user for a file path to save CSV
    QString fileName = QFileDialog::getSaveFileName(this, "Export Data", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Export Failed", "Cannot open file for writing.");
        return;
    }

    QTextStream stream(&file);
    // Write CSV header
    stream << "Brand,Fuel,Mileage,Age,Engine,Price\n";

    // Retrieve data from the Cars table
    QSqlQuery query("SELECT Brand, Fuel, Mileage, Age, Engine, Price FROM Cars");
    while (query.next()) {
        QString line = QString("%1,%2,%3,%4,%5,%6")
        .arg(query.value("Brand").toString())
            .arg(query.value("Fuel").toString())
            .arg(query.value("Mileage").toString())
            .arg(query.value("Age").toString())
            .arg(query.value("Engine").toString())
            .arg(query.value("Price").toString());
        stream << line << "\n";
    }
    file.close();
    showNotification("Export Successful", "Data exported to CSV successfully.");
}

void MainWindow::importDataFromCSV() {
    // Ask the user for the CSV file to import
    QString fileName = QFileDialog::getOpenFileName(this, "Import Data", "", "CSV Files (*.csv)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Import Failed", "Cannot open file for reading.");
        return;
    }

    QTextStream stream(&file);
    // Read header (assumes header is present)
    QString headerLine = stream.readLine();

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    db.transaction();  // Start a transaction for efficiency

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList fields = line.split(",");
        if (fields.size() < 6)
            continue;  // Skip incomplete rows

        query.prepare("INSERT INTO Cars (Brand, Fuel, Mileage, Age, Engine, Price) VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(fields.at(0));
        query.addBindValue(fields.at(1));
        query.addBindValue(fields.at(2).toInt());
        query.addBindValue(fields.at(3).toInt());
        query.addBindValue(fields.at(4).toDouble());
        query.addBindValue(fields.at(5).toDouble());

        if (!query.exec()) {
            db.rollback();
            QMessageBox::warning(this, "Import Failed", "Failed to import data: " + query.lastError().text());
            file.close();
            return;
        }
    }
    db.commit();
    file.close();
    showNotification("Import Successful", "Data imported from CSV successfully.");
}

// ======================
// (Existing slots follow...)
// ======================

void MainWindow::on_logout_clicked() {
    logIn *log = new logIn;
    log->show();
    hide();
}

void MainWindow::on_aboutb_clicked() {
    About *aub = new About(tablename);
    aub->show();
    hide();
}

void MainWindow::on_superb_clicked() {
    buttonValue = !buttonValue;
    QFont font = ui->superb->font();
    font.setWeight(buttonValue ? QFont::Bold : QFont::Normal);
    ui->superb->setFont(font);
    QString style = buttonValue ? "QPushButton { background-color: rgba(70,162,218,50%); }" : "";
    ui->superb->setStyleSheet(style);
}
void MainWindow::on_buttonLoad_clicked() { QSqlQueryModel m; }

// Get the selected row from the database table
void MainWindow::on_kupiBtn_clicked() {
    int index = ui->dbtable->currentIndex().row();
    if (index < 0) {
        QMessageBox::warning(this, "No selection", "Please select a row from the table.");
        return;
    }

    QString brand = ui->dbtable->model()->index(index, 0).data().toString();
    QString fuel = ui->dbtable->model()->index(index, 1).data().toString();
    int mileage = ui->dbtable->model()->index(index, 2).data().toInt();
    int age = ui->dbtable->model()->index(index, 3).data().toInt();
    qreal engine = ui->dbtable->model()->index(index, 4).data().toReal();
    qreal price = ui->dbtable->model()->index(index, 5).data().toReal();

    QSqlQuery query;
    query.prepare("SELECT * FROM Cars_Account WHERE car_id = (SELECT ID FROM Cars WHERE "
                  "Brand = :brand AND Fuel = :fuel AND Mileage = :mileage AND Age = :age "
                  "AND Engine = :engine AND Price = :price) AND account_id = (SELECT ID "
                  "FROM Account WHERE User = :user)");
    query.bindValue(":brand", brand);
    query.bindValue(":fuel", fuel);
    query.bindValue(":mileage", mileage);
    query.bindValue(":age", age);
    query.bindValue(":engine", engine);
    query.bindValue(":price", price);
    query.bindValue(":user", tablename);

    if (query.exec() && query.next()) {
        query.prepare("UPDATE Cars_Account SET quantity = quantity + 1 WHERE car_id = "
                      "(SELECT ID FROM Cars WHERE Brand = :brand AND Fuel = :fuel AND "
                      "Mileage = :mileage AND Age = :age AND Engine = :engine AND Price = "
                      ":price) AND account_id = (SELECT ID FROM Account WHERE User = :user)");
        query.bindValue(":brand", brand);
        query.bindValue(":fuel", fuel);
        query.bindValue(":mileage", mileage);
        query.bindValue(":age", age);
        query.bindValue(":engine", engine);
        query.bindValue(":price", price);
        query.bindValue(":user", tablename);

        if (query.exec()) {
            QMessageBox::information(this, "Purchase", "Quantity updated successfully.");
        } else {
            QMessageBox::critical(this, "Purchase", "Failed to update quantity: " + query.lastError().text());
        }
    } else {
        query.prepare("INSERT INTO Cars_Account (car_id, account_id, quantity) VALUES "
                      "((SELECT ID FROM Cars WHERE Brand = :brand AND Fuel = :fuel AND "
                      "Mileage = :mileage AND Age = :age AND Engine = :engine AND Price = "
                      ":price), (SELECT ID FROM Account WHERE User = :user), 1)");
        query.bindValue(":brand", brand);
        query.bindValue(":fuel", fuel);
        query.bindValue(":mileage", mileage);
        query.bindValue(":age", age);
        query.bindValue(":engine", engine);
        query.bindValue(":price", price);
        query.bindValue(":user", tablename);

        if (query.exec()) {
            QMessageBox::information(this, "Purchase", "Car added to Cars_Account successfully.");
        } else {
            QMessageBox::critical(this, "Purchase", "Failed to add car to Cars_Account: " + query.lastError().text());
        }
    }

    // Oslobađanje prethodnog modela
    if (ui->soldtable->model()) {
        delete ui->soldtable->model();
    }

    // Kreiranje novog modela i ažuriranje
    QSqlQueryModel *soldTableModel = new QSqlQueryModel();
    if (query.exec("SELECT cr.Brand, cr.Fuel, cr.Mileage, cr.Age, cr.Engine, cr.Price, ca.quantity "
                   "FROM Cars AS cr "
                   "INNER JOIN Cars_Account AS ca ON cr.ID = ca.car_id "
                   "INNER JOIN Account AS ac ON ac.ID = ca.account_id "
                   "WHERE ac.User = '" + tablename + "'")) {
        soldTableModel->setQuery(query.executedQuery());
        ui->soldtable->setModel(soldTableModel);
    } else {
        qDebug() << "Error executing query for soldtable: " << query.lastError();
        delete soldTableModel;
    }
}


void MainWindow::on_refreshbutton_clicked() {
    QString brand = ui->searchText->text();
    QString fuel = ui->comboBox->currentText();
    QString startAge = ui->startage->text();
    QString endAge = ui->endage->text();
    QString startMileage = ui->startmilage->text();
    QString endMileage = ui->endmilage->text();

    QString whereClause;

    if (!brand.isEmpty()) {
        whereClause += "Brand LIKE '" + brand + "%' AND ";
    }

    if (!fuel.isEmpty()) {
        whereClause += "Fuel='" + fuel + "' AND ";
    }

    if (ui->useAge->isChecked()) {
        if (!startAge.isEmpty() && !endAge.isEmpty()) {
            whereClause += "Age BETWEEN " + startAge + " AND " + endAge + " AND ";
        }
    }

    if (ui->useMileage->isChecked()) {
        if (!startMileage.isEmpty() && !endMileage.isEmpty()) {
            whereClause += "Mileage BETWEEN " + startMileage + " AND " + endMileage + " AND ";
        }
    }
    if (buttonValue == 1) {
        whereClause += "SuperCar=" + QString::number(buttonValue) + " AND ";
    }

    if (whereClause.endsWith(" AND ")) {
        whereClause.chop(5);
    }

    QString queryStr = "SELECT Brand, Fuel, Mileage, Age, Engine,Price FROM Cars";

    if (!whereClause.isEmpty()) {
        queryStr += " WHERE " + whereClause;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(queryStr));

    ui->dbtable->setModel(model);
}

void MainWindow::on_useMileage_stateChanged(int state) {
    ui->startmilage->setEnabled(state == Qt::Checked);
    ui->endmilage->setEnabled(state == Qt::Checked);
}

void MainWindow::on_useAge_stateChanged(int state) {
    ui->startage->setEnabled(state == Qt::Checked);
    ui->endage->setEnabled(state == Qt::Checked);
}
