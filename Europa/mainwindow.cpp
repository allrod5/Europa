#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	drawLogin();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::drawLogin()
{
	qDeleteAll(ui->centralWidget->children());
	delete ui->centralWidget->layout();

	statusBar()->clearMessage();

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setObjectName("parentLayout");

	//layout->addItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding));

	QHBoxLayout* topLayout = new QHBoxLayout;
	topLayout->setObjectName("topLayout");

	topLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

	QPixmap *pic = new QPixmap;
	pic->load(":/logo_alpha.png");
	QLabel *logo = new QLabel;
	logo->setPixmap(pic->scaledToWidth(pic->width()/2,Qt::SmoothTransformation));
	topLayout->addWidget(logo);

	topLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

	layout->addLayout(topLayout);

	//layout->addItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding));

	QHBoxLayout *middleLayout = new QHBoxLayout;
	middleLayout->setObjectName("middleLayout");

	QGridLayout *fieldsLayout = new QGridLayout;
	fieldsLayout->setObjectName("fieldsLayout");

	QLabel* userLabel = new QLabel("Usuário:");
	QFont font = userLabel->font();
	font.setPointSize(18);
	userLabel->setFont(font);
	QLineEdit* userField = new QLineEdit;
	userField->setObjectName("userField");
	userField->setFont(font);
	QLabel* passwordLabel = new QLabel("Senha:");
	passwordLabel->setFont(font);
	QLineEdit* passwordField = new QLineEdit;
	passwordField->setEchoMode(QLineEdit::Password);
	passwordField->setObjectName("passwordField");
	passwordField->setFont(font);

	fieldsLayout->addWidget(userLabel,0,0);
	fieldsLayout->addWidget(userField,0,1);
	fieldsLayout->addWidget(passwordLabel,1,0);
	fieldsLayout->addWidget(passwordField,1,1);

	middleLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
	middleLayout->addLayout(fieldsLayout);
	middleLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

	layout->addLayout(middleLayout);

	layout->addItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding));

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->setObjectName("bottomLayout");

	QPushButton *connectButton = new QPushButton;
	connect(connectButton, SIGNAL(clicked()), this, SLOT(login()));
	connectButton->setText("Entrar");
	connectButton->setFont(font);
	connectButton->setObjectName("connectButton");
	bottomLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));
	bottomLayout->addWidget(connectButton);
	bottomLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));

	layout->addLayout(bottomLayout);

	layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

	ui->centralWidget->setLayout(layout);
}

void MainWindow::login()
{
	QPalette *palette = new QPalette();
	palette->setColor(QPalette::Base,Qt::lightGray);
	palette->setColor(QPalette::Text,Qt::gray);

	ui->centralWidget->findChild<QLineEdit*>("userField")->setReadOnly(true);
	ui->centralWidget->findChild<QLineEdit*>("userField")->setPalette(*palette);

	ui->centralWidget->findChild<QLineEdit*>("passwordField")->setReadOnly(true);
	ui->centralWidget->findChild<QLineEdit*>("passwordField")->setPalette(*palette);

	QString user = ui->centralWidget->findChild<QLineEdit*>("userField")->text();
	QString password = ui->centralWidget->findChild<QLineEdit*>("passwordField")->text();

	if(connection.createConnection(user, password)) {
		drawInterface();
		statusBar()->showMessage(user);
	} else {
		ui->centralWidget->findChild<QLineEdit*>("userField")->setReadOnly(false);
		ui->centralWidget->findChild<QLineEdit*>("userField")
				->setPalette( QApplication::palette( ui->centralWidget->findChild<QLineEdit*>("userField") ) );

		ui->centralWidget->findChild<QLineEdit*>("passwordField")->setReadOnly(false);
		ui->centralWidget->findChild<QLineEdit*>("passwordField")
				->setPalette( QApplication::palette( ui->centralWidget->findChild<QLineEdit*>("passwordField") ) );
	}
}

