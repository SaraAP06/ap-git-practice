#include "reservationview.h"
#include "ui_reservationview.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>

ReservationView::ReservationView(QString carId,
                                 QString brand,
                                 QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ReservationView),
      carId(carId)
{
    ui->setupUi(this);
    ui->carInfoLabel->setText("Selected Car: " + brand);

    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate().addDays(1));
}

ReservationView::~ReservationView()
{
    delete ui;
}

void ReservationView::on_confirmButton_clicked()
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

    // فرمت:
    // customerId,carId,startDate,endDate,status
    out << "1," << carId << ","
        << start.toString("yyyy-MM-dd") << ","
        << end.toString("yyyy-MM-dd") << ",Pending\n";

    file.close();

    QMessageBox::information(this, "Done",
                             "Reservation request submitted");
    close();
}

void ReservationView::on_cancelButton_clicked()
{
    close();
}
