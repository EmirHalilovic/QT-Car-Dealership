#include "about.h"
#include "mainwindow.h"
#include "ui_about.h"
#include <QApplication>
#include <QFile>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QDate>

#include <login.h>

About::About(QString tablename, QWidget *parent)
    : QDialog(parent), ui(new Ui::About) {
    ui->setupUi(this);

    QLabel *versionLabel = new QLabel(this);
    versionLabel->setText("Version: 1.0.0\nBuild Date: 01 January 2025");
    versionLabel->setAlignment(Qt::AlignCenter);
    setWindowIcon(QIcon(":/polo"));
    setFixedSize(QSize(810, 550));

    // Apply stylesheet
    QFile styleSheetFile(":/Integrid.qss");
    if (styleSheetFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleSheetFile.readAll());
        this->setStyleSheet(styleSheet);
    }

    this->tablename = tablename;
    setWindowTitle("About " + tablename);

    // Create a layout to add content

    QVBoxLayout *layout = ui->verticalLayout;
    layout->addWidget(versionLabel);


    // Application Description
    QLabel *descriptionLabel = new QLabel(this);
    descriptionLabel->setText("This application is a comprehensive car sales management system.\n"
                              "It supports user management, car listings, sales statistics, and more.");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(descriptionLabel);



    // Developer/Team Info
    QLabel *developerLabel = new QLabel(this);
    developerLabel->setTextFormat(Qt::RichText);
    developerLabel->setText("Developed by: Emir Halilovic<br>Visit: <a href='https://www.youtube.com/watch?v=dQw4w9WgXcQ'>Youtube Hit</a>");
    developerLabel->setOpenExternalLinks(true);
    developerLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(developerLabel);

    // Add spacing at the bottom
    layout->addStretch();
}

About::~About() {
    delete ui;
}

// Logout button slot
void About::on_logout_clicked() {
    logIn *log = new logIn;
    log->show();
    hide();
}

// Home button slot
void About::on_homeb_clicked() {
    MainWindow *MainW = new MainWindow(tablename);
    MainW->show();
    hide();
}
