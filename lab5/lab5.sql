DROP DATABASE IF EXISTS lab5_1;
CREATE DATABASE lab5_1;
USE lab5_1;

DROP TABLE IF EXISTS Participation;
DROP TABLE IF EXISTS Venue;
DROP TABLE IF EXISTS Event;
DROP TABLE IF EXISTS Student;

CREATE TABLE Student (
    student_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    department VARCHAR(20),
    year INT
);

CREATE TABLE Event (
    event_id INT PRIMARY KEY AUTO_INCREMENT,
    event_name VARCHAR(100) NOT NULL,
    category VARCHAR(20)
);

CREATE TABLE Venue (
    venue_id INT PRIMARY KEY AUTO_INCREMENT,
    venue_name VARCHAR(100) NOT NULL,
    capacity INT
);

CREATE TABLE Participation (
    participation_id INT PRIMARY KEY AUTO_INCREMENT,
    student_id INT,
    event_id INT,
    registration_date DATE,
    FOREIGN KEY (student_id) REFERENCES Student(student_id),
    FOREIGN KEY (event_id) REFERENCES Event(event_id)
);

INSERT INTO Student (name, department, year) VALUES
('Ravi', 'CSE', 2),
('Meena', 'ECE', 1),
('Arjun', 'CSE', 3),
('Priya', 'ME', 2);

INSERT INTO Event (event_name, category) VALUES
('Tech Quiz', 'Technical'),
('Coding Contest', 'Technical'),
('Football Match', 'Sports'),
('Dance Competition', 'Cultural');

INSERT INTO Venue (venue_name, capacity) VALUES
('Auditorium', 200),
('S+l', 80),
('Playground', 500),
('Lab 1', 40);

INSERT INTO Participation (student_id, event_id, registration_date) VALUES
(1, 1, '2025-08-25'),
(2, 2, '2025-08-26'),
(3, 1, '2025-08-27'),
(4, 4, '2025-08-28');

SELECT name, department FROM Student;
SELECT event_name, category FROM Event;
SELECT venue_name FROM Venue WHERE capacity > 100;
SELECT DISTINCT s.name FROM Student s JOIN Participation p ON s.student_id = p.student_id;
SELECT name, year FROM Student WHERE department = 'CSE';
SELECT COUNT(*) AS total_events FROM Event;
INSERT INTO Student (name, department, year) VALUES ('Asha', 'ECE', 2);
UPDATE Student SET department = 'ME' WHERE name = 'Asha';
DELETE FROM Event WHERE category = 'Sports';
SELECT DISTINCT e.event_name FROM Event e JOIN Participation p ON e.event_id = p.event_id;
