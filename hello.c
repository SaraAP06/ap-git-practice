int row = ui->carTable->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a car");
        return;
    }

    QString status = ui->carTable->item(row, 4)->text();
    if (status != "Available") {
        QMessageBox::warning(this, "Error", "Car is not available");
        return;
    }

    QString carId = ui->carTable->item(row, 0)->text();
    QString brand = ui->carTable->item(row, 1)->text();

    ReservationView *view = new ReservationView(carId, brand);
    view->show();
}
