DROP DATABASE IF EXISTS Registro;
CREATE DATABASE Registro;
USE Registro;

CREATE TABLE Jugador (
	usuario VARCHAR(40),
	contraseña VARCHAR(20),
	identificador_partidas INT,
	partidas_jugadas INT,
	partidas_ganadas INT,
) engine = InnoDB;

CREATE TABLE Partida (
	identificador INT,
	fecha DATETIME,
	duración TIME,
	ganador VARCHAR(40),
)ENGINE = InnoDB;


INSERT INTO Jugador VALUES ('Miguel', 'jfd0930jd', 1, 1, 1);
INSERT INTO Jugador VALUES ('Sergio', 'dje839jwos0', 1, 1, 0);
INSERT INTO Jugador VALUES ('Victor', 'ifoe930jw2i93', 2, 2, 0);
INSERT INTO Jugador VALUES ('Neus', '8302j0so20', 2, 2, 1);



INSERT INTO Partida VALUES (1,2023-07-05 00:05:00, 00:40:00, 'Miguel');
INSERT INTO Partida VALUES (2,2023-09-15 11:20:00, 00:15:00, 'Neus');
