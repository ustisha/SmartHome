export class RadioLog {

    sender = {
        2: 'Метеостанция',
        3: 'Теплица'
    };

    ports = {
        2001: 'INFO',
        2002: 'DS18B20',
        2003: 'BME280',
        2004: 'BH1750',
        2005: 'VCC'
    };

    commands = {
        100: {
            title: 'Информация',
            values: {
                1: 'Запуск',
                9: 'Успешно',
                20: 'Ошибка датчика DS18B20',
                21: 'Ошибка датчика BME280',
                22: 'Ошибка датчика BH1750'
            }
        }
    };

    getSenderTitle(sender) {
        return this.sender[sender]
            ? this.sender[sender]
            : false;
    }

    getPortTitle(port) {
        return (this.ports[port])
            ? this.ports[port]
            : false;
    }

    getCommandTitle(command) {
        return this.commands[command]
            ? this.commands[command].title
            : false;
    }

    getCommandValueTitle(command, value) {
        return (this.commands[command] && this.commands[command].values[value])
            ? this.commands[command].values[value]
            : false;
    }
}