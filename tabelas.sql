CREATE DATABASE IF NOT EXISTS matriculasrff;
USE matriculasrff;

DROP TABLE IF EXISTS sala_turma;
DROP TABLE IF EXISTS turma_professor;
DROP TABLE IF EXISTS professor;
DROP TABLE IF EXISTS sala;
DROP TABLE IF EXISTS matricula;
DROP TABLE IF EXISTS turma;
DROP TABLE IF EXISTS disciplina;
DROP TABLE IF EXISTS aluno;
DROP TABLE IF EXISTS email;
DROP TABLE IF EXISTS endereco;
DROP TABLE IF EXISTS campus;
DROP TABLE IF EXISTS quadrimestre;


CREATE TABLE quadrimestre (
  id int NOT NULL,
  codigo varchar(6) DEFAULT NULL,
  inicio date DEFAULT NULL,
  fim date DEFAULT NULL,
  PRIMARY KEY (id)
);


CREATE TABLE campus (
  id int NOT NULL,
  nome varchar(20) DEFAULT NULL,
  PRIMARY KEY (id)
);


CREATE TABLE endereco (
  rua varchar(30) DEFAULT NULL,
  numero int NOT NULL,
  bairro varchar(30) DEFAULT NULL,
  cidade varchar(30) DEFAULT NULL,
  estado char(2) DEFAULT NULL,
  pais varchar(30) DEFAULT NULL,
  cep char(9) NOT NULL,
  PRIMARY KEY (numero, cep)
);


CREATE TABLE email (
  endereco char(50) NOT NULL,
  PRIMARY KEY (endereco)
);


CREATE TABLE aluno (
  ra int NOT NULL,
  nome varchar(100) DEFAULT NULL,
  rg varchar(15) DEFAULT NULL,
  cpf char(14) DEFAULT NULL,
  sexo char(1) DEFAULT NULL,
  id_campus int NOT NULL,
  numero_endereco int NOT NULL,
  cep_endereco char(9) NOT NULL,
  endereco_email char(50) NOT NULL,
  data_nascimento date DEFAULT NULL,
  turno varchar(7) DEFAULT NULL,
  cr decimal(4,3) DEFAULT NULL,
  cp decimal(4,3) DEFAULT NULL,
  ca decimal(4,3) DEFAULT NULL,
  PRIMARY KEY (ra),
  KEY fk_aluno_campus (id_campus),
  KEY fk_aluno_endereco (numero_endereco, cep_endereco),
  KEY fk_aluno_email (endereco_email),
  CONSTRAINT fk_aluno_campus FOREIGN KEY (id_campus) REFERENCES campus (id) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_aluno_endereco FOREIGN KEY (numero_endereco, cep_endereco) REFERENCES endereco (numero, cep) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_aluno_email FOREIGN KEY (endereco_email) REFERENCES email (endereco) ON DELETE NO ACTION ON UPDATE NO ACTION
);


CREATE TABLE disciplina (
  codigo varchar(10) NOT NULL,
  nome varchar(80) DEFAULT NULL,
  t smallint DEFAULT NULL,
  p smallint DEFAULT NULL,
  i smallint DEFAULT NULL,
  carga_horaria smallint DEFAULT NULL,
  creditos smallint DEFAULT NULL,
  ementa varchar(200) DEFAULT NULL,
  PRIMARY KEY (codigo)
);


CREATE TABLE turma (
  id int NOT NULL,
  nome varchar(30) DEFAULT NULL,
  id_campus int NOT NULL,
  turno varchar(7) NOT NULL,
  vagas smallint DEFAULT NULL,
  codigo_disciplina varchar(10) DEFAULT NULL,
  quadrimestre int NOT NULL,
  PRIMARY KEY (id),
  KEY fk_turma_id_campus (id_campus),
  KEY fk_turma_disciplina (codigo_disciplina),
  KEY fk_turma_quadrimestre (quadrimestre),
  UNIQUE (nome, id_campus, turno, codigo_disciplina, quadrimestre),
  CONSTRAINT fk_turma_id_campus FOREIGN KEY (id_campus) REFERENCES campus (id) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_turma_disciplina FOREIGN KEY (codigo_disciplina) REFERENCES disciplina (codigo) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_turma_quadrimestre FOREIGN KEY (quadrimestre) REFERENCES quadrimestre (id) ON DELETE NO ACTION ON UPDATE NO ACTION
);


