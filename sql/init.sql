CREATE DATABASE 'hg' DEFAULT CHARACTER SET utf8 DEFAULT COLLATE utf8_general_ci;
CREATE USER 'hg'@'localhost' IDENTIFIED BY '1qazxsw2';
GRANT ALL ON hg.* TO 'hg'@'localhost';
