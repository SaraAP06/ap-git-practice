#include "paymentview.h"
#include "ui_paymentview.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

PaymentView::PaymentView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PaymentView)
{
    ui->setupUi(this);
    loadPayments();
}

PaymentView::~PaymentView()
{
    delete ui;
}

void PaymentView::loadPayments()
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

    ui->paymentTable->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() != 4)
            continue;

        if (parts[0] != "1")
            continue;

        ui->paymentTable->insertRow(row);

        ui->paymentTable->setItem(row, 0,
            new QTableWidgetItem(parts[1]));
        ui->paymentTable->setItem(row, 1,
            new QTableWidgetItem(parts[2]));
        ui->paymentTable->setItem(row, 2,
            new QTableWidgetItem(parts[3]));

        row++;
    }

    file.close();
}
