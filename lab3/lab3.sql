CREATE DATABASE IF NOT EXISTS lab2_3;
USE lab2_3;

CREATE TABLE Department (
    dept_id INT PRIMARY KEY,
    dept_name VARCHAR(100) UNIQUE NOT NULL,
    mgr_id INT,
    mgr_start_date DATE
);

CREATE TABLE Employee (
    emp_id INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    ssn INT UNIQUE NOT NULL,
    address VARCHAR(200),
    salary DECIMAL(12,2) CHECK (salary > 0),
    super_id INT,
    dept_id INT
);

CREATE TABLE Project (
    proj_id INT PRIMARY KEY,
    proj_name VARCHAR(100) NOT NULL,
    proj_location VARCHAR(50),
    dept_id INT
);

CREATE TABLE Works_On (
    emp_id INT,
    proj_id INT,
    hours DECIMAL(6,2) CHECK (hours >= 0),
    PRIMARY KEY (emp_id, proj_id)
);

ALTER TABLE Department
ADD CONSTRAINT fk_dept_mgr FOREIGN KEY (mgr_id) REFERENCES Employee(emp_id);

ALTER TABLE Employee
ADD CONSTRAINT fk_emp_super FOREIGN KEY (super_id) REFERENCES Employee(emp_id);

ALTER TABLE Employee
ADD CONSTRAINT fk_emp_dept FOREIGN KEY (dept_id) REFERENCES Department(dept_id);

ALTER TABLE Project
ADD CONSTRAINT fk_proj_dept FOREIGN KEY (dept_id) REFERENCES Department(dept_id);

ALTER TABLE Works_On
ADD CONSTRAINT fk_works_emp FOREIGN KEY (emp_id) REFERENCES Employee(emp_id);

ALTER TABLE Works_On
ADD CONSTRAINT fk_works_proj FOREIGN KEY (proj_id) REFERENCES Project(proj_id);

ALTER TABLE Employee
ADD email VARCHAR(100) UNIQUE;

ALTER TABLE Project
MODIFY proj_location VARCHAR(100);

ALTER TABLE Employee
DROP COLUMN address;

INSERT INTO Employee (emp_id, name, ssn, salary, super_id, dept_id, email)
VALUES
(1, 'Alice Johnson', 111111111, 95000, NULL, NULL, 'alice@example.com'),
(2, 'Bob Smith',     222222222, 75000, 1, NULL, 'bob@example.com'),
(3, 'Charlie Lee',   333333333, 85000, 1, NULL, 'charlie@example.com'),
(4, 'Diana White',   444444444, 65000, 2, NULL, 'diana@example.com');

INSERT INTO Department (dept_id, dept_name, mgr_id, mgr_start_date)
VALUES
(10, 'HR',     1, '2020-01-15'),
(20, 'Finance',3, '2021-03-01'),
(30, 'IT',     2, '2019-07-20');

UPDATE Employee SET dept_id = 10 WHERE emp_id = 1;
UPDATE Employee SET dept_id = 20 WHERE emp_id = 2;
UPDATE Employee SET dept_id = 30 WHERE emp_id = 3;
UPDATE Employee SET dept_id = 30 WHERE emp_id = 4;

INSERT INTO Project (proj_id, proj_name, proj_location, dept_id)
VALUES
(100, 'Recruitment System', 'New York', 10),
(200, 'Budget Analysis',    'Chicago',  20),
(300, 'Network Upgrade',    'San Jose', 30),
(400, 'AI Research',        'Boston',   30);

INSERT INTO Works_On (emp_id, proj_id, hours)
VALUES
(1, 100, 20.5),
(2, 200, 15.0),
(3, 300, 30.0),
(4, 300, 12.0),
(4, 400, 25.0);

SELECT * FROM Department;
SELECT * FROM Employee;
SELECT * FROM Project;
SELECT * FROM Works_On;


CREATE VIEW HighSalaryEmployees AS
SELECT E.name AS employee_name, D.dept_name, E.salary
FROM Employee E
JOIN Department D ON E.dept_id = D.dept_id
WHERE E.salary > 80000;

CREATE VIEW DeptProjectCount AS
SELECT D.dept_name, COUNT(P.proj_id) AS project_count
FROM Department D
LEFT JOIN Project P ON D.dept_id = P.dept_id
GROUP BY D.dept_name;


	