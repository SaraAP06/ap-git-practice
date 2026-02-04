#include "extensionview.h"
#include "ui_extensionview.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDate>

ExtensionView::ExtensionView(QString carId,
                             QString oldEndDate,
                             QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ExtensionView),
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

ExtensionView::~ExtensionView()
{
    delete ui;
}

void ExtensionView::on_submitButton_clicked()
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

void ExtensionView::on_cancelButton_clicked()
{
    close();
}
