#include "myreservationsview.h"
#include "ui_myreservationsview.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

MyReservationsView::MyReservationsView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MyReservationsView)
{
    ui->setupUi(this);
    loadReservations();
}

MyReservationsView::~MyReservationsView()
{
    delete ui;
}

void MyReservationsView::loadReservations()
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

    ui->reservationTable->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() != 5)
            continue;

        // فقط رزروهای customer فعلی
        if (parts[0] != "1")
            continue;

        ui->reservationTable->insertRow(row);

        for (int col = 0; col < 5; col++) {
            ui->reservationTable->setItem(
                row, col,
                new QTableWidgetItem(parts[col])
            );
        }
        row++;
    }

    file.close();
}
