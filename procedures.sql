USE matriculasrff;

delimiter @!!
drop procedure if exists checkUser @!!
create procedure checkUser()
begin
	SELECT 'ok';
end @!!

delimiter ;

delimiter @!!
drop procedure if exists mostraAluno @!!
create procedure mostraAluno()
begin
	SELECT * FROM aluno;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraEmail @!!
create procedure mostraEmail()
begin
	SELECT * FROM email;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraCampus @!!
create procedure mostraCampus()
begin
	SELECT * FROM campus;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraDisciplina @!!
create procedure mostraDisciplina()
begin
	SELECT * FROM disciplina;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraEndereco @!!
create procedure mostraEndereco()
begin
	SELECT * FROM endereco;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraMatricula @!!
create procedure mostraMatricula()
begin
	SELECT * FROM matricula;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraProfessor @!!
create procedure mostraProfessor()
begin
	SELECT * FROM professor;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraSala @!!
create procedure mostraSala()
begin
	SELECT * FROM sala;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraSalaTurma @!!
create procedure mostraSalaTurma()
begin
	SELECT * FROM sala_turma;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraTurma @!!
create procedure mostraTurma()
begin
	SELECT * FROM turma;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraTurmaProfessor @!!
create procedure mostraTurmaProfessor()
begin
	SELECT * FROM turma_professor;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists mostraQuadrimestre @!!
create procedure mostraQuadrimestre()
begin
	SELECT * FROM quadrimestre;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists insereAluno @!!
create procedure insereAluno(
	in ra int,
	in nome varchar(100),
	in rg varchar(15),
	in cpf varchar(14),
	in sexo char(1),
	in campus int,
	in email char(30),
	in nascimento date,
	in turno varchar(7),
	in numero int,
	in cep char(9)
)
begin
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    begin
		-- ERROR
		rollback;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Erro ao inserir aluno. Operação revertida.';
	end;
	START TRANSACTION;
    
		INSERT IGNORE INTO endereco (numero, cep) VALUES (numero, cep);
		
		INSERT INTO email (endereco) VALUES (email);
		
		INSERT INTO aluno (ra, nome, rg, cpf, sexo, id_campus, endereco_email, data_nascimento, turno, numero_endereco, cep_endereco)
			VALUES (ra, nome, rg, cpf, sexo, campus, email, nascimento, turno, numero, cep);
		
    COMMIT;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists insereCampus @!!
create procedure insereCampus(in nome varchar(20))
begin
	START TRANSACTION;
		INSERT INTO campus (id, nome) VALUES (genCampusId(), nome);
    COMMIT;
end @!!

delimiter ;


delimiter @!!
drop procedure if exists insereDisciplina @!!
create procedure insereDisciplina(
	in codigo varchar(10),
	in nome varchar(80),
	in t smallint,
	in p smallint,
	in i smallint,
	in carga_horaria smallint,
	in creditos smallint,
	in ementa varchar(200)
)
begin
	START TRANSACTION;
		INSERT INTO disciplina (codigo, nome, t, p, i, carga_horaria, creditos, ementa) VALUES (codigo, nome, t, p, i, carga_horaria, creditos, ementa);
    COMMIT;
end @!!

delimiter ;


delimiter @!!
drop procedure if exists insereSala @!!
create procedure insereSala(
	in codigo varchar(10),
	in tipo varchar(15),
	in capacidade smallint,
	in campus int
)
begin
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    begin
		-- ERROR
		rollback;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Erro ao inserir sala. Operação revertida.';
	end;
	START TRANSACTION;
		INSERT INTO sala (codigo, tipo, capacidade, id_campus) VALUES (codigo, tipo, capacidade, campus);
    COMMIT;
end @!!

delimiter ;


delimiter @!!
drop procedure if exists insereProfessor @!!
create procedure insereProfessor(
	in nome varchar(30),
	in rg varchar(15),
	in cpf varchar(14),
	in sexo char(1),
	in numero int,
	in cep char(9),
	in email char(30),
	in nascimento date,
	in sala varchar(10),
	in campus int
)
begin
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    begin
		-- ERROR
		rollback;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Erro ao inserir professor. Operação revertida.';
	end;
	START TRANSACTION;
		INSERT IGNORE INTO endereco (numero, cep) VALUES (numero, cep);
		
		INSERT INTO email (endereco) VALUES (email);
        
		INSERT INTO professor (siape, nome, rg, cpf, sexo, numero_endereco, cep_endereco, endereco_email, data_nascimento, codigo_sala, id_campus)
			VALUES (genProfessorSiape(), nome, rg, cpf, sexo, numero, cep, email, nascimento, sala, campus);
    COMMIT;
end @!!

delimiter ;


delimiter @!!
drop procedure if exists insereTurma @!!
create procedure insereTurma(
	in nome varchar(30),
    in id_campus int,
    in turno varchar(7),
	in vagas smallint,
	in disciplina varchar(10)
)
begin
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    begin
		-- ERROR
		rollback;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Erro ao inserir turma. Operação revertida.';
	end;
	START TRANSACTION;
		INSERT INTO turma (id, nome, vagas, codigo_disciplina, quadrimestre) VALUES (genTurmaId(), nome, vagas, disciplina, curQuadrimestreId());
    COMMIT;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists fazMatricula @!!
create procedure fazMatricula(
	in ra int,
	in turma int
)
begin
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    begin
		-- ERROR
		rollback;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Erro ao fazer a matrícula. Operação revertida.';
	end;
	START TRANSACTION;
		INSERT INTO matricula (ra, id_turma) VALUES (ra, turma);
    COMMIT;
end @!!

delimiter ;

delimiter @!!
drop procedure if exists raUsuario @!!
create procedure raUsuario(
	in usuario char(30)
)
begin
	SELECT ra FROM aluno
    WHERE endereco_email LIKE CONCAT(usuario, '%');
end @!!

delimiter ;



delimiter @!!
drop procedure if exists buscaAluno @!!
create procedure buscaAluno(
	in ra int
)
begin
	SELECT * FROM aluno
    WHERE aluno.ra=ra;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists nomeDisciplina @!!
create procedure nomeDisciplina(
	in codigo varchar(10)
)
begin
	SELECT nome FROM disciplina
    WHERE disciplina.codigo=codigo;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists nomeCampus @!!
create procedure nomeCampus(
	in id int
)
begin
	SELECT nome FROM campus
    WHERE campus.id=id;
end @!!

delimiter ;



delimiter @!!
drop procedure if exists novoQuadrimestre @!!
create procedure novoQuadrimestre(
	in codigo varchar(6),
    in inicio date,
    in fim date
)
begin
	DECLARE id int;
    set id = curQuadrimestreId()+1;
	INSERT INTO quadrimestre (id, codigo, inicio, fim) VALUES (id, codigo, inicio, fim);
end @!!

delimiter ;





































