#include "carlistview.h"
#include "ui_carlistview.h"

CarListView::CarListView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::CarListView)
{
    ui->setupUi(this);
    loadCars();
}

CarListView::~CarListView()
{
    delete ui;
}

void CarListView::loadCars()
{
    ui->carTable->setRowCount(3);

    ui->carTable->setItem(0, 0, new QTableWidgetItem("1"));
    ui->carTable->setItem(0, 1, new QTableWidgetItem("Toyota"));
    ui->carTable->setItem(0, 2, new QTableWidgetItem("Sedan"));
    ui->carTable->setItem(0, 3, new QTableWidgetItem("50"));
    ui->carTable->setItem(0, 4, new QTableWidgetItem("Available"));

    ui->carTable->setItem(1, 0, new QTableWidgetItem("2"));
    ui->carTable->setItem(1, 1, new QTableWidgetItem("BMW"));
    ui->carTable->setItem(1, 2, new QTableWidgetItem("SUV"));
    ui->carTable->setItem(1, 3, new QTableWidgetItem("90"));
    ui->carTable->setItem(1, 4, new QTableWidgetItem("Rented"));

    ui->carTable->setItem(2, 0, new QTableWidgetItem("3"));
    ui->carTable->setItem(2, 1, new QTableWidgetItem("Kia"));
    ui->carTable->setItem(2, 2, new QTableWidgetItem("Hatchback"));
    ui->carTable->setItem(2, 3, new QTableWidgetItem("40"));
    ui->carTable->setItem(2, 4, new QTableWidgetItem("Available"));
}
