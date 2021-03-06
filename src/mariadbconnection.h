#ifndef MARIADB_H
#define MARIADB_H
#include <QString>
#include <QSqlDatabase>


class MariaDBConnection
{
private:
    QSqlDatabase db;
public:

    MariaDBConnection();
    MariaDBConnection(QString dbtype, QString hostname,QString dbname, QString username, QString pass,int port);
    int Login(QString login,QString pass);
    bool AddPassword(int Owner_ID,QString Hashed_Password,QString Destination,QString Destination_User);
    bool CheckLogin(QString login);
    bool RegisterUser(QString login, QString HashedPassword);
    bool RemovePassword(int Owner_ID, int ID);
    QList<QString>* listPasswords(QString Login, QString Password);

    bool ModifyPassword(QString login,QString pass, QString Hashed_Password, QString Destination, QString Destination_User, int Pass_ID);
};

#endif // MARIADB_H
