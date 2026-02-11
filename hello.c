#ifndef CARLISTVIEW_H
#define CARLISTVIEW_H

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




<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>carListView</class>
 <widget class="QWidget" name="carListView">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="font">
   <font>
    <pointsize>12</pointsize>
    <italic>true</italic>
   </font>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <widget class="QPushButton" name="reservePushButton">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>460</y>
     <width>83</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Reserve</string>
   </property>
  </widget>
  <widget class="QPushButton" name="backPushButton">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>510</y>
     <width>83</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Back</string>
   </property>
  </widget>
  <widget class="QLineEdit" name="brandLineEdit">
   <property name="geometry">
    <rect>
     <x>310</x>
     <y>10</y>
     <width>113</width>
     <height>28</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>13</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string/>
   </property>
  </widget>
  <widget class="QLineEdit" name="typeLineEdit">
   <property name="geometry">
    <rect>
     <x>310</x>
     <y>60</y>
     <width>113</width>
     <height>28</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>13</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string/>
   </property>
  </widget>
  <widget class="QLineEdit" name="maxPriceLineEdit">
   <property name="geometry">
    <rect>
     <x>310</x>
     <y>110</y>
     <width>113</width>
     <height>28</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>13</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string/>
   </property>
  </widget>
  <widget class="QPushButton" name="filterPushButton">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>160</y>
     <width>83</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Filter</string>
   </property>
  </widget>
  <widget class="QLabel" name="brandLabel">
   <property name="geometry">
    <rect>
     <x>200</x>
     <y>10</y>
     <width>81</width>
     <height>21</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>13</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Brand</string>
   </property>
  </widget>
  <widget class="QLabel" name="typeLabel">
   <property name="geometry">
    <rect>
     <x>200</x>
     <y>50</y>
     <width>51</width>
     <height>41</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>13</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Type</string>
   </property>
  </widget>
  <widget class="QLabel" name="maxPriceLabel">
   <property name="geometry">
    <rect>
     <x>200</x>
     <y>110</y>
     <width>101</width>
     <height>21</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>13</pointsize>
     <italic>true</italic>
     <bold>false</bold>
     <underline>false</underline>
     <strikeout>false</strikeout>
     <kerning>true</kerning>
    </font>
   </property>
   <property name="text">
    <string>Max Price</string>
   </property>
  </widget>
  <widget class="QTableWidget" name="CarTableWidget">
   <property name="geometry">
    <rect>
     <x>20</x>
     <y>220</y>
     <width>721</width>
     <height>221</height>
    </rect>
   </property>
   <property name="maximumSize">
    <size>
     <width>721</width>
     <height>221</height>
    </size>
   </property>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <column>
    <property name="text">
     <string>New Column</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Brand</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Price</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>New Column</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Id</string>
    </property>
   </column>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>






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



<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>customerdashboard</class>
 <widget class="QWidget" name="customerdashboard">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <widget class="QLabel" name="welcomeLabel">
   <property name="geometry">
    <rect>
     <x>270</x>
     <y>80</y>
     <width>181</width>
     <height>61</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>25</pointsize>
     <italic>true</italic>
     <bold>true</bold>
    </font>
   </property>
   <property name="text">
    <string>welcome</string>
   </property>
  </widget>
  <widget class="QPushButton" name="viewCarPushButton">
   <property name="geometry">
    <rect>
     <x>270</x>
     <y>210</y>
     <width>171</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>View Cars</string>
   </property>
  </widget>
  <widget class="QPushButton" name="myReservationPushButton">
   <property name="geometry">
    <rect>
     <x>270</x>
     <y>280</y>
     <width>171</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>My Reservation</string>
   </property>
  </widget>
  <widget class="QPushButton" name="paymentsPushButton">
   <property name="geometry">
    <rect>
     <x>270</x>
     <y>340</y>
     <width>171</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>Payments</string>
   </property>
  </widget>
  <widget class="QPushButton" name="LogoutPushButton">
   <property name="geometry">
    <rect>
     <x>270</x>
     <y>480</y>
     <width>171</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>Logout</string>
   </property>
  </widget>
  <widget class="QPushButton" name="pmyExtensionsPushButton">
   <property name="geometry">
    <rect>
     <x>270</x>
     <y>410</y>
     <width>171</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>My Extensions</string>
   </property>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>




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

    out << "1," << carId << ","
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




