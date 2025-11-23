-- DROP DATABASE IF EXISTS lab6_1;
-- CREATE DATABASE lab6_1;
USE lab6_1;

DROP TABLE IF EXISTS Products;

CREATE TABLE Products (
	product_id INT PRIMARY KEY,
    store1 INT,
    store2 INT,
    store3 INT
);

INSERT INTO Products (product_id, store1, store2, store3)
VALUES
(1, 100, 105, 200),
(2, 110, 115, 210),
(3, 120, 125, 220),
(4, 130, 135, 230),
(5, 140, 145, 240),
(6, 150, 155, 250),
(7, 160, 165, 260),
(8, 170, 175, 270),
(9, 180, 185, 280),
(10, 190, 195, 290),
(11, 200, 205, 300),
(12, 210, 215, 310),
(13, 220, 225, 320),
(14, 230, 235, 330),
(15, 240, 245, 340),
(16, 250, 255, 350),
(17, 260, 265, 360),
(18, 270, 275, 370),
(19, 280, 285, 380),
(20, 290, 295, 390);

SELECT product_id, 'store1' AS store, store1 AS price
FROM Products
WHERE store1 IS NOT NULL

UNION ALL

SELECT product_id, 'store2' AS store, store2 AS price
FROM Products
WHERE store2 IS NOT NULL

UNION ALL

SELECT product_id, 'store3' AS store, store3 AS price
FROM Products
WHERE store3 IS NOT NULL; 


