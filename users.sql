USE matriculasrff;

CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin';
GRANT ALL PRIVILEGES ON matriculasrff.* TO 'admin'@'localhost' WITH GRANT OPTION;

CREATE USER 'coordenador'@'localhost' IDENTIFIED BY 'coordenador';
GRANT EXECUTE ON PROCEDURE mostraAluno TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraCampus TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraDisciplina TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraEmail TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraEndereco TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraMatricula TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraProfessor TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraSala TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraSalaTurma TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraTurma TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraTurmaProfessor TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE insereAluno TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE insereCampus TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE insereDisciplina TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE insereProfessor TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE insereSala TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE insereTurma TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE raUsuario TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE buscaAluno TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE nomeDisciplina TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE nomeCampus TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE fazMatricula TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE desfazMatricula TO 'coordenador'@'localhost';
GRANT EXECUTE ON PROCEDURE turmasAluno TO 'coordenador'@'localhost';

CREATE USER 'professor'@'localhost' IDENTIFIED BY 'professor';
GRANT EXECUTE ON PROCEDURE mostraCampus TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraDisciplina TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraSala TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraSalaTurma TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraTurma TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraTurmaProfessor TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE raUsuario TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE nomeDisciplina TO 'professor'@'localhost';
GRANT EXECUTE ON PROCEDURE nomeCampus TO 'professor'@'localhost';

CREATE USER 'aluno'@'localhost' IDENTIFIED BY 'aluno';
GRANT EXECUTE ON PROCEDURE mostraCampus TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraDisciplina TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraSala TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraSalaTurma TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraTurma TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE mostraTurmaProfessor TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE raUsuario TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE buscaAluno TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE nomeDisciplina TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE nomeCampus TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE fazMatricula TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE desfazMatricula TO 'aluno'@'localhost';
GRANT EXECUTE ON PROCEDURE turmasAluno TO 'aluno'@'localhost';

