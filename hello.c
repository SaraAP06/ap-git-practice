QFile file("reservations.txt");
file.open(QIODevice::Append | QIODevice::Text);
QTextStream out(&file);

out << Session::currentUser << "|"
    << carId << "|"
    << date << "|pending\n";

file.close();
