#include <QWidget>
#include <QString>
#include <QList>

namespace Ui {
class carListView;
}

class carListView : public QWidget
{
    Q_OBJECT

public:
    explicit carListView(QWidget *parent = nullptr);
    ~carListView();

private slots:
    void on_reservePushButton_clicked();

    void on_filterPushButton_clicked();

private:
    Ui::carListView *ui;
    void loadCars();
    QString selectedCarId;

    QList<QStringList> allCars;
    void loadCarsFromFile();
    void showCars(const QList<QStringList>&cars);
};

#endif // CARLISTVIEW_H



#include "carlistview.h"
#include "ui_carlistview.h"
#include "reservationview.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

carListView::carListView(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::carListView)
{
    ui->setupUi(this);

    // تنظیمات ساده جدول
    ui->CarTableWidget->setColumnCount(5);
    ui->CarTableWidget->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Brand" << "Type" << "Price" << "Status"
        );
    ui->CarTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->CarTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->CarTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    loadCarsFromFile();
}

carListView::~carListView()
{
    delete ui;
}

void carListView::loadCarsFromFile()
{
    QString path = QCoreApplication::applicationDirPath() + "/cars.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    allCars.clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 5)
            allCars.append(parts);
    }
    file.close();

    showCars(allCars);
}

void carListView::showCars(const QList<QStringList>& cars)
{
    ui->CarTableWidget->setRowCount(0);

    for (int i = 0; i < cars.size(); i++) {
        ui->CarTableWidget->insertRow(i);
        for (int j = 0; j < 5; j++) {
            ui->CarTableWidget->setItem(i, j,
                                  new QTableWidgetItem(cars[i][j]));
        }
    }
}

void carListView::on_reservePushButton_clicked()
{
    int row = ui->CarTableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a car");
        return;
    }

    QString status = ui->CarTableWidget->item(row, 4)->text();
    if (status != "Available") {
        QMessageBox::warning(this, "Error", "Car is not available");
        return;
    }

    QString carId = ui->CarTableWidget->item(row, 0)->text();
    QString brand = ui->CarTableWidget->item(row, 1)->text();

    reservationView *view = new reservationView(carId, brand);
    view->show();
}

void carListView::on_filterPushButton_clicked()
{
    QString brand = ui->brandLineEdit->text().toLower();
    QString type = ui->typeLineEdit->text().toLower();
    QString maxPriceText = ui->maxPriceLineEdit->text();

    QList<QStringList> filtered;

    for (int i = 0; i < allCars.size(); i++) {
        QStringList car = allCars[i];

        if (!brand.isEmpty() &&
            !car[1].toLower().contains(brand))
            continue;

        if (!type.isEmpty() &&
            !car[2].toLower().contains(type))
            continue;

        if (!maxPriceText.isEmpty() &&
            car[3].toInt() > maxPriceText.toInt())
            continue;

        filtered.append(car);
    }

    showCars(filtered);
}



#ifndef CUSTOMERDASHBOARD_H
#define CUSTOMERDASHBOARD_H

#include <QMainWindow>

namespace Ui {
class customerdashboard;
}

class customerdashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit customerdashboard(QWidget *parent = nullptr);
    ~customerdashboard();

private slots:
    void on_viewCarPushButton_clicked();

    void on_myReservationPushButton_clicked();

    void on_paymentsPushButton_clicked();

    void on_LogoutPushButton_clicked();

    void on_pmyExtensionsPushButton_clicked();

private:
    Ui::customerdashboard *ui;
};

#endif // CUSTOMERDASHBOARD_H



#include "customerdashboard.h"
#include "ui_customerdashboard.h"
#include "carlistview.h"
#include "myreservationsview.h"
#include "paymentview.h"
#include "myextensionsview.h"

customerdashboard::customerdashboard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::customerdashboard)
{
    ui->setupUi(this);
}

customerdashboard::~customerdashboard()
{
    delete ui;
}

void customerdashboard::on_viewCarPushButton_clicked()
{
    carListView *view = new carListView();
    view->show();
}


void customerdashboard::on_myReservationPushButton_clicked()
{
    myReservationsView *view = new myReservationsView();
    view->show();
}


