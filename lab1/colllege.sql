
CREATE TABLE college(
  Roll INT PRIMARY KEY,
  Name VARCHAR(50) NOT NULL,
  Age INT NOT NULL
);

INSERT INTO college VALUES
(1, "A", 21),
(2, "B", 19),
(3, "C", 22),
(4, "D", 18),
(5, "E", 22);
SELECT * FROM college;
SELECT * FROM college WHERE Age > 20;
SELECT * FROM college ORDER BY Age DESC LIMIT 1;
SELECT * FROM college ORDER BY Age;
