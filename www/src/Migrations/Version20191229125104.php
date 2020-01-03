<?php

declare(strict_types=1);

namespace DoctrineMigrations;

use Doctrine\DBAL\Schema\Schema;
use Doctrine\Migrations\AbstractMigration;

/**
 * Auto-generated Migration: Please modify to your needs!
 */
final class Version20191229125104 extends AbstractMigration
{
    public function getDescription() : string
    {
        return '';
    }

    public function up(Schema $schema) : void
    {
        // this up() migration is auto-generated, please modify it to your needs
        $this->abortIf($this->connection->getDatabasePlatform()->getName() !== 'sqlite', 'Migration can only be executed safely on \'sqlite\'.');

        $this->addSql('CREATE TABLE user (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, email VARCHAR(180) NOT NULL, roles CLOB NOT NULL --(DC2Type:json)
        , password VARCHAR(255) NOT NULL)');
        $this->addSql('CREATE UNIQUE INDEX UNIQ_8D93D649E7927C74 ON user (email)');
        $this->addSql('CREATE TEMPORARY TABLE __temp__radio_log AS SELECT id, date, type, direction, sender, sender_port, receiver, receiver_port, command, data, rssi, snr FROM radio_log');
        $this->addSql('DROP TABLE radio_log');
        $this->addSql('CREATE TABLE radio_log (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, date DATETIME NOT NULL, type SMALLINT NOT NULL, direction SMALLINT NOT NULL, sender INTEGER NOT NULL, sender_port INTEGER NOT NULL, receiver INTEGER NOT NULL, receiver_port INTEGER NOT NULL, command SMALLINT NOT NULL, data BIGINT NOT NULL, rssi INTEGER, snr DOUBLE PRECISION)');
        $this->addSql('INSERT INTO radio_log (id, date, type, direction, sender, sender_port, receiver, receiver_port, command, data, rssi, snr) SELECT id, date, type, direction, sender, sender_port, receiver, receiver_port, command, data, rssi, snr FROM __temp__radio_log');
        $this->addSql('DROP TABLE __temp__radio_log');
    }

    public function down(Schema $schema) : void
    {
        // this down() migration is auto-generated, please modify it to your needs
        $this->abortIf($this->connection->getDatabasePlatform()->getName() !== 'sqlite', 'Migration can only be executed safely on \'sqlite\'.');

        $this->addSql('DROP TABLE user');
        $this->addSql('CREATE TEMPORARY TABLE __temp__radio_log AS SELECT id, date, type, direction, sender, sender_port, receiver, receiver_port, command, data, rssi, snr FROM radio_log');
        $this->addSql('DROP TABLE radio_log');
        $this->addSql('CREATE TABLE radio_log (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, date DATETIME NOT NULL, type SMALLINT NOT NULL, direction SMALLINT NOT NULL, sender INTEGER NOT NULL, sender_port INTEGER NOT NULL, receiver INTEGER NOT NULL, receiver_port INTEGER NOT NULL, command SMALLINT NOT NULL, data BIGINT NOT NULL, rssi INTEGER DEFAULT NULL, snr DOUBLE PRECISION DEFAULT NULL)');
        $this->addSql('INSERT INTO radio_log (id, date, type, direction, sender, sender_port, receiver, receiver_port, command, data, rssi, snr) SELECT id, date, type, direction, sender, sender_port, receiver, receiver_port, command, data, rssi, snr FROM __temp__radio_log');
        $this->addSql('DROP TABLE __temp__radio_log');
    }
}