<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>extensionView</class>
 <widget class="QWidget" name="extensionView">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <widget class="QLabel" name="infoLabel">
   <property name="geometry">
    <rect>
     <x>260</x>
     <y>70</y>
     <width>271</width>
     <height>51</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <bold>true</bold>
    </font>
   </property>
   <property name="text">
    <string>Request Extension</string>
   </property>
  </widget>
  <widget class="QDateEdit" name="newEndDateEdit">
   <property name="geometry">
    <rect>
     <x>300</x>
     <y>230</y>
     <width>131</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
    </font>
   </property>
  </widget>
  <widget class="QPushButton" name="submitPushButton">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>390</y>
     <width>83</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Submit</string>
   </property>
  </widget>
  <widget class="QPushButton" name="cancelPushButton">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>460</y>
     <width>83</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Cancel</string>
   </property>
  </widget>
  <widget class="QLabel" name="label">
   <property name="geometry">
    <rect>
     <x>290</x>
     <y>170</y>
     <width>161</width>
     <height>41</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>New End Date</string>
   </property>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>





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
#include <QMessageBox>

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
    QString username =
        ui->usernameLineEdit->text();
    QString password =
        ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty())
    {
        ui->messageLabel->setText("Please fill all fields.");
        return;
    }
    //فعلا فیک بعدا وصل میشه به manager
    if (username == "test" && password == "1234")
    {
        QMessageBox::information(this, "Login", "Login successful");
        customerdashboard *dashboard = new customerdashboard();
        dashboard->show();
        this->close();
    }
    else
    {
        ui->messageLabel->setText("Invalid username or password.");
    }
}

void LoginWindow::on_registerPushButton_clicked()
{
    QMessageBox::information(this, "Register", "Register page not implemented yet");
}



