DROP DATABASE IF EXISTS lab7;
CREATE DATABASE lab7;
USE lab7;

CREATE TABLE Student (
    student_id INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    department VARCHAR(50) NOT NULL,
    year INT
);

CREATE TABLE Venue (
    venue_id INT PRIMARY KEY,
    venue_name VARCHAR(100) NOT NULL,
    capacity INT
);

CREATE TABLE Event (
    event_id INT PRIMARY KEY,
    event_name VARCHAR(100) NOT NULL,
    category VARCHAR(50) NOT NULL,
    venue_id INT,
    event_date DATE,
    FOREIGN KEY (venue_id) REFERENCES Venue(venue_id)
);

CREATE TABLE Participation (
    participation_id INT PRIMARY KEY,
    student_id INT,
    event_id INT,
    registration_date DATE,
    FOREIGN KEY (student_id) REFERENCES Student(student_id),
    FOREIGN KEY (event_id) REFERENCES Event(event_id)
);

CREATE TABLE Result (
    result_id INT PRIMARY KEY,
    event_id INT,
    student_id INT,
    prize VARCHAR(50), -- e.g., '1st', '2nd', '3rd'
    FOREIGN KEY (event_id) REFERENCES Event(event_id),
    FOREIGN KEY (student_id) REFERENCES Student(student_id)
);

INSERT INTO Venue (venue_id, venue_name, capacity) VALUES
(101, 'Main Auditorium', 500),
(102, 'Seminar Hall A', 150),
(103, 'Sports Ground', 1000),
(104, 'Computer Lab 3', 50);

INSERT INTO Event (event_id, event_name, category, venue_id, event_date) VALUES
(201, 'Code Clash', 'Technical', 104, '2024-11-15'),
(202, 'Web Design Mania', 'Technical', 104, '2024-11-16'),
(203, 'Data Hackathon', 'Technical', 102, '2025-02-05'),
(204, 'Dance Off', 'Cultural', 101, '2024-12-01'),
(205, 'Singing Star', 'Cultural', 101, '2024-12-02'),
(206, 'Football Tournament', 'Sports', 103, '2025-01-10'),
(207, 'Quiz Bowl', 'General', 102, '2025-03-01'),
(208, 'Tech Talk Series', 'Technical', 102, '2025-02-15'),
(209, 'Poetry Slam', 'Cultural', 101, '2025-02-20'),
(210, 'Basketball Match', 'Sports', 103, '2025-01-11'),
(211, 'E-Sports', 'Technical', 104, '2025-01-20');

INSERT INTO Student (student_id, name, department, year) VALUES
(1, 'Alice Johnson', 'CSE', 2),
(2, 'Bob Williams', 'CSE', 3),
(3, 'Charlie Brown', 'ECE', 2),
(4, 'Diana Prince', 'MECH', 4),
(5, 'Ethan Hunt', 'ECE', 1),
(6, 'Fiona Glen', 'CSE', 3),
(7, 'George King', 'MECH', 2);

INSERT INTO Participation (participation_id, student_id, event_id, registration_date) VALUES
(1001, 1, 201, '2024-10-01'),
(1002, 1, 202, '2024-10-02'), 
(1003, 1, 203, '2024-12-05'),  
(1004, 1, 208, '2025-01-10'),  
(1005, 1, 211, '2025-01-01'),  
(1006, 2, 201, '2024-10-01'), 
(1007, 2, 204, '2024-10-05'), 
(1008, 3, 202, '2024-10-03'), 
(1009, 3, 205, '2024-11-01'),
(1010, 3, 206, '2024-12-10'),
(1011, 4, 201, '2024-10-04'),
(1012, 4, 207, '2025-01-15'),
(1013, 5, 206, '2024-12-11'),
(1014, 6, 201, '2024-10-01'),
(1015, 6, 202, '2024-10-02'),
(1016, 6, 203, '2024-12-05'),
(1017, 6, 208, '2025-01-10'),
(1018, 6, 211, '2025-01-01'),
(1019, 7, 207, '2025-01-20'),
(1020, 7, 210, '2025-01-21'),
(1021, 5, 207, '2025-01-01'),
(1022, 5, 208, '2025-01-02'),
(1023, 5, 210, '2025-01-03');