void MainWindow::logout()
{
	drawLogin();
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

	QVector<int> turmasAluno;

	query.exec("call turmasAluno("+RA+");");
	while(query.next()) {
		turmasAluno.append(query.value(0).toInt());
	}

	text = "start transaction; ";
	QListWidget *list = ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QListWidget*>("listWidget");
	for(unsigned int i=0; i<list->count(); i++ ) {
		QListWidgetItem * item = list->item(i);
		if(item->checkState()) {
			int j;
			for(j=0; j<turmasAluno.size(); j++) {
				if(turmasAluno[j]==item->data(Qt::UserRole).toInt())
					break;
			} if(j==turmasAluno.size()) {
				text += "call fazMatricula("+RA+","+item->data(Qt::UserRole).toString()+"); ";
			}
		} else {
			int j;
			for(j=0; j<turmasAluno.size(); j++) {
				if(turmasAluno[j]==item->data(Qt::UserRole).toInt())
					break;
			} if(j<turmasAluno.size()) {
				text += "call desfazMatricula("+RA+","+item->data(Qt::UserRole).toString()+"); ";
			}
		}
	}
	text += "commit;";

	query.exec(text);

	drawInterface();

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
	connect(exitButton, SIGNAL(clicked()), this, SLOT(logout()));
	exitButton->setText("Sair");
	exitButton->setObjectName("exitButton");
	QFont font = exitButton->font();
	font.setPointSize(18);
	exitButton->setFont(font);

	QPixmap *pic = new QPixmap;
	pic->load(":/logo_alpha.png");
	QLabel *logo = new QLabel;
	logo->setPixmap(pic->scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	bottomLayout->addWidget(logo);
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
	QSqlQuery query;

	query.exec("call siapeUsuario('"+connection.user+"');");
	query.first();
	QString SIAPE = query.value(0).toString();

	QTabWidget *tW = ui->centralWidget->findChild<QTabWidget*>("tabWidget");




	QWidget *inicio = new QWidget();
	QVBoxLayout *inicioLayout = new QVBoxLayout(inicio);

	QListWidget* listWidget = new QListWidget;
	listWidget->setObjectName("listaTurmasProfessor");
	QListWidgetItem* item;

	query.exec("call turmasProfessor("+SIAPE+");");
	while(query.next()) {
		QString text;
		QSqlQuery query2;
		QSqlQuery query3;

		query2.exec("call buscaTurma("+query.value(0).toString()+");");
		query2.first();

		query3.exec("call nomeCampus("+query2.value(2).toString()+")");
		query3.first();
		text = query2.value(1).toString()+"\t"+query2.value(3).toString()+"\t"+query3.value(0).toString()+"\t";

		query3.exec("call nomeDisciplina('"+query2.value(5).toString()+"')");
		query3.first();
		text += query3.value(0).toString();

		item = new QListWidgetItem(listWidget);
		item->setFlags(item->flags() | Qt::ItemIsSelectable);

		item->setText(text);
		item->setData(Qt::UserRole, query2.value(0).toString());
	}

	inicioLayout->addWidget(new QLabel("Suas turmas são:"));

	inicioLayout->addWidget(listWidget);



	tW->addTab(inicio,"Início");

	tW->addTab(new QWidget(),"Consultas");

	QWidget *manutencao = new QWidget;
	QHBoxLayout *manutencaoLayout = new QHBoxLayout(manutencao);
	manutencaoLayout->setObjectName("manutencaoLayout");

	QListWidget *opcoes = new QListWidget;
	opcoes->setObjectName("opcoesListWidget");
	opcoes->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

	item = new QListWidgetItem(opcoes);
	item->setText("Nova turma");
	item->setData(Qt::UserRole, 0);

	item = new QListWidgetItem(opcoes);
	item->setText("Excluir turma");
	item->setData(Qt::UserRole, 1);

	item = new QListWidgetItem(opcoes);
	item->setText("Alocar professor");
	item->setData(Qt::UserRole, 2);

	item = new QListWidgetItem(opcoes);
	item->setText("Desalocar professor");
	item->setData(Qt::UserRole, 3);

	manutencaoLayout->addWidget(opcoes);

	QWidget *conf = new QWidget;
	conf->setObjectName("conf");

	QVBoxLayout *confLayout = new QVBoxLayout(conf);

	confLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

	manutencaoLayout->addWidget(conf);

	connect(opcoes, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(drawSettings(QListWidgetItem*)));

	tW->addTab(manutencao,"Manutenção");

	QWidget *perfil = new QWidget();
	QVBoxLayout *perfilLayout = new QVBoxLayout(perfil);

	query.exec("call buscaProfessor("+SIAPE+");");
	query.first();
	QLabel *label = new QLabel;

	label->setText("SIAPE: "+query.value(0).toString());
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
	label->setText("Email: "+query.value(7).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Sala: "+query.value(9).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Campus: "+query.value(10).toString());
	perfilLayout->addWidget(label);

	perfilLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

	tW->addTab(perfil,"Perfil");
}

void MainWindow::drawProfessorControls()
{
	QSqlQuery query;

	query.exec("call siapeUsuario('"+connection.user+"');");
	query.first();
	QString SIAPE = query.value(0).toString();

	QTabWidget *tW = ui->centralWidget->findChild<QTabWidget*>("tabWidget");




	QWidget *inicio = new QWidget();
	QVBoxLayout *inicioLayout = new QVBoxLayout(inicio);

	QListWidget* listWidget = new QListWidget;
	listWidget->setObjectName("listaTurmasProfessor");
	QListWidgetItem* item;

	query.exec("call turmasProfessor("+SIAPE+");");
	while(query.next()) {
		QString text;
		QSqlQuery query2;
		QSqlQuery query3;

		query2.exec("call buscaTurma("+query.value(0).toString()+");");
		query2.first();

		query3.exec("call nomeCampus("+query2.value(2).toString()+")");
		query3.first();
		text = query2.value(1).toString()+"\t"+query2.value(3).toString()+"\t"+query3.value(0).toString()+"\t";

		query3.exec("call nomeDisciplina('"+query2.value(5).toString()+"')");
		query3.first();
		text += query3.value(0).toString();

		item = new QListWidgetItem(listWidget);
		item->setFlags(item->flags() | Qt::ItemIsSelectable);

		item->setText(text);
		item->setData(Qt::UserRole, query2.value(0).toString());
	}

	inicioLayout->addWidget(new QLabel("Suas turmas são:"));

	inicioLayout->addWidget(listWidget);



	tW->addTab(inicio,"Início");


	QWidget *perfil = new QWidget();
	QVBoxLayout *perfilLayout = new QVBoxLayout(perfil);

	query.exec("call buscaProfessor("+SIAPE+");");
	query.first();
	QLabel *label = new QLabel;

	label->setText("SIAPE: "+query.value(0).toString());
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
	label->setText("Email: "+query.value(7).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Sala: "+query.value(9).toString());
	perfilLayout->addWidget(label);

	label = new QLabel;
	label->setText("Campus: "+query.value(10).toString());
	perfilLayout->addWidget(label);

	perfilLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

	tW->addTab(perfil,"Perfil");
}

void MainWindow::drawAlunoControls()
{
	QSqlQuery query;

	query.exec("call raUsuario('"+connection.user+"');");
	query.first();
	QString RA = query.value(0).toString();

	QTabWidget *tW = ui->centralWidget->findChild<QTabWidget*>("tabWidget");

	QWidget *inicio = new QWidget();
	QVBoxLayout *inicioLayout = new QVBoxLayout(inicio);

	QListWidget* listWidget = new QListWidget;
	listWidget->setObjectName("listaTurmasAluno");
	QListWidgetItem* item;

	query.exec("call turmasAluno("+RA+");");
	while(query.next()) {
		QString text;
		QSqlQuery query2;
		QSqlQuery query3;

		query2.exec("call buscaTurma("+query.value(0).toString()+");");
		query2.first();

		query3.exec("call nomeCampus("+query2.value(2).toString()+")");
		query3.first();
		text = query2.value(1).toString()+"\t"+query2.value(3).toString()+"\t"+query3.value(0).toString()+"\t";

		query3.exec("call nomeDisciplina('"+query2.value(5).toString()+"')");
		query3.first();
		text += query3.value(0).toString();

		item = new QListWidgetItem(listWidget);
		item->setFlags(item->flags() | Qt::ItemIsSelectable);

		item->setText(text);
		item->setData(Qt::UserRole, query2.value(0).toString());
	}

	QVector<int> turmasAluno;

	query.exec("call turmasAluno("+RA+");");
	while(query.next()) {
		turmasAluno.append(query.value(0).toInt());
	}

	QVector<QString> disciplinasAluno;

	for(int i=0; i<turmasAluno.size(); i++) {
		query.exec("call buscaTurma("+QString::number(turmasAluno[i])+");");
		query.first();
		disciplinasAluno.append(query.value(5).toString());
	}

	inicioLayout->addWidget(new QLabel("Suas requisições de matrícula:"));

	inicioLayout->addWidget(listWidget);



	tW->addTab(inicio,"Início");

	QWidget* matricula = new QWidget;
	QVBoxLayout* matriculaLayout = new QVBoxLayout(matricula);
	listWidget = new QListWidget;
	listWidget->setObjectName("listWidget");
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
		int i;
		for(i=0; i<turmasAluno.size(); i++) {
			if(turmasAluno[i]==query.value(0).toInt()) {
				item->setCheckState(Qt::Checked);
				break;
			}
		} if(i==turmasAluno.size()) {
			for(int j=0; j<disciplinasAluno.size(); j++) {
				if(QString::compare(disciplinasAluno[j],query.value(5).toString())==0) {
					item->setFlags(item->flags() & Qt::ItemIsEnabled);
					item->setBackgroundColor(Qt::lightGray);
					break;
				}
			}
			item->setCheckState(Qt::Unchecked);
		}
		item->setText(text);
		item->setData(Qt::UserRole, query.value(0).toString());

	}
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(recalculateRestrictions()));

	QPushButton* save = new QPushButton("Salvar");
	save->setObjectName("saveButton");
	connect(save, SIGNAL(clicked(bool)), this, SLOT(requestEnrollment()));

	matriculaLayout->addWidget(listWidget);
	matriculaLayout->addWidget(save);

	tW->addTab(matricula,"Matrícula");



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

void MainWindow::recalculateRestrictions()
{
	QSqlQuery query;

	query.exec("call raUsuario('"+connection.user+"');");
	query.first();
	QString RA = query.value(0).toString();

	QListWidget *list = ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QListWidget*>("listWidget");

	QVector<int> turmasAluno;

	for(unsigned int i=0; i<list->count(); i++ ) {
		QListWidgetItem * item = list->item(i);
		if(item->checkState()) {
			turmasAluno.append(item->data(Qt::UserRole).toInt());
		}
	}

	QVector<QString> disciplinasAluno;

	for(int i=0; i<turmasAluno.size(); i++) {
		query.exec("call buscaTurma("+QString::number(turmasAluno[i])+");");
		query.first();
		disciplinasAluno.append(query.value(5).toString());
	}

	for(unsigned int i=0; i<list->count(); i++ ) {
		QListWidgetItem * item = list->item(i);

		query.exec("call buscaTurma("+item->data(Qt::UserRole).toString()+");");
		query.first();

		int j;
		for(j=0; j<disciplinasAluno.size(); j++) {
			if(QString::compare(disciplinasAluno[j],query.value(5).toString())==0 && !item->checkState()) {
				item->setFlags(item->flags() & Qt::ItemIsEnabled);
				item->setBackgroundColor(Qt::lightGray);
				break;
			}
		} if(j==disciplinasAluno.size()) {
			item->setFlags(item->flags() | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
			item->setBackgroundColor(Qt::transparent);
		}
	}
}

void MainWindow::drawSettings(QListWidgetItem *item)
{

	QHBoxLayout *manutencaoLayout = ui->centralWidget->findChild<QTabWidget*>("tabWidget")
			->findChild<QHBoxLayout*>("manutencaoLayout");

	QWidget *conf = ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QWidget*>("conf");

	qDeleteAll(conf->children());
	delete conf->layout();

	QVBoxLayout *confLayout = new QVBoxLayout(conf);
	QHBoxLayout *majorConfLayout = new QHBoxLayout;
	QVBoxLayout *leftConfLayout = new QVBoxLayout;
	QVBoxLayout *rightConfLayout = new QVBoxLayout;
	QHBoxLayout *bottomLayout = new QHBoxLayout;

	QLabel *label;
	QLineEdit *field;
	QComboBox *comboBox;
	QPushButton *button;

	QSqlQuery query;

	switch (item->data(Qt::UserRole).toInt()) {
		case 0:
			label = new QLabel("Disciplina:");
			leftConfLayout->addWidget(label);

			comboBox = new QComboBox;
			comboBox->setObjectName("disciplina");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			query.exec("call mostraDisciplina();");
			while(query.next())
				comboBox->addItem(query.value(1).toString(), query.value(0).toString());

			rightConfLayout->addWidget(comboBox);

			label = new QLabel("Campus:");
			leftConfLayout->addWidget(label);

			comboBox = new QComboBox;
			comboBox->setObjectName("campus");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			query.exec("call mostraCampus();");
			while(query.next())
				comboBox->addItem(query.value(1).toString(), query.value(0).toString());

			rightConfLayout->addWidget(comboBox);

			label = new QLabel("Turno:");
			leftConfLayout->addWidget(label);

			comboBox = new QComboBox;
			comboBox->setObjectName("turno");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			comboBox->addItem("diurno");
			comboBox->addItem("noturno");

			rightConfLayout->addWidget(comboBox);

			label = new QLabel("Nome identificador da turma:");
			leftConfLayout->addWidget(label);

			field = new QLineEdit;
			field->setObjectName("nome");
			rightConfLayout->addWidget(field);

			label = new QLabel("Vagas:");
			leftConfLayout->addWidget(label);

			field = new QLineEdit;
			field->setObjectName("vagas");
			field->setValidator(new QIntValidator(1, 500, this));
			rightConfLayout->addWidget(field);

			button = new QPushButton("Criar nova turma");
			button->setObjectName("executar");
			bottomLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
			bottomLayout->addWidget(button);
			bottomLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

			connect(button, SIGNAL(clicked(bool)), this, SLOT(novaTurma()));

			break;

		case 1:


			break;

		case 2:
			label = new QLabel("Disciplina:");
			leftConfLayout->addWidget(label);

			comboBox = new QComboBox;
			comboBox->setObjectName("disciplina");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			query.exec("call mostraDisciplina();");
			while(query.next())
				comboBox->addItem(query.value(1).toString(), query.value(0).toString());

			rightConfLayout->addWidget(comboBox);

			label = new QLabel("Campus:");
			leftConfLayout->addWidget(label);

			comboBox = new QComboBox;
			comboBox->setObjectName("campus");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			query.exec("call mostraCampus();");
			while(query.next())
				comboBox->addItem(query.value(1).toString(), query.value(0).toString());

			rightConfLayout->addWidget(comboBox);

			label = new QLabel("Turno:");
			leftConfLayout->addWidget(label);

			comboBox = new QComboBox;
			comboBox->setObjectName("turno");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			comboBox->addItem("diurno");
			comboBox->addItem("noturno");

			rightConfLayout->addWidget(comboBox);

			label = new QLabel("Nome identificador da turma:");
			leftConfLayout->addWidget(label);

			field = new QLineEdit;
			field->setObjectName("nome");
			rightConfLayout->addWidget(field);

			/*comboBox = new QComboBox;
			comboBox->setObjectName("nome");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			query.exec("call quadrimestreAtual();");
			query.first();

			query.exec("call mostraTurmasEspecificas("+
					   rightConfLayout->findChild<QComboBox*>("campus")->currentData().toString()+","+
					   "'"+rightConfLayout->findChild<QComboBox*>("turno")->currentText()+"',"+
					   "'"+rightConfLayout->findChild<QComboBox*>("disciplina")->currentData().toString()+"',"+
					   query.value(0).toString()+","+
					   ");");
			while(query.next())
				comboBox->addItem(query.value(1).toString(), query.value(0).toString());

			rightConfLayout->addWidget(comboBox);*/


			label = new QLabel("Professor:");
			leftConfLayout->addWidget(label);

			comboBox = new QComboBox;
			comboBox->setObjectName("professor");
			comboBox->setEditable(true);
			comboBox->setAutoCompletion(true);

			query.exec("call mostraProfessor();");
			while(query.next())
				comboBox->addItem(query.value(1).toString(), query.value(0).toString());

			rightConfLayout->addWidget(comboBox);

			button = new QPushButton("Alocar professor");
			button->setObjectName("executar");
			bottomLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));
			bottomLayout->addWidget(button);
			bottomLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding));

			connect(button, SIGNAL(clicked(bool)), this, SLOT(alocarProfessor()));

			break;

		case 3:

			break;

		default:
			break;
	}





	majorConfLayout->addLayout(leftConfLayout);
	majorConfLayout->addLayout(rightConfLayout);
	confLayout->addLayout(majorConfLayout);
	confLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding));
	confLayout->addLayout(bottomLayout);
	confLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding));

	manutencaoLayout->addWidget(conf);
}

