void MyReservationsView::on_extensionButton_clicked()
{
    int row = ui->reservationTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Error",
            "Select a reservation first");
        return;
    }

    QString carId =
        ui->reservationTable->item(row, 1)->text();
    QString endDate =
        ui->reservationTable->item(row, 3)->text();

    ExtensionView *view =
        new ExtensionView(carId, endDate);
    view->show();
}
