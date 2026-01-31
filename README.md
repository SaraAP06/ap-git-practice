void CarListView::loadCars()
{
    QFile file("cars.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open cars file");
        return;
    }

    QTextStream in(&file);
    int row = 0;

    ui->carTable->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() != 5)
            continue;

        ui->carTable->insertRow(row);

        for (int col = 0; col < 5; col++) {
            ui->carTable->setItem(row, col,
                new QTableWidgetItem(parts[col]));
        }

        row++;
    }

    file.close();
}