void MainWindow::novaTurma()
{
	QWidget *conf = ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QWidget*>("conf");
	QSqlQuery query;
	query.exec("call insereTurma('"+
			   conf->findChild<QLineEdit*>("nome")->text()+"',"+
			   conf->findChild<QComboBox*>("campus")->currentData().toString()+","+
			   "'"+conf->findChild<QComboBox*>("turno")->currentText()+"',"+
			   conf->findChild<QLineEdit*>("vagas")->text()+","+
			   "'"+conf->findChild<QComboBox*>("disciplina")->currentData().toString()+"'"+
			   ");");
	if(query.lastError().isValid()) {
		QMessageBox msgBox;
		msgBox.setText("Erro ao tentar criar uma nova turma:\n"+query.lastError().text());
		msgBox.exec();
	} else {
		QMessageBox msgBox;
		msgBox.setText("Turma criada com sucesso.");
		msgBox.exec();
	}
}

void MainWindow::deletarTurma()
{

}

void MainWindow::alocarProfessor()
{
	QWidget *conf = ui->centralWidget->findChild<QTabWidget*>("tabWidget")->findChild<QWidget*>("conf");
	QSqlQuery query;

	query.exec("call quadrimestreAtual();");
	query.first();

	query.exec("call idTurma('"+
			   conf->findChild<QLineEdit*>("nome")->text()+"',"+
			   conf->findChild<QComboBox*>("campus")->currentData().toString()+","+
			   "'"+conf->findChild<QComboBox*>("turno")->currentText()+"',"+
			   "'"+conf->findChild<QComboBox*>("disciplina")->currentData().toString()+"',"+
			   query.value(0).toString()+
			   ");");
	query.first();

	query.exec("call alocarProfessor("+
			   conf->findChild<QComboBox*>("professor")->currentData().toString()+","+
			   query.value(0).toString()+
			   ");");
	if(query.lastError().isValid()) {
		QMessageBox msgBox;
		msgBox.setText("Erro ao tentar alocar professor:\n"+query.lastError().text());
		msgBox.exec();
	} else {
		QMessageBox msgBox;
		msgBox.setText("Professor alocado com sucesso.");
		msgBox.exec();
	}
}

void MainWindow::desalocarProfessor()
{

}



































