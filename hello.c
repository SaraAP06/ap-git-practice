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

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() != 4)
            continue;

        //if (parts[0] != "1")
           // continue;

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




1,3,120,Unpaid
1,1,50,Paid
