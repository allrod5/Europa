#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::establishConnection()
{
	QPalette *palette = new QPalette();
	palette->setColor(QPalette::Base,Qt::gray);
	palette->setColor(QPalette::Text,Qt::darkGray);

	ui->lineEdit->setReadOnly(true);
	ui->lineEdit->setPalette(*palette);

	ui->lineEdit_2->setReadOnly(true);
	ui->lineEdit_2->setPalette(*palette);

	QString user = ui->lineEdit->text();
	QString password = ui->lineEdit->text();

	if(connection.createConnection(user, password)) {
		drawInterface();
	} else {
		ui->lineEdit->setReadOnly(false);
		ui->lineEdit->setPalette( QApplication::palette( ui->lineEdit ) );

		ui->lineEdit_2->setReadOnly(false);
		ui->lineEdit_2->setPalette( QApplication::palette( ui->lineEdit_2 ) );
	}
}

void MainWindow::sendQuery()
{
	QSqlQuery query;
	query.exec(ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QTextEdit*>("Entrada")->toPlainText());
	QTextEdit *output = ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QTextEdit*>("Saida");

	if(!query.first()) {
		output->setHtml(query.lastError().text());
		return;
	}
	const int columns = query.record().count();
	QString table = "<table border=\"1\" CELLPADDING=\"5\" style=\"width:100%\">";
	while(true) {
		table += "<tr>";
		for(int i=0; i<columns; i++) {
			table += "<td>"+query.value(i).toString()+"<\td>";
		}
		table += "<\tr>";
		if(!query.next()) break;
	}
	output->setHtml(table);
}

void MainWindow::requestEnrollment()
{
	QSqlQuery query;
	QString text;
	query.exec("call raUsuario('"+connection.user+"')");
	query.first();
	QString RA = query.value(0).toString();
	text = "start transaction; ";
	QListWidget *list = ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QListWidget*>("listWidget");
	for(unsigned int i=0; i<list->count(); i++ ) {
		QListWidgetItem * item = list->item(i);
		if(item->checkState()) {
			text += "call fazMatricula("+RA+","+item->data(Qt::UserRole).toString()+"); ";
		}
	}
	text += "commit;";

	query.exec(text);

}

void MainWindow::drawInterface()
{
	qDeleteAll(ui->centralWidget->children());
	delete ui->centralWidget->layout();

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setObjectName("parentLayout");

	QTabWidget *tabWidget = new QTabWidget;
	tabWidget->setObjectName("tabWidget");
	layout->addWidget(tabWidget);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->setObjectName("bottomLayout");

	QPushButton *exitButton = new QPushButton;
	connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
	exitButton->setText("Sair");
	exitButton->setObjectName("exitButton");
	bottomLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
	bottomLayout->addWidget(exitButton);

	layout->addLayout(bottomLayout);

	ui->centralWidget->setLayout(layout);

	QString username = connection.userName();
	if(username=="admin")
		drawAdminControls();
	else if(username=="coordenador")
		drawCoordenadorControls();
	else if(username=="professor")
		drawProfessorControls();
	else if(username=="aluno")
		drawAlunoControls();
	else
		qWarning("Interface error.");
}


void MainWindow::drawAdminControls()
{
	QTabWidget *tW = ui->centralWidget->findChild<QTabWidget*>("tabWidget");

	QWidget *inicio = new QWidget();
	QVBoxLayout *inicioLayout = new QVBoxLayout(inicio);

	QSqlQuery query;
	unsigned int counter;

	counter = 0;
	QLabel *label = new QLabel;
	query.exec("call mostraAluno();");
	while(query.next()) {
		counter++;
		query.value(0);
	}
	label->setText("Total de alunos: "+QString::number(counter));
	inicioLayout->addWidget(label);

	counter = 0;
	label = new QLabel;
	query.exec("call mostraProfessor();");
	while(query.next()) {
		counter++;
		query.value(0);
	}
	label->setText("Total de professores: "+QString::number(counter));
	inicioLayout->addWidget(label);

	counter = 0;
	label = new QLabel;
	query.exec("call mostraDisciplina();");
	while(query.next()) {
		counter++;
		query.value(0);
	}
	label->setText("Número de disciplinas cadastradas: "+QString::number(counter));
	inicioLayout->addWidget(label);

	counter = 0;
	label = new QLabel;
	query.exec("call mostraTurma();");
	while(query.next()) {
		counter++;
		query.value(0);
	}
	label->setText("Total de turmas disponíveis: "+QString::number(counter));
	inicioLayout->addWidget(label);

	counter = 0;
	label = new QLabel;
	query.exec("call mostraCampus();");
	while(query.next()) {
		counter++;
		query.value(0);
	}
	label->setText("Número de campi: "+QString::number(counter));
	inicioLayout->addWidget(label);

	counter = 0;
	label = new QLabel;
	query.exec("call mostraSala();");
	while(query.next()) {
		counter++;
		query.value(0);
	}
	label->setText("Número de salas em todos os campi: "+QString::number(counter));
	inicioLayout->addWidget(label);

	counter = 0;
	label = new QLabel;
	query.exec("call mostraMatricula();");
	while(query.next()) {
		counter++;
		query.value(0);
	}
	label->setText("Matriculas realizadas: "+QString::number(counter));
	inicioLayout->addWidget(label);

	tW->addTab(inicio,"Início");







	tW->addTab(new QWidget(),"Consultas");





	tW->addTab(new QWidget(),"Manutenção");





	QWidget *acesso = new QWidget();
	QVBoxLayout *acessoLayout = new QVBoxLayout(acesso);
	QTextEdit *text = new QTextEdit();
	text->setObjectName("Entrada");
	QPushButton *exec = new QPushButton("Executar");
	exec->setObjectName("Executar");

	connect(exec, SIGNAL(clicked(bool)), this, SLOT(sendQuery()));

	QTextEdit *output = new QTextEdit();
	output->setObjectName("Saida");
	output->setReadOnly(true);

	acessoLayout->addWidget(text);
	acessoLayout->addWidget(exec);
	acessoLayout->addWidget(output);

	tW->addTab(acesso,"Acesso direto");
}

