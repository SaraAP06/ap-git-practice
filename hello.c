QString user = ui->usernameEdit->text();
QString pass = ui->passwordEdit->text();

if(UserManager::login(user, pass))
{
    Session::currentUser = user;

    QMessageBox::information(this,"Login","Success");

    // برو صفحه اصلی
}
else
{
    QMessageBox::warning(this,"Login","Wrong username or password");
}
