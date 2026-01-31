#include "carlistview.h"
#include "ui_carlistview.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

CarListView::CarListView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::CarListView)
{
    ui->setupUi(this);

    // تنظیمات ساده جدول
    ui->carTable->setColumnCount(5);
    ui->carTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Brand" << "Type" << "Price" << "Status"
    );
    ui->carTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->carTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->carTable->setSelectionMode(QAbstractItemView::SingleSelection);

    loadCars();
}

CarListView::~CarListView()
{
    delete ui;
}

void CarListView::loadCars()
{
    QString path = QCoreApplication::applicationDirPath() + "/cars.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error",
                             "Could not open cars.txt\nPath:\n" + path);
        return;
    }

    QTextStream in(&file);
    ui->carTable->setRowCount(0);

    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList parts = line.split(",");
        if (parts.size() != 5)
            continue;

        ui->carTable->insertRow(row);

        for (int col = 0; col < 5; col++) {
            ui->carTable->setItem(
                row,
                col,
                new QTableWidgetItem(parts[col])
            );
        }
        row++;
    }

    file.close();
}