<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>LoginWindow</class>
 <widget class="QMainWindow" name="LoginWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="font">
   <font>
    <pointsize>6</pointsize>
    <italic>false</italic>
    <bold>false</bold>
    <underline>false</underline>
    <strikeout>false</strikeout>
    <stylestrategy>PreferDefault</stylestrategy>
    <hintingpreference>PreferDefaultHinting</hintingpreference>
   </font>
  </property>
  <property name="mouseTracking">
   <bool>false</bool>
  </property>
  <property name="tabletTracking">
   <bool>false</bool>
  </property>
  <property name="focusPolicy">
   <enum>Qt::NoFocus</enum>
  </property>
  <property name="windowTitle">
   <string>MainWindow</string>
  </property>
  <property name="toolButtonStyle">
   <enum>Qt::ToolButtonTextOnly</enum>
  </property>
  <widget class="QWidget" name="centralwidget">
   <widget class="QLabel" name="UsernameLabel">
    <property name="geometry">
     <rect>
      <x>300</x>
      <y>60</y>
      <width>111</width>
      <height>41</height>
     </rect>
    </property>
    <property name="font">
     <font>
      <pointsize>15</pointsize>
      <italic>true</italic>
      <bold>false</bold>
      <underline>false</underline>
      <strikeout>false</strikeout>
      <stylestrategy>PreferDefault</stylestrategy>
      <hintingpreference>PreferDefaultHinting</hintingpreference>
     </font>
    </property>
    <property name="text">
     <string>Username</string>
    </property>
   </widget>
   <widget class="QLineEdit" name="usernameLineEdit">
    <property name="geometry">
     <rect>
      <x>300</x>
      <y>110</y>
      <width>113</width>
      <height>28</height>
     </rect>
    </property>
    <property name="font">
     <font>
      <pointsize>11</pointsize>
      <italic>false</italic>
      <bold>false</bold>
      <underline>false</underline>
      <strikeout>false</strikeout>
      <stylestrategy>PreferDefault</stylestrategy>
      <hintingpreference>PreferDefaultHinting</hintingpreference>
     </font>
    </property>
    <property name="focusPolicy">
     <enum>Qt::StrongFocus</enum>
    </property>
    <property name="accessibleDescription">
     <string>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; color:#c595c8;&quot;&gt;#c595c8&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</string>
    </property>
   </widget>
   <widget class="QLabel" name="PasswordLabel">
    <property name="geometry">
     <rect>
      <x>300</x>
      <y>190</y>
      <width>111</width>
      <height>41</height>
     </rect>
    </property>
    <property name="font">
     <font>
      <pointsize>15</pointsize>
      <italic>true</italic>
      <bold>false</bold>
      <underline>false</underline>
      <strikeout>false</strikeout>
      <stylestrategy>PreferDefault</stylestrategy>
      <hintingpreference>PreferDefaultHinting</hintingpreference>
     </font>
    </property>
    <property name="toolTipDuration">
     <number>-1</number>
    </property>
    <property name="text">
     <string>Password</string>
    </property>
   </widget>
   <widget class="QLineEdit" name="passwordLineEdit">
    <property name="geometry">
     <rect>
      <x>300</x>
      <y>240</y>
      <width>113</width>
      <height>28</height>
     </rect>
    </property>
    <property name="font">
     <font>
      <pointsize>11</pointsize>
      <italic>false</italic>
      <bold>false</bold>
      <underline>false</underline>
      <strikeout>false</strikeout>
      <stylestrategy>PreferDefault</stylestrategy>
      <hintingpreference>PreferDefaultHinting</hintingpreference>
     </font>
    </property>
    <property name="focusPolicy">
     <enum>Qt::StrongFocus</enum>
    </property>
    <property name="echoMode">
     <enum>QLineEdit::Password</enum>
    </property>
   </widget>
   <widget class="QPushButton" name="loginPushButton">
    <property name="geometry">
     <rect>
      <x>310</x>
      <y>370</y>
      <width>83</width>
      <height>29</height>
     </rect>
    </property>
    <property name="font">
     <font>
      <pointsize>12</pointsize>
      <italic>true</italic>
      <bold>false</bold>
      <underline>false</underline>
      <strikeout>false</strikeout>
      <stylestrategy>PreferDefault</stylestrategy>
      <hintingpreference>PreferDefaultHinting</hintingpreference>
     </font>
    </property>
    <property name="focusPolicy">
     <enum>Qt::StrongFocus</enum>
    </property>
    <property name="text">
     <string>Login</string>
    </property>
   </widget>
   <widget class="QPushButton" name="registerPushButton">
    <property name="geometry">
     <rect>
      <x>310</x>
      <y>420</y>
      <width>83</width>
      <height>29</height>
     </rect>
    </property>
    <property name="font">
     <font>
      <pointsize>12</pointsize>
      <italic>true</italic>
      <bold>false</bold>
      <underline>false</underline>
      <strikeout>false</strikeout>
      <stylestrategy>PreferDefault</stylestrategy>
      <hintingpreference>PreferDefaultHinting</hintingpreference>
     </font>
    </property>
    <property name="focusPolicy">
     <enum>Qt::StrongFocus</enum>
    </property>
    <property name="text">
     <string>Register</string>
    </property>
   </widget>
   <widget class="QLabel" name="messageLabel">
    <property name="geometry">
     <rect>
      <x>310</x>
      <y>460</y>
      <width>91</width>
      <height>41</height>
     </rect>
    </property>
    <property name="font">
     <font>
      <pointsize>13</pointsize>
      <italic>true</italic>
      <bold>false</bold>
      <underline>false</underline>
      <strikeout>false</strikeout>
      <stylestrategy>PreferDefault</stylestrategy>
      <hintingpreference>PreferDefaultHinting</hintingpreference>
     </font>
    </property>
    <property name="text">
     <string>Message</string>
    </property>
   </widget>
  </widget>
  <widget class="QMenuBar" name="menubar">
   <property name="geometry">
    <rect>
     <x>0</x>
     <y>0</y>
     <width>760</width>
     <height>19</height>
    </rect>
   </property>
  </widget>
  <widget class="QStatusBar" name="statusbar"/>
 </widget>
 <resources/>
 <connections/>
