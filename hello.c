void CarListView::on_filterButton_clicked()
{
    QString brand = ui->brandLineEdit->text().toLower();
    QString type = ui->typeLineEdit->text().toLower();
    QString maxPriceText = ui->maxPriceLineEdit->text();

    QList<QStringList> filtered;

    for (int i = 0; i < allCars.size(); i++) {
        QStringList car = allCars[i];

        if (!brand.isEmpty() &&
            !car[1].toLower().contains(brand))
            continue;

        if (!type.isEmpty() &&
            !car[2].toLower().contains(type))
            continue;

        if (!maxPriceText.isEmpty() &&
            car[3].toInt() > maxPriceText.toInt())
            continue;

        filtered.append(car);
    }

    showCars(filtered);
}