void customerdashboard::on_paymentsPushButton_clicked()
{
    paymentView *view = new paymentView();
    view->show();
}


void customerdashboard::on_LogoutPushButton_clicked()
{
    close();
}


void customerdashboard::on_pmyExtensionsPushButton_clicked()
{
    myExtensionsView *view = new myExtensionsView();
    view->show();
}




#ifndef EXTENSIONVIEW_H
#define EXTENSIONVIEW_H

#include <QWidget>
#include <QString>

namespace Ui {
class extensionView;
}

class extensionView : public QWidget
{
    Q_OBJECT

public:
    explicit extensionView(QString carId, QString oldEndDate, QWidget *parent = nullptr);
    ~extensionView();

private slots:
    void on_submitPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::extensionView *ui;
    QString carId;
    QString oldEndDate;
};

#endif // EXTENSIONVIEW_H



#include "extensionview.h"
#include "ui_extensionview.h"
#include "session.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDate>

extensionView::extensionView(QString carId,
                             QString oldEndDate,
                             QWidget *parent)
    : QWidget(parent),
    ui(new Ui::extensionView),
    carId(carId),
    oldEndDate(oldEndDate)
{
    ui->setupUi(this);
    ui->infoLabel->setText(
        "Car ID: " + carId + "\nCurrent End: " + oldEndDate
        );

    ui->newEndDateEdit->setDate(
        QDate::fromString(oldEndDate, "yyyy-MM-dd").addDays(1)
        );
}

extensionView::~extensionView()
{
    delete ui;
}

void extensionView::on_submitPushButton_clicked()
{
    QDate newDate = ui->newEndDateEdit->date();
    QDate oldDate = QDate::fromString(oldEndDate, "yyyy-MM-dd");

    if (newDate <= oldDate) {
        QMessageBox::warning(this, "Error",
                             "New date must be after current end date");
        return;
    }

    QString path = QCoreApplication::applicationDirPath()
                   + "/extensions.txt";
    QFile file(path);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error",
                             "Could not save extension request");
        return;
    }

    QTextStream out(&file);

    out << session::userId << "," << carId << ","
        << oldEndDate << ","
        << newDate.toString("yyyy-MM-dd")
        << ",Pending\n";

    file.close();

    QMessageBox::information(this, "Done",
                             "Extension request submitted");

    close();
}

void extensionView::on_cancelPushButton_clicked()
{
    close();
}




#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_loginPushButton_clicked();

    void on_registerPushButton_clicked();

private:
    Ui::LoginWindow *ui;
};
#endif // LOGINWINDOW_H



#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "customerdashboard.h"
#include "session.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QCoreApplication>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginPushButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->messageLabel->setText("Fill all fields");
        return;
    }

    QString path = QCoreApplication::applicationDirPath()
                   + "/users.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->messageLabel->setText("Could not open users file");
        return;
    }

    QTextStream in(&file);
    bool found = false;

    int id = -1;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() < 4)
            continue;

        if (parts[1] == username && parts[2] == password) {
            found = true;
            id = parts[0].toInt();
            break;
        }
    }

    file.close();

    if (!found) {
        ui->messageLabel->setText("Invalid username or password");
        return;
    }

    // ورود موفق
    session::userId=id;
    session::username = username;

    customerdashboard *dashboard = new customerdashboard();
    dashboard->show();
    this->close();
}

void LoginWindow::on_registerPushButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->messageLabel->setText("Fill all fields");
        return;
    }

    QString path = QCoreApplication::applicationDirPath()
                   + "/users.txt";
    QFile file(path);

    // اول چک کنیم یوزر وجود نداشته باشه
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");

            if (parts.size() >= 2 && parts[1] == username) {
                ui->messageLabel->setText("Username already exists");
                file.close();
                return;
            }
        }
        file.close();
    }

    // پیدا کردن id جدید
    int newId = 1;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            newId++;
            in.readLine();
        }
        file.close();
    }

    // ذخیره کاربر
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        ui->messageLabel->setText("Error saving user");
        return;
    }

    QTextStream out(&file);
    out << newId << ","
        << username << ","
        << password << ",customer\n";

    file.close();

    ui->messageLabel->setText("Registered successfully");
}




