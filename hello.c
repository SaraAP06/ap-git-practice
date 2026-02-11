void LoginWindow::on_registerButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->messageLabel->setText("Fill all fields");
        return;
    }

    QString path = QCoreApplication::applicationDirPath()
                   + "/users.txt";
    QFile file(path);

    // اول چک کنیم یوزر وجود نداشته باشه
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");

            if (parts.size() >= 2 && parts[1] == username) {
                ui->messageLabel->setText("Username already exists");
                file.close();
                return;
            }
        }
        file.close();
    }

    // پیدا کردن id جدید
    int newId = 1;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            newId++;
            in.readLine();
        }
        file.close();
    }

    // ذخیره کاربر
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        ui->messageLabel->setText("Error saving user");
        return;
    }

    QTextStream out(&file);
    out << newId << ","
        << username << ","
        << password << ",customer\n";

    file.close();

    ui->messageLabel->setText("Registered successfully");
}
