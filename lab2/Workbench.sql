CREATE DATABASE IF NOT EXISTS lab2;
USE lab2;

DROP TABLE IF EXISTS Employee;

CREATE TABLE Employee (
    EmpID INT PRIMARY KEY,
    EmpName VARCHAR(100),
    Department VARCHAR(50),
    Salary DECIMAL(10,2),
    City VARCHAR(50)
);

SELECT * FROM Employee;

SET SQL_SAFE_UPDATES = 0;

INSERT INTO Employee VALUES
(210, 'Kiran Sharma', 'HR', 45000, 'Delhi'),
(211, 'A', 'IT', 60000, 'Mumbai'),
(212, 'B', 'Finance', 28000, 'Kolkata');

SELECT * FROM Employee;

UPDATE Employee SET Salary = 52000 WHERE EmpName = 'A';
SELECT * FROM Employee;
UPDATE Employee SET City = 'Bengaluru' WHERE Department = 'IT';
SELECT * FROM Employee;
DELETE FROM Employee WHERE Salary < 30000;
SELECT * FROM Employee;
UPDATE Employee SET Salary = Salary * 1.17 WHERE Department = 'Finance';
SELECT * FROM Employee;

SET SQL_SAFE_UPDATES = 1;

SELECT * FROM Employee;
