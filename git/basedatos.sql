DROP DATABASE IF EXISTS juegoSo;
CREATE DATABASE juegoSo;
USE juegoSo;
CREATE TABLE Jugador (
	Id INTEGER NOT NULL AUTO_INCREMENT,
	username VARCHAR(255),
	password VARCHAR(255),
	ganadas INTEGER,
	PRIMARY KEY (Id)
)ENGINE = InnoDB;

CREATE TABLE Partida (
	Id INTEGER NOT NULL AUTO_INCREMENT,
	fecha DATETIME,
	duracion TIME,
	ganador VARCHAR(255),
	PRIMARY KEY(Id)
)ENGINE = InnoDB;

CREATE TABLE Participacion (
	IdJ INTEGER NOT NULL,	
	IdP INTEGER NOT NULL,
	Posicion INTEGER NOT NULL,
	FOREIGN KEY (IdJ) REFERENCES Jugador(Id),
	FOREIGN KEY (IdP) REFERENCES Partida(Id)
)ENGINE = InnoDB;


INSERT INTO Jugador(username,password,ganadas) VALUES('ajudan','234afbnadf',5);
INSERT INTO Jugador(username,password,ganadas) VALUES('MichaelJackson','undostrespicaparet',3);
INSERT INTO Jugador(username,password,ganadas) VALUES('Carolina Herrera','123123',6);
INSERT INTO Jugador(username,password,ganadas) VALUES('Donald Trump','macarrronsperdinar',2);
INSERT INTO Jugador(username,password,ganadas) VALUES('ELJOseleete','12341234password',1);
INSERT INTO Jugador(username,password,ganadas) VALUES('Arnau','tres',4);

INSERT INTO Partida(fecha,duracion,ganador) VALUES('20220612','00:20:43','ajudan');
INSERT INTO Partida(fecha,duracion,ganador) VALUES('20220612','00:43:32','ajudan');
INSERT INTO Partida(fecha,duracion,ganador) VALUES('20220612','01:32:34','MichaelJackson');
INSERT INTO Partida(fecha,duracion,ganador) VALUES('20220612','00:12:32','Donald Trump');
INSERT INTO Partida(fecha,duracion,ganador) VALUES('20220612','00:34:10','MichaelJackson');


INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(1,1,1);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(1,2,2);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(1,3,1);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(2,1,2);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(2,2,1);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(2,3,3);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(3,1,3);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(3,2,3);
INSERT INTO Participacion(IdJ,IdP,Posicion) VALUES(3,3,2);
