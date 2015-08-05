#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QMessageBox>

class Connection
{
	public:
		Connection();
		bool createConnection(QString, QString);
		QString userName();
		QString user;
	private:
		QSqlDatabase db;
		bool getUserType(QString&, QString&, QString&);
};

#endif // CONNECTION_H
