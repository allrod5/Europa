#include "connection.h"

Connection::Connection()
{
	db = QSqlDatabase::addDatabase("QMYSQL");
}

bool Connection::createConnection(QString user, QString password){
	this->user = user;
	QString usertype;
	if(getUserType(usertype, user, password)) {
		db.setHostName("localhost");
		db.setDatabaseName("matriculasrff");
		db.setUserName(usertype);
		db.setPassword(usertype);
		if (!db.open()) {
			QMessageBox msgBox;
			msgBox.setText("Não foi possível estabelecer uma conexão com o banco de dados.\nCertifique-se de que você possui os drivers necessários.");
			msgBox.exec();
			return false;
		}
		return true;
	}
	QMessageBox msgBox;
	msgBox.setText("O usuário não existe ou a senha está incorreta!");
	msgBox.exec();
	return false;
}

QString Connection::userName()
{
	return db.userName();
}

bool Connection::getUserType(QString &usertype, QString &user, QString &password)
{
	if(user.length()==0)
		return false;

	QFile file(":/userlist.txt");

	if(!file.open(QIODevice::ReadOnly)) {
		qWarning("Failed to verify user.");
		return false;
	}

	QTextStream stream(&file);

	QString u, p, t;

	while(!stream.atEnd()) {
		stream >> u >> p >> t;
		if(!QString::compare(user, u, Qt::CaseInsensitive) && !QString::compare(password, p, Qt::CaseSensitive)) {
			usertype = t;
			return true;
		}
	}

	return false;
}
