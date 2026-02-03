void CarListView::loadCarsFromFile()
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



void CarListView::showCars(const QList<QStringList>& cars)
{
    ui->carTable->setRowCount(0);

    for (int i = 0; i < cars.size(); i++) {
        ui->carTable->insertRow(i);
        for (int j = 0; j < 5; j++) {
            ui->carTable->setItem(i, j,
                new QTableWidgetItem(cars[i][j]));
        }
    }
}