CREATE TABLE matricula (
  ra int NOT NULL,
  id_turma int NOT NULL,
  conceito char(1) DEFAULT NULL,
  PRIMARY KEY (ra, id_turma),
  KEY fk_matricula_aluno (ra),
  KEY fk_matricula_turma (id_turma),
  CONSTRAINT fk_matricula_aluno FOREIGN KEY (ra) REFERENCES aluno (ra) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_matricula_turma FOREIGN KEY (id_turma) REFERENCES turma (id) ON DELETE NO ACTION ON UPDATE NO ACTION
);


CREATE TABLE sala (
  codigo varchar(10) NOT NULL,
  tipo varchar(15) DEFAULT NULL,
  capacidade smallint DEFAULT NULL,
  id_campus int DEFAULT NULL,
  PRIMARY KEY (codigo, id_campus),
  KEY fk_sala_campus (id_campus),
  CONSTRAINT fk_sala_campus FOREIGN KEY (id_campus) REFERENCES campus (id) ON DELETE NO ACTION ON UPDATE NO ACTION
);


CREATE TABLE professor (
  siape int NOT NULL,
  nome varchar(30) DEFAULT NULL,
  rg varchar(15) DEFAULT NULL,
  cpf char(14) DEFAULT NULL,
  sexo char(1) DEFAULT NULL,
  numero_endereco int NOT NULL,
  cep_endereco char(9) NOT NULL,
  endereco_email char(50) NOT NULL,
  data_nascimento date DEFAULT NULL,
  codigo_sala varchar(10) NOT NULL,
  id_campus int NOT NULL,
  PRIMARY KEY (siape),
  KEY fk_professor_sala (codigo_sala, id_campus),
  CONSTRAINT fk_professor_sala FOREIGN KEY (codigo_sala, id_campus) REFERENCES sala (codigo, id_campus) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_professor_endereco FOREIGN KEY (numero_endereco, cep_endereco) REFERENCES endereco (numero, cep) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_professor_email FOREIGN KEY (endereco_email) REFERENCES email (endereco) ON DELETE NO ACTION ON UPDATE NO ACTION
);


CREATE TABLE turma_professor (
  siape_professor int DEFAULT NULL,
  id_turma int DEFAULT NULL,
  KEY fk_turma_professor_professor (siape_professor),
  KEY fk_turma_professor_turma (id_turma),
  UNIQUE (siape_professor, id_turma),
  CONSTRAINT fk_turma_professor_professor FOREIGN KEY (siape_professor) REFERENCES professor (siape) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_turma_professor_turma FOREIGN KEY (id_turma) REFERENCES turma (id) ON DELETE NO ACTION ON UPDATE NO ACTION
);


CREATE TABLE sala_turma (
  codigo_sala varchar(10) DEFAULT NULL,
  id_campus int NOT NULL,
  id_turma int NOT NULL,
  dia date NOT NULL,
  hora_inicio time(0) NOT NULL,
  hora_fim time(0) NOT NULL,
  KEY fk_sala_turma_sala (codigo_sala, id_campus),
  KEY fk_sala_turma_turma (id_turma),
  CONSTRAINT fk_sala_turma_sala FOREIGN KEY (codigo_sala, id_campus) REFERENCES sala (codigo, id_campus) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT fk_sala_turma_turma FOREIGN KEY (id_turma) REFERENCES turma (id) ON DELETE NO ACTION ON UPDATE NO ACTION
);