INSERT INTO Result (result_id, event_id, student_id, prize) VALUES
(3001, 201, 1, '1st'),   
(3002, 201, 6, '2nd'),   
(3003, 204, 2, '1st'),   
(3004, 204, 3, '2nd'),   
(3005, 206, 5, '1st'),   
(3006, 206, 3, '2nd'),   
(3007, 203, 1, '3rd'),   
(3008, 203, 6, '1st'),   
(3009, 207, 4, '1st');   


SELECT
    T1.name
FROM
    Student AS T1
INNER JOIN
    Participation AS T2 ON T1.student_id = T2.student_id
INNER JOIN
    Event AS T3 ON T2.event_id = T3.event_id
WHERE
    T3.category = 'Technical'
GROUP BY
    T1.student_id, T1.name
HAVING
    COUNT(DISTINCT T3.event_id) = (
        SELECT COUNT(event_id) FROM Event WHERE category = 'Technical'
    );


SELECT
    T1.event_name
FROM
    Event AS T1
INNER JOIN
    Venue AS T2 ON T1.venue_id = T2.venue_id
WHERE
    T2.capacity = (
        SELECT MAX(capacity) FROM Venue
    );


WITH DepartmentParticipationCounts AS (
    SELECT
        T1.student_id,
        T1.name,
        T1.department,
        COUNT(T2.event_id) AS total_participations,
        -- Use RANK() or DENSE_RANK() if ties for maximum should all be returned
        ROW_NUMBER() OVER(PARTITION BY T1.department ORDER BY COUNT(T2.event_id) DESC) as rn
    FROM
        Student AS T1
    JOIN
        Participation AS T2 ON T1.student_id = T2.student_id
    GROUP BY
        T1.student_id, T1.name, T1.department
)
SELECT
    name,
    department,
    total_participations
FROM
    DepartmentParticipationCounts
WHERE
    rn = 1;


SELECT
    event_name
FROM
    Event
WHERE
    event_id NOT IN (
        SELECT DISTINCT event_id FROM Participation
    );


SELECT
    T1.name,
    T1.department,
    COUNT(T2.event_id) AS events_participated,
    RANK() OVER(PARTITION BY T1.department ORDER BY COUNT(T2.event_id) DESC) AS department_rank
FROM
    Student AS T1
LEFT JOIN
    Participation AS T2 ON T1.student_id = T2.student_id
GROUP BY
    T1.student_id, T1.name, T1.department
ORDER BY
    T1.department, department_rank;


SELECT
    T1.venue_name
FROM
    Venue AS T1
JOIN
    Event AS T2 ON T1.venue_id = T2.venue_id
GROUP BY
    T1.venue_id, T1.venue_name
HAVING
    COUNT(T2.event_id) > 3;


SELECT
    T1.name
FROM
    Student AS T1
INNER JOIN
    Participation AS T2 ON T1.student_id = T2.student_id
INNER JOIN
    Event AS T3 ON T2.event_id = T3.event_id
GROUP BY
    T1.student_id, T1.name
HAVING
    -- Check that all participations for the student are after the date
    MIN(T3.event_date) > '2025-01-01'
    -- And ensure they have participated in at least one event
    AND COUNT(T2.participation_id) > 0;

SELECT
    T1.event_name,
    SUM(CASE WHEN T2.prize = '1st' THEN 1 ELSE 0 END) AS num_first_prizes,
    SUM(CASE WHEN T2.prize = '2nd' THEN 1 ELSE 0 END) AS num_second_prizes,
    SUM(CASE WHEN T2.prize = '3rd' THEN 1 ELSE 0 END) AS num_third_prizes
FROM
    Event AS T1
LEFT JOIN
    Result AS T2 ON T1.event_id = T2.event_id
GROUP BY
    T1.event_id, T1.event_name
ORDER BY
    T1.event_name;



SELECT
    T1.name
FROM
    Student AS T1
INNER JOIN
    Participation AS T2 ON T1.student_id = T2.student_id
WHERE
    T1.student_id NOT IN (
        SELECT DISTINCT student_id FROM Result
    )
GROUP BY
    T1.student_id, T1.name
HAVING
    COUNT(T2.event_id) >= 2;



WITH EventParticipantCounts AS (
    SELECT
        event_id,
        COUNT(DISTINCT student_id) AS unique_participants
    FROM
        Participation
    GROUP BY
        event_id
)
SELECT
    T1.event_name,
    T2.unique_participants
FROM
    Event AS T1
INNER JOIN
    EventParticipantCounts AS T2 ON T1.event_id = T2.event_id
WHERE
    T2.unique_participants = (
        SELECT MAX(unique_participants) FROM EventParticipantCounts
    );