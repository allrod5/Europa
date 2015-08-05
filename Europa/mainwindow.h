#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QWidget>
#include <QTextEdit>
#include <QListWidget>

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
		void establishConnection();
		void sendQuery();
		void requestEnrollment();

	private:
		Ui::MainWindow *ui;
		Connection connection;

		void drawInterface();
		void drawAdminControls();
		void drawCoordenadorControls();
		void drawProfessorControls();
		void drawAlunoControls();
};

#endif // MAINWINDOW_H
