#include "myextensionsview.h"
#include "ui_myextensionsview.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QCoreApplication>

MyExtensionsView::MyExtensionsView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MyExtensionsView)
{
    ui->setupUi(this);
    loadExtensions();
}

MyExtensionsView::~MyExtensionsView()
{
    delete ui;
}

void MyExtensionsView::loadExtensions()
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

    ui->extensionTable->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() < 5)
            continue;

        if (parts[0] != "1")
            continue;

        ui->extensionTable->insertRow(row);

        ui->extensionTable->setItem(row, 0,
            new QTableWidgetItem(parts[1]));
        ui->extensionTable->setItem(row, 1,
            new QTableWidgetItem(parts[2]));
        ui->extensionTable->setItem(row, 2,
            new QTableWidgetItem(parts[3]));
        ui->extensionTable->setItem(row, 3,
            new QTableWidgetItem(parts[4]));

        row++;
    }

    file.close();
}
