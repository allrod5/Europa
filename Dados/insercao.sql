call novoQuadrimestre('3Q2015', '2015-09-21', '2015-12-18');
call insereCampus('Santo André');
call insereCampus('São Bernardo');
call insereSala('S501-2', 'S', 2, 1);
INSERT INTO endereco (rua, numero, bairro, cidade, estado, pais, cep)
	VALUES ('R. Abolição', 1, 'Bangu', 'Santo André', 'SP', 'Brasil', 1);

load data infile '/tmp/emails.csv'
into table email
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/aluno1.csv'
into table aluno
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/alunos2.csv'
into table aluno
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/alunos3.csv'
into table aluno
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/disciplinas.csv'
into table disciplina
FIELDS TERMINATED BY ';'
LINES TERMINATED BY '\n';

load data infile '/tmp/docentes.csv'
into table professor
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

load data infile '/tmp/turmas.csv'
into table turma
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';