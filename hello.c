void PaymentView::on_payButton_clicked()
{
    int row = ui->paymentTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Error",
                             "Select a payment first");
        return;
    }

    QString status = ui->paymentTable
                     ->item(row, 2)->text();

    if (status == "Paid") {
        QMessageBox::information(this, "Info",
                                 "Already paid");
        return;
    }

    QMessageBox::information(this, "Payment",
                             "Payment successful");

    ui->paymentTable->setItem(row, 2,
        new QTableWidgetItem("Paid"));
}
