CREATE TABLE IF NOT EXISTS produse (
    id serial PRIMARY KEY,
    name VARCHAR(300) NOT NULL,
    category VARCHAR(300) NOT NULL,
    buying_price NUMERIC(8, 2) NOT NULL,
    selling_price NUMERIC(8, 2) NOT NULL
);

INSERT INTO produse (name, category, buying_price, selling_price) VALUES ('Pix', 'papetarie', 0.99, 4.99),
                                                                     ('Top hartie A4', 'papetarie', 0.45, 3.99),
                                                                     ('iPhone 12 Pro Max', 'telefoane', 4000, 6000)