</ui>




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

        if (parts[0] != "1")
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


<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>myExtensionsView</class>
 <widget class="QWidget" name="myExtensionsView">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <widget class="QTableWidget" name="extensionTableWidget">
   <property name="geometry">
    <rect>
     <x>20</x>
     <y>180</y>
     <width>721</width>
     <height>221</height>
    </rect>
   </property>
   <property name="maximumSize">
    <size>
     <width>721</width>
     <height>221</height>
    </size>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <column>
    <property name="text">
     <string>Car ID</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Old End Date</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>New End Date</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Status</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Note</string>
    </property>
   </column>
  </widget>
  <widget class="QPushButton" name="backPushButton">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>480</y>
     <width>83</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>Back</string>
   </property>
  </widget>
  <widget class="QLabel" name="label">
   <property name="geometry">
    <rect>
     <x>290</x>
     <y>70</y>
     <width>171</width>
     <height>31</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <bold>true</bold>
    </font>
   </property>
   <property name="text">
    <string>My Extensions</string>
   </property>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>



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




<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>myReservationsView</class>
 <widget class="QWidget" name="myReservationsView">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <widget class="QTableWidget" name="reservationTableWidget">
   <property name="geometry">
    <rect>
     <x>20</x>
     <y>160</y>
     <width>721</width>
     <height>221</height>
    </rect>
   </property>
   <property name="maximumSize">
    <size>
     <width>721</width>
     <height>221</height>
    </size>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <row>
    <property name="text">
     <string>New Row</string>
    </property>
   </row>
   <column>
    <property name="text">
     <string>Customer ID</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Car ID</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Start Date</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>End Date</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Status</string>
    </property>
   </column>
  </widget>
  <widget class="QPushButton" name="backPushButton">
   <property name="geometry">
    <rect>
     <x>280</x>
     <y>480</y>
     <width>170</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Back</string>
   </property>
  </widget>
  <widget class="QPushButton" name="extensionPushButton">
   <property name="geometry">
    <rect>
     <x>280</x>
     <y>430</y>
     <width>171</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Request Extension</string>
   </property>
  </widget>
  <widget class="QLabel" name="label">
   <property name="geometry">
    <rect>
     <x>270</x>
     <y>80</y>
     <width>191</width>
     <height>41</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>true</bold>
    </font>
   </property>
   <property name="text">
    <string>My Reservations</string>
   </property>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>




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
#include "paymentview.h"
#include "ui_paymentview.h"
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

        if (parts[0] != "1")
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



<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>paymentView</class>
 <widget class="QWidget" name="paymentView">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <widget class="QTableWidget" name="paymentTableWidget">
   <property name="geometry">
    <rect>
     <x>120</x>
     <y>180</y>
     <width>521</width>
     <height>181</height>
    </rect>
   </property>
   <property name="maximumSize">
    <size>
     <width>521</width>
     <height>181</height>
    </size>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <row/>
   <row/>
   <row/>
   <row/>
   <column>
    <property name="text">
     <string>Car ID</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Amount</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Status</string>
    </property>
   </column>
   <column>
    <property name="text">
     <string>Action</string>
    </property>
   </column>
  </widget>
  <widget class="QPushButton" name="payPushButton">
   <property name="geometry">
    <rect>
     <x>310</x>
     <y>430</y>
     <width>151</width>
     <height>31</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>Pay Selected</string>
   </property>
  </widget>
  <widget class="QPushButton" name="backPushButton">
   <property name="geometry">
    <rect>
     <x>310</x>
     <y>480</y>
     <width>151</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
     <bold>false</bold>
    </font>
   </property>
   <property name="text">
    <string>Back</string>
   </property>
  </widget>
  <widget class="QLabel" name="paymentsLabel">
   <property name="geometry">
    <rect>
     <x>310</x>
     <y>80</y>
     <width>121</width>
     <height>41</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>true</bold>
    </font>
   </property>
   <property name="text">
    <string>Payments</string>
   </property>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>






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

