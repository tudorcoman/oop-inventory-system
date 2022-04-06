CREATE TABLE IF NOT EXISTS depozite (
    id serial PRIMARY KEY,
    nume VARCHAR(300) NOT NULL,
    address TEXT NOT NULL,
    manager INTEGER REFERENCES angajati(id) NOT NULL
);

INSERT INTO depozite (nume, address, manager) VALUES ('Gestiune principala', 'Strada Academiei nr. 14', 1),
                                                     ('Depozit auxiliar', 'Soseaua Pipera 45', 2);
