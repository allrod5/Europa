USE matriculasrff;

CALL mostraAluno();

CALL mostraCampus();

CALL mostraDisciplina();

CALL mostraEmail();

CALL mostraEndereco();

CALL mostraMatricula();

CALL mostraProfessor();

CALL mostraSala();

CALL mostraSalaTurma();

CALL mostraTurma();

CALL mostraTurmaProfessor();

CALL insereAluno(
	11009755,
	'Dumb',
	'44.222.111-0',
	'444.999.111-00',
	'M',
	1,
	'dumb@aluno.ufabc.edu.br',
	'1995-2-3',
	'diurno',
	1,
	'1'
);

CALL insereCampus('Santo André');

CALL insereSala('S514-2', 'S', 2, 1);

CALL insereDisciplina('BC0001', 'Base Experiemental das Ciências Naturais', 0, 4, 2, 4, 4, 'RECOMENDAÇÕES: NÃO HÁ. Experimentos...');

CALL insereProfessor('Márcio Oikawa', '22.111.444-0', '888.000.111-00', 'M', 101, '11222-333', 'marcio.oikawa@ufabc.edu.br', '1980-1-30', 'S514-2', 1);

CALL insereTurma('A1 - Matutino', 30, 'BC0001');

create temporary table TMP(ra int,
  nome varchar(100),
  rg varchar(15),
  cpf char(14),
  sexo char(1),
  id_campus int,
  numero_endereco int,
  cep_endereco char(9),
  endereco_email char(30),
  data_nascimento date,
  turno varchar(7),
  cr decimal(4,3),
  cp decimal(4,3),
  ca decimal(4,3));


call raUsuario('dumb');

call buscaAluno(11009755);

select count(aluno.ra) from aluno;





insert into email (endereco) values ('rodrigo.m@aluno.ufabc.edu.br');

update aluno
set endereco_email = email;


call nomeDisciplina('BC0001');

select * from disciplina;


call mostraQuadrimestre();


call turmasAluno(11009713);







