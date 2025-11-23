DROP DATABASE IF EXISTS lab8;
CREATE DATABASE lab8;
USE lab8;

DROP TABLE IF EXISTS Assignment;
DROP TABLE IF EXISTS Project;
DROP TABLE IF EXISTS Employee;

CREATE TABLE Employee (
    employee_id INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    manager_id INT,
    department VARCHAR(50),
    salary DECIMAL(10,2),
    hire_date DATE NOT NULL,
    FOREIGN KEY (manager_id) REFERENCES Employee(employee_id)
);

CREATE TABLE Project (
    project_id INT PRIMARY KEY,
    project_name VARCHAR(100) NOT NULL,
    budget DECIMAL(12,2),
    start_date DATE NOT NULL,
    end_date DATE
);

CREATE TABLE Assignment (
    assignment_id INT PRIMARY KEY,
    employee_id INT NOT NULL,
    project_id INT NOT NULL,
    role VARCHAR(50) NOT NULL,
    hours_allocated INT NOT NULL,
    FOREIGN KEY (employee_id) REFERENCES Employee(employee_id),
    FOREIGN KEY (project_id) REFERENCES Project(project_id)
);

INSERT INTO Employee (employee_id, name, manager_id, department, salary, hire_date) VALUES
(100, 'Alice Johnson', NULL, 'Executive', 120000.00, '2015-05-10'),
(101, 'Bob Smith', 100, 'Sales', 85000.00, '2020-11-20'),
(102, 'Charlie Brown', 100, 'IT', 95000.00, '2016-01-15'),
(103, 'Diana Prince', 101, 'Sales', 70000.00, '2021-03-01'),
(104, 'Eve Adams', 102, 'IT', 98000.00, '2022-07-25'),
(105, 'Frank Miller', 102, NULL, 60000.00, '2023-09-01'),
(106, 'Grace Lee', 101, 'Sales', NULL, '2019-02-14'),
(107, 'Henry Ford', 100, 'HR', 75000.00, '2018-06-30');

INSERT INTO Project (project_id, project_name, budget, start_date, end_date) VALUES
(1, 'Alpha Migration', 500000.00, '2024-01-01', '2024-12-31'),
(2, 'Quantum Leap', 1200000.00, '2025-05-01', '2026-03-15'),
(3, 'Sales Campaign Q4', 80000.00, '2025-10-01', '2025-11-30'),
(4, 'Legacy Cleanup', 50000.00, '2020-03-01', '2020-09-30');

INSERT INTO Assignment (assignment_id, employee_id, project_id, role, hours_allocated) VALUES
(1, 102, 1, 'Tech Lead', 160),
(2, 104, 1, 'Developer', 80),
(3, 101, 2, 'Project Manager', 200),
(4, 103, 3, 'Sales Strategist', 120),
(5, 106, 3, 'Marketing Support', 80),
(6, 105, 4, 'Analyst', 160),
(7, 104, 2, 'Architect', 40),
(8, 102, 2, 'Engineer', 80);

-- 1
SELECT
    E.name AS EmployeeName,
    M.name AS ManagerName
FROM
    Employee E
LEFT JOIN
    Employee M ON E.manager_id = M.employee_id;


-- 2
SELECT
    name AS EmployeeName,
    COALESCE(CAST(salary AS CHAR(20)), 'Not Disclosed') AS Salary
FROM
    Employee;

-- 3
SELECT
    E.name AS EmployeeName,
    M.name AS ManagerName,
    E.salary - M.salary AS SalaryDifference
FROM
    Employee E
INNER JOIN
    Employee M ON E.manager_id = M.employee_id
WHERE
    E.manager_id IS NOT NULL;


-- 4
SELECT
    project_name,
    DATEDIFF(end_date, start_date) AS DurationDays
FROM
    Project
WHERE
    end_date IS NOT NULL;


-- 5
SELECT
    name,
    hire_date
FROM
    Employee
WHERE
    hire_date < DATE_SUB(CURDATE(), INTERVAL 5 YEAR);



-- 6
SELECT
    name AS EmployeeName,
    COALESCE(department, 'Unassigned') AS Department
FROM
    Employee;


-- 7
SELECT
    E.name AS EmployeeName,
    E.salary AS EmployeeSalary,
    M.name AS ManagerName,
    M.salary AS ManagerSalary
FROM
    Employee E
INNER JOIN
    Employee M ON E.manager_id = M.employee_id
WHERE
    E.salary > M.salary;



-- 8
SELECT
    project_name,
    CASE
        WHEN end_date > CURRENT_DATE THEN 'Ongoing'
        WHEN end_date <= CURRENT_DATE THEN 'Completed'
        ELSE 'Status Unknown'
    END AS ProjectStatus
FROM
    Project;


-- 9
SELECT
    E.name AS EmployeeName,
    COUNT(A.project_id) AS NumberOfProjects
FROM
    Employee E
LEFT JOIN
    Assignment A ON E.employee_id = A.employee_id
GROUP BY
    E.employee_id, E.name
ORDER BY
    NumberOfProjects DESC;


-- 10
SELECT
    COALESCE(department, 'No Department') AS Department,
    AVG(salary) AS AverageSalary
FROM
    Employee
WHERE
    salary IS NOT NULL
GROUP BY
    COALESCE(department, 'No Department')
ORDER BY
    AverageSalary DESC;
