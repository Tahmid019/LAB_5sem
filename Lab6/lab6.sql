DROP DATABASE IF EXISTS Lab6_1;
CREATE DATABASE lab6_1;
USE lab6_1;

DROP TABLE IF EXISTS Employees;
DROP TABLE IF EXISTS Departments;

CREATE TABLE Employees (
	emp_id INT PRIMARY KEY,
    emp_name VARCHAR(50),
    dept_id INT,
    salary INT
);

CREATE TABLE Departments (
	dept_id INT PRIMARY KEY,
    dept_name VARCHAR(50),
    location VARCHAR(50)
);


INSERT INTO Employees (emp_id, emp_name, dept_id, salary) VALUES
(2312183, 'Gaurav', 103, 95000),
(2312182, 'Kunal', 102, 100000),
(2312181, 'Arghijit', 101, 105000),
(2312177, 'Pranava', 103, 110000),
(2312184, 'Tahmid', 101, 90000);

INSERT INTO Departments (dept_id, dept_name, location) VALUES
(101, 'IT', 'Bengaluru'),
(102, 'HR', 'Delhi'),
(103, 'Finance', 'Mumbai');

ALTER TABLE Employees ADD CONSTRAINT fk_deptid FOREIGN KEY (dept_id) REFERENCES Departments(dept_id); 


SELECT * FROM Employees;
SELECT * FROM Departments;

SELECT d.dept_name, e.emp_name, e.salary 
FROM Departments AS d
JOIN Employees AS e ON d.dept_id = e.dept_id
WHERE d.dept_name  = 'IT';

