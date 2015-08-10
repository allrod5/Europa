call novoQuadrimestre('3Q2015', '2015-09-21', '2015-12-18');
call insereCampus('Santo André');
call insereCampus('São Bernardo');
call insereSala('S501-2', 'S', 2, 1);
INSERT INTO endereco (rua, numero, bairro, cidade, estado, pais, cep)
	VALUES ('R. Abolição', 1, 'Bangu', 'Santo André', 'SP', 'Brasil', 1);

load data infile '/tmp/emails.csv'
ignore into table email
CHARACTER SET UTF8
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/aluno1.csv'
ignore into table aluno
CHARACTER SET UTF8
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/alunos2.csv'
ignore into table aluno
CHARACTER SET UTF8
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/alunos3.csv'
ignore into table aluno
CHARACTER SET UTF8
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/disciplinas.csv'
ignore into table disciplina
CHARACTER SET UTF8
FIELDS TERMINATED BY ';'
LINES TERMINATED BY '\n';

load data infile '/tmp/docentes.csv'
ignore into table professor
CHARACTER SET UTF8
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/turmas.csv'
ignore into table turma
CHARACTER SET UTF8
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';