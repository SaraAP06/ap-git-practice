#include "UserManager.h"
#include <QFile>
#include <QTextStream>

bool UserManager::registerUser(QString username, QString password)
{
    QFile file("users.txt");

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return false;

    QTextStream in(&file);

    // check if user exists
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if(parts[0] == username)
        {
            file.close();
            return false; // user exists
        }
    }

    QTextStream out(&file);
    out << username << "|" << password << "\n";

    file.close();
    return true;
}

bool UserManager::loginUser(QString username, QString password)
{
    QFile file("users.txt");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if(parts.size() >= 2 &&
           parts[0] == username &&
           parts[1] == password)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}
