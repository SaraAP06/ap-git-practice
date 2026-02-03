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
    ui->paymentTableWidget->setRowCount(0);

    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList parts = line.split(",");
        if (parts.size() != 4)
            continue;

        ui->paymentTableWidget->insertRow(row);

        for (int col = 0; col < 4; col++) {
            ui->paymentTableWidget->setItem(
                row,
                col,
                new QTableWidgetItem(parts[col])
                );
        }
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


