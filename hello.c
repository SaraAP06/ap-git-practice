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
    int row = 0;

    ui->paymentTableWidget->setRowCount(0);
    ui->paymentTableWidget->setColumnCount(3);

    QStringList headers;
    headers << "Car ID" << "Amount" << "Status";
    ui->paymentTableWidget->setHorizontalHeaderLabels(headers);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList parts = line.split(",");

        if (parts.size() != 4)
            continue;

        ui->paymentTableWidget->insertRow(row);

        ui->paymentTableWidget->setItem(row, 0,
            new QTableWidgetItem(parts[1]));
        ui->paymentTableWidget->setItem(row, 1,
            new QTableWidgetItem(parts[2]));
        ui->paymentTableWidget->setItem(row, 2,
            new QTableWidgetItem(parts[3]));

        row++;
    }

    file.close();
}