#include <QStringList>
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

void updateCarStatus(const QString &carId)
{
    QString path = QCoreApplication::applicationDirPath() + "/cars.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QStringList lines;
    QTextStream in(&file);

    while (!in.atEnd())
        lines.append(in.readLine());

    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (int i = 0; i < lines.size(); i++)
    {
        QStringList parts = lines[i].split(",");

        if (parts.size() == 5 && parts[0] == carId)
            parts[4] = "Reserved";

        out << parts.join(",") << "\n";
    }

    file.close();
}

void createPayment(const QString &carId)
{
    QString carPath = QCoreApplication::applicationDirPath() + "/cars.txt";
    QFile carFile(carPath);

    if (!carFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&carFile);
    int price = 0;

    while (!in.atEnd())
    {
        QStringList parts = in.readLine().split(",");
        if (parts.size() == 5 && parts[0] == carId)
        {
            price = parts[3].toInt();
            break;
        }
    }

    carFile.close();

    QString payPath = QCoreApplication::applicationDirPath() + "/payments.txt";
    QFile payFile(payPath);

    if (!payFile.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&payFile);
    out << "1," << carId << "," << price << ",Unpaid\n";

    payFile.close();
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

    out << "1," << carId << ","
        << start.toString("yyyy-MM-dd") << ","
        << end.toString("yyyy-MM-dd") << ",Pending\n";

    file.close();

    // تغییر وضعیت ماشین
    updateCarStatus(carId);

    // ساخت Payment
    createPayment(carId);

    QMessageBox::information(this, "Done",
                             "Reservation submitted & Car reserved");

    close();
}

void reservationView::on_cancelPushButton_clicked()
{

}






<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>reservationView</class>
 <widget class="QWidget" name="reservationView">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>760</width>
    <height>560</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <widget class="QLabel" name="carInfoLabel">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>160</y>
     <width>111</width>
     <height>20</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Selected Car</string>
   </property>
  </widget>
  <widget class="QDateEdit" name="startDateEdit">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>240</y>
     <width>108</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
  </widget>
  <widget class="QDateEdit" name="endDateEdit">
   <property name="geometry">
    <rect>
     <x>320</x>
     <y>310</y>
     <width>110</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
  </widget>
  <widget class="QPushButton" name="confirmPushButton">
   <property name="geometry">
    <rect>
     <x>280</x>
     <y>400</y>
     <width>191</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Confirm Reservation</string>
   </property>
  </widget>
  <widget class="QPushButton" name="cancelPushButton">
   <property name="geometry">
    <rect>
     <x>280</x>
     <y>450</y>
     <width>191</width>
     <height>29</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Cancel</string>
   </property>
  </widget>
  <widget class="QLabel" name="reservationLabel">
   <property name="geometry">
    <rect>
     <x>310</x>
     <y>60</y>
     <width>141</width>
     <height>41</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>15</pointsize>
     <italic>true</italic>
     <bold>true</bold>
    </font>
   </property>
   <property name="text">
    <string>Reservation</string>
   </property>
  </widget>
  <widget class="QLabel" name="label">
   <property name="geometry">
    <rect>
     <x>240</x>
     <y>240</y>
     <width>63</width>
     <height>20</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>Start</string>
   </property>
  </widget>
  <widget class="QLabel" name="label_2">
   <property name="geometry">
    <rect>
     <x>240</x>
     <y>310</y>
     <width>63</width>
     <height>20</height>
    </rect>
   </property>
   <property name="font">
    <font>
     <pointsize>12</pointsize>
     <italic>true</italic>
    </font>
   </property>
   <property name="text">
    <string>End</string>
   </property>
  </widget>
 </widget>
 <resources/>
 <connections/>
</ui>
