DROP DATABASE IF EXISTS lab9;
CREATE DATABASE lab9;
USE lab9;

DROP TABLE IF EXISTS Books;
DROP TABLE IF EXISTS Members;
DROP TABLE IF EXISTS Issue_Records;

CREATE TABLE Books (
    book_id INT PRIMARY KEY,
    title VARCHAR(100),
    author VARCHAR(100),
    category VARCHAR(50),
    price DECIMAL(8,2),
    available_copies INT
);

CREATE TABLE Members (
    member_id INT PRIMARY KEY,
    name VARCHAR(100),
    membership_date DATE,
    email VARCHAR(100)
);

CREATE TABLE Issue_Records (
    issue_id INT PRIMARY KEY,
    book_id INT REFERENCES Books(book_id),
    member_id INT REFERENCES Members (member_id),
    issue_date DATE,
    return_date DATE
);


INSERT INTO Books (book_id, title, author, category, price, available_copies) VALUES
(101, 'Database Management Systems', 'Raghu Ramakrishnan', 'Technology', 55.00, 3),
(102, 'Introduction to Algorithms', 'Thomas H. Cormen', 'Technology', 75.50, 0),
(103, 'The Great Gatsby', 'F. Scott Fitzgerald', 'Fiction', 15.00, 5),
(104, 'Sapiens: A Brief History of Humankind', 'Yuval Noah Harari', 'History', 30.00, 1),
(105, 'Design Patterns', 'Erich Gamma', 'Technology', 65.00, 2),
(106, 'To Kill a Mockingbird', 'Harper Lee', 'Fiction', 12.00, 0),
(107, 'Cosmos', 'Carl Sagan', 'Science', 45.99, 4);

INSERT INTO Members (member_id, name, membership_date, email) VALUES
(1, 'Alice Johnson', '2021-05-10', 'alice@example.com'),
(2, 'Bob Williams', '2022-01-20', 'bob@example.com'),
(3, 'Charlie Brown', '2023-09-01', 'charlie@example.com'),
(4, 'Diana Prince', '2021-11-15', 'diana@example.com'),
(5, 'Ethan Hunt', '2024-03-25', 'ethan@example.com');

INSERT INTO Issue_Records (issue_id, book_id, member_id, issue_date, return_date) VALUES
(1001, 101, 1, '2024-10-01', '2024-10-15'),
(1002, 102, 2, '2024-10-05', NULL),
(1003, 103, 1, '2024-10-10', '2024-10-20'),
(1004, 104, 3, '2024-10-12', NULL),
(1005, 105, 2, '2024-10-18', '2024-10-28'),
(1006, 101, 4, '2024-10-25', NULL),
(1007, 107, 5, '2024-10-01', '2024-10-20');

-- 1
SELECT title, author, category
FROM Books;

-- 2
SELECT DISTINCT M.name
FROM Members M
JOIN Issue_Records IR ON M.member_id = IR.member_id;

-- 3
SELECT M.name, COUNT(IR.book_id) AS total_books_issued
FROM Members M
JOIN Issue_Records IR ON M.member_id = IR.member_id
GROUP BY M.name;

-- 4
SELECT title, author
FROM Books
WHERE available_copies = 0;

-- 5
SELECT name, membership_date
FROM Members
WHERE membership_date < '2022-01-01';

-- 6.
SELECT SUM(B.price) AS total_revenue
FROM Issue_Records IR
JOIN Books B ON IR.book_id = B.book_id;

-- 7
SELECT title, author, price
FROM Books
WHERE category = 'Technology';

-- 8
SELECT DISTINCT M.name
FROM Members M
JOIN Issue_Records IR ON M.member_id = IR.member_id
WHERE IR.return_date IS NULL;

-- 9
SELECT B.title, B.author, IR.issue_date
FROM Issue_Records IR
JOIN Books B ON IR.book_id = B.book_id
ORDER BY IR.issue_date DESC
LIMIT 1;

-- 10
SELECT category, AVG(price) AS average_price
FROM Books
GROUP BY category;