#ifndef MYEXTENSIONSVIEW_H
#define MYEXTENSIONSVIEW_H

#include <QWidget>

namespace Ui {
class myExtensionsView;
}

class myExtensionsView : public QWidget
{
    Q_OBJECT

public:
    explicit myExtensionsView(QWidget *parent = nullptr);
    ~myExtensionsView();

private:
    Ui::myExtensionsView *ui;
    void loadExtensions();
};

#endif // MYEXTENSIONSVIEW_H




#include "myextensionsview.h"
#include "ui_myextensionsview.h"
#include "session.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

myExtensionsView::myExtensionsView(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::myExtensionsView)
{
    ui->setupUi(this);
    loadExtensions();
}

myExtensionsView::~myExtensionsView()
{
    delete ui;
}

void myExtensionsView::loadExtensions()
{
    QString path = QCoreApplication::applicationDirPath()
    + "/extensions.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error",
                             "Could not open extensions file");
        return;
    }

    QTextStream in(&file);
    int row = 0;

    ui->extensionTableWidget->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() < 5)
            continue;

        if (parts[0].toInt() != session::userId)
            continue;

        ui->extensionTableWidget->insertRow(row);

        ui->extensionTableWidget->setItem(row, 0,
                                    new QTableWidgetItem(parts[1]));
        ui->extensionTableWidget->setItem(row, 1,
                                    new QTableWidgetItem(parts[2]));
        ui->extensionTableWidget->setItem(row, 2,
                                    new QTableWidgetItem(parts[3]));
        ui->extensionTableWidget->setItem(row, 3,
                                    new QTableWidgetItem(parts[4]));

        row++;
    }

    file.close();
}




#ifndef MYRESERVATIONSVIEW_H
#define MYRESERVATIONSVIEW_H

#include <QWidget>

namespace Ui {
class myReservationsView;
}

class myReservationsView : public QWidget
{
    Q_OBJECT

public:
    explicit myReservationsView(QWidget *parent = nullptr);
    ~myReservationsView();

private slots:
    void on_extensionPushButton_clicked();

private:
    Ui::myReservationsView *ui;
    void loadReservations();
};

#endif // MYRESERVATIONSVIEW_H




#include "myreservationsview.h"
#include "ui_myreservationsview.h"
#include "extensionview.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

myReservationsView::myReservationsView(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::myReservationsView)
{
    ui->setupUi(this);
    loadReservations();
}

myReservationsView::~myReservationsView()
{
    delete ui;
}

void myReservationsView::loadReservations()
{
    QString path = QCoreApplication::applicationDirPath()
    + "/reservations.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error",
                             "Could not open reservations file");
        return;
    }

    QTextStream in(&file);
    int row = 0;

    ui->reservationTableWidget->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() != 5)
            continue;

        // فقط رزروهای customer فعلی
        if (parts[0] != "1")
            continue;

        ui->reservationTableWidget->insertRow(row);

        for (int col = 0; col < 5; col++) {
            ui->reservationTableWidget->setItem(
                row, col,
                new QTableWidgetItem(parts[col])
                );
        }
        row++;
    }

    file.close();
}

void myReservationsView::on_extensionPushButton_clicked()
{
    int row = ui->reservationTableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Error",
                             "Select a reservation first");
        return;
    }

    QString carId =
        ui->reservationTableWidget->item(row, 1)->text();
    QString endDate =
        ui->reservationTableWidget->item(row, 3)->text();

    extensionView *view =
        new extensionView(carId, endDate);
    view->show();
}




#ifndef PAYMENTVIEW_H
#define PAYMENTVIEW_H

#include <QWidget>

namespace Ui {
class paymentView;
}

class paymentView : public QWidget
{
    Q_OBJECT

public:
    explicit paymentView(QWidget *parent = nullptr);
    ~paymentView();

private slots:
    void on_payPushButton_clicked();

    void on_backPushButton_clicked();

private:
    Ui::paymentView *ui;
    void loadPayments();
};

#endif // PAYMENTVIEW_H




#include "paymentview.h"
#include "ui_paymentview.h"
#include "session.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

paymentView::paymentView(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::paymentView)
{   
    ui->setupUi(this);
    loadPayments();
}

