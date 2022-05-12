CREATE TABLE IF NOT EXISTS angajati (
    id serial PRIMARY KEY,
    first_name VARCHAR(300) NOT NULL,
    last_name VARCHAR(300) NOT NULL,
    cnp BIGINT NOT NULL,
    manager_id INTEGER REFERENCES angajati(id),
    data_angajare TIMESTAMP DEFAULT current_timestamp
);

INSERT INTO angajati (first_name, last_name, cnp, manager_id) VALUES ('Tudor', 'Coman', 5000000000000, NULL),
                                                                     ('Bill', 'Gates', 1000000000000, 1),
                                                                     ('Sundar', 'Pichai', 2000000000000, 1)