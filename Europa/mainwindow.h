#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QPixmap>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>
#include <QComboBox>

#include <iostream>

#include "connection.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:
		void login();
		void sendQuery();
		void requestEnrollment();
		void logout();
		void recalculateRestrictions();
		void drawSettings(QListWidgetItem*);
		void novaTurma();
		void deletarTurma();
		void alocarProfessor();
		void desalocarProfessor();

	private:
		Ui::MainWindow *ui;
		Connection connection;

		void drawLogin();
		void drawInterface();
		void drawAdminControls();
		void drawCoordenadorControls();
		void drawProfessorControls();
		void drawAlunoControls();
};

#endif // MAINWINDOW_H