paymentView::~paymentView()
{
    delete ui;
}

void paymentView::loadPayments()
{
    QString path = QCoreApplication::applicationDirPath()
    + "/payments.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error",
                             "Could not open payments file");
        return;
    }

    QTextStream in(&file);
    int row = 0;

    ui->paymentTableWidget->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() != 4)
            continue;

        if (parts[0].toInt() != session::userId)
            continue;

        ui->paymentTableWidget->insertRow(row);

        ui->paymentTableWidget->setItem(row, 0,
                                  new QTableWidgetItem(parts[1]));
        ui->paymentTableWidget->setItem(row, 1,
                                  new QTableWidgetItem(parts[2]));
        ui->paymentTableWidget->setItem(row, 2,
                                  new QTableWidgetItem(parts[3]));

        row++;
    }

    file.close();
}

void paymentView::on_payPushButton_clicked()
{
    int row = ui->paymentTableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Error",
                             "Select a payment first");
        return;
    }

    QString status = ui->paymentTableWidget
                         ->item(row, 2)->text();

    if (status == "Paid") {
        QMessageBox::information(this, "Info",
                                 "Already paid");
        return;
    }

    QMessageBox::information(this, "Payment",
                             "Payment successful");

    ui->paymentTableWidget->setItem(row, 2,
                              new QTableWidgetItem("Paid"));
}

void paymentView::on_backPushButton_clicked()
{

}




#ifndef RESERVATIONVIEW_H
#define RESERVATIONVIEW_H

#include <QWidget>
#include <QString>
namespace Ui {
class reservationView;
}

class reservationView : public QWidget
{
    Q_OBJECT

public:
    explicit reservationView(QString carId, QString brand, QWidget *parent = nullptr);
    ~reservationView();

private slots:
    void on_confirmPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::reservationView *ui;
    QString carId;
};

#endif // RESERVATIONVIEW_H





#include "reservationview.h"
#include "ui_reservationview.h"
#include "session.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>

reservationView::reservationView(QString carId,
                                 QString brand,
                                 QWidget *parent)
    : QWidget(parent),
    ui(new Ui::reservationView),
    carId(carId)
{
    ui->setupUi(this);
    ui->carInfoLabel->setText("Selected Car: " + brand);

    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate().addDays(1));
}

reservationView::~reservationView()
{
    delete ui;
}

void reservationView::on_confirmPushButton_clicked()
{
    QDate start = ui->startDateEdit->date();
    QDate end = ui->endDateEdit->date();

    if (end <= start) {
        QMessageBox::warning(this, "Error",
                             "End date must be after start date");
        return;
    }

    QString path = QCoreApplication::applicationDirPath()
                   + "/reservations.txt";
    QFile file(path);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error",
                             "Could not save reservation");
        return;
    }

    QTextStream out(&file);

    out << session::userId << "," << carId << ","
        << start.toString("yyyy-MM-dd") << ","
        << end.toString("yyyy-MM-dd") << ",Pending\n";

    file.close();

    QMessageBox::information(this, "Done",
                             "Reservation request submitted");
    close();
}

void reservationView::on_cancelPushButton_clicked()
{
    close();
}




#ifndef SESSION_H
#define SESSION_H

#include <QString>

class session
{
public:
    static int userId;
    static QString username;
};

#endif // SESSION_H




#include "session.h"

int session::userId = -1;
QString session::username = "";





#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>

class userManager
{
public:
    static bool login(QString username, QString password);

    static bool registerUser(QString username, QString password);
};

#endif // USERMANAGER_H






#include "userManager.h"
#include <QFile>
#include <QTextStream>

bool userManager::registerUser(QString username, QString password)
{
    QFile file("users.txt");

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return false;

    QTextStream in(&file);

    // check if user exists
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if(parts[0] == username)
        {
            file.close();
            return false; // user exists
        }
    }

    QTextStream out(&file);
    out << username << "|" << password << "\n";

    file.close();
    return true;
}

bool userManager::login(QString username, QString password)
{
    QFile file("users.txt");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if(parts.size() >= 2 &&
            parts[0] == username &&
            parts[1] == password)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}