void MainWindow::drawCoordenadorControls()
{
	QTabWidget *tW = ui->centralWidget->findChild<QTabWidget*>("tabWidget");
	tW->addTab(new QWidget(),"Início");
	tW->addTab(new QWidget(),"Consultas");
	tW->addTab(new QWidget(),"Manutenção");
}

void MainWindow::drawProfessorControls()
{
	QTabWidget *tW = ui->centralWidget->findChild<QTabWidget*>("tabWidget");
	tW->addTab(new QWidget(),"Início");
	tW->addTab(new QWidget(),"Turmas");
	tW->addTab(new QWidget(),"Perfil");
}

void MainWindow::drawAlunoControls()
{
	QSqlQuery query;

	query.exec("call raUsuario('"+connection.user+"');");
	query.first();

	QTabWidget *tW = ui->centralWidget->findChild<QTabWidget*>("tabWidget");

	QWidget *inicio = new QWidget();
	QVBoxLayout *inicioLayout = new QVBoxLayout(inicio);

	tW->addTab(inicio,"Início");

	QWidget* matricula = new QWidget;
	QVBoxLayout* matriculaLayout = new QVBoxLayout(matricula);
	QListWidget* listWidget = new QListWidget;
	listWidget->setObjectName("listWidget");
	QListWidgetItem* item;
	query.exec("call mostraTurma()");
	while(query.next()) {
		QString text;
		QSqlQuery query2;

		query2.exec("call nomeCampus("+query.value(2).toString()+")");
		query2.first();
		text = query.value(1).toString()+"\t"+query.value(3).toString()+"\t"+query2.value(0).toString()+"\t";

		query2.exec("call nomeDisciplina('"+query.value(5).toString()+"')");
		query2.first();
		text += query2.value(0).toString();

		item = new QListWidgetItem(listWidget);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Unchecked);
		item->setText(text);
		item->setData(Qt::UserRole, query.value(0).toString());
	}

	QPushButton* save = new QPushButton("Salvar");
	save->setObjectName("saveButton");
	connect(save, SIGNAL(clicked(bool)), this, SLOT(requestEnrollment()));

	matriculaLayout->addWidget(listWidget);
	matriculaLayout->addWidget(save);

	tW->addTab(matricula,"Matrícula");



	query.exec("call raUsuario('"+connection.user+"');");
	query.first();
	QString RA = query.value(0).toString();

	QWidget *perfil = new QWidget();
	QVBoxLayout *perfilLayout = new QVBoxLayout(perfil);

	query.exec("call buscaAluno("+RA+");");
	query.first();
	QLabel *label = new QLabel;

	label->setText("RA: "+query.value(0).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Nome: "+query.value(1).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("RG: "+query.value(2).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("CPF: "+query.value(3).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	if(!QString::compare("M", query.value(4).toString()))
		label->setText("Sexo: Masculino");
	else
		label->setText("Sexo: Feminino");
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Campus: "+query.value(5).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Campus: "+query.value(6).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Email: "+query.value(9).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Turno: "+query.value(10).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("CR: "+query.value(11).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("CP: "+query.value(12).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("CA: "+query.value(13).toString());
	perfilLayout->addWidget(label);

	perfilLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

	tW->addTab(perfil,"Perfil");
}









































