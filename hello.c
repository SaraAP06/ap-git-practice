void LoginWindow::on_loginButton_clicked()
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

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->messageLabel->setText("Could not open users file");
        return;
    }

    QTextStream in(&file);
    bool found = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() < 4)
            continue;

        if (parts[1] == username && parts[2] == password) {
            found = true;
            break;
        }
    }

    file.close();

    if (!found) {
        ui->messageLabel->setText("Invalid username or password");
        return;
    }

    // ورود موفق
    CustomerDashboard *dashboard = new CustomerDashboard();
    dashboard->show();
    this->close();
}
