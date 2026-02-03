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

    // 🔴 ست کامل جدول
    ui->paymentTableWidget->clear();
    ui->paymentTableWidget->setRowCount(0);
    ui->paymentTableWidget->setColumnCount(4);

    QStringList headers;
    headers << "Customer ID" << "Car ID" << "Amount" << "Status";
    ui->paymentTableWidget->setHorizontalHeaderLabels(headers);

    QTextStream in(&file);
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

    ui->paymentTableWidget->resizeColumnsToContents();
}
