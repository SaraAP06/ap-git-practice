#include "usermanager.h"
#include <QFile>
#include <QTextStream>

bool UserManager::login(QString username, QString password)
{
    QFile file("users.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString u, p;
        in >> u >> p;

        if(u == username && p == password)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool UserManager::registerUser(QString username, QString password)
{
    QFile file("users.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    // check exists
    while(!in.atEnd())
    {
        QString u, p;
        in >> u >> p;

        if(u == username)
        {
            file.close();
            return false;
        }
    }

    file.close();

    // add new user
    if(!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << username << " " << password << "\n";

    file.close();
    return true;
}
