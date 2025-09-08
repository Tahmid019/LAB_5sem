DROP DATABASE IF EXISTS lab4_1;
CREATE DATABASE lab4_1;
USE lab4_1;

CREATE TABLE IF NOT EXISTS Department (
  DeptID INT PRIMARY KEY,
  DeptName VARCHAR(100) NOT NULL,
  Location VARCHAR(100)
);

CREATE TABLE IF NOT EXISTS Employee (
  EmpID INT PRIMARY KEY,
  EmpName VARCHAR(100) NOT NULL,
  Age INT,
  DeptID INT,
  Salary DECIMAL(10,2),
  FOREIGN KEY (DeptID) REFERENCES Department(DeptID)
);

INSERT INTO Department (DeptID, DeptName, Location) VALUES
(101, 'HR', 'Mumbai'),
(102, 'IT', 'Bangalore'),
(103, 'Finance', 'Delhi');

INSERT INTO Employee (EmpID, EmpName, Age, DeptID, Salary) VALUES
(1, 'Ramesh', 30, 101, 45000),
(2, 'Sita', 28, 102, 55000),
(3, 'Arjun', 35, 101, 60000),
(4, 'Priya', 26, 103, 40000),
(5, 'Kiran', 29, 102, 48000);

SELECT * FROM Employee
WHERE EmpName LIKE 'A%';

SELECT d.DeptName, AVG(e.Salary) AS AvgSalary
FROM Employee e
JOIN Department d ON e.DeptID = d.DeptID
GROUP BY d.DeptName;

SELECT * FROM Employee
WHERE Age = (SELECT MIN(Age) FROM Employee);


SELECT MAX(Salary) AS SecondHighestSalary
FROM Employee
WHERE Salary < (SELECT MAX(Salary) FROM Employee);
