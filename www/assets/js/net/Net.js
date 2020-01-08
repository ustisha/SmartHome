class Net {

    static components = new Map([
       [Net.GATEWAY, 'gateway'],
       [Net.OUTSIDE_TEMP, 'outside'],
       [Net.GREENHOUSE, 'greenhouse'],
       [Net.COLD_CHAMBER, 'coldchamber']
    ]);

    static getName(id) {
        return Net.components.get(id);
    }

    // Components
    static get BROADCAST() {
        return 255;
    }

    static get GATEWAY() {
        return 0;
    }

    static get OUTSIDE_TEMP() {
        return 2;
    }

    static get GREENHOUSE() {
        return 3;
    }

    static get COLD_CHAMBER() {
        return 4;
    }

    // Component ports
    static get PORT_HTTP_HANDLER() {
        return 255;
    }

    static get PORT_INFO() {
        return 254;
    }

    static get PORT_18B20() {
        return 2;
    }

    static get PORT_BME280() {
        return 3;
    }

    static get PORT_BH1750() {
        return 4;
    }

    static get PORT_VCC() {
        return 5;
    }

    static get PORT_VALUE() {
        return 6;
    }

    static get PORT_TEMP_CTRL() {
        return 7;
    }

    static get PORT_LIGHT_CTRL_00() {
        return 10;
    }

    static get PORT_LIGHT_CTRL_01() {
        return 11;
    }

    static get PORT_LIGHT_CTRL_02() {
        return 12;
    }

    static get PORT_LIGHT_CTRL_03() {
        return 13;
    }

    static get PORT_LIGHT_CTRL_04() {
        return 14;
    }

    // Commands.
    static get CMD_TEMPERATURE() {
        return 1;
    }

    static get CMD_HUMIDITY() {
        return 2;
    }

    static get CMD_PRESSURE() {
        return 3;
    }

    static get CMD_LIGHT() {
        return 4;
    }

    static get CMD_VCC() {
        return 5;
    }

    static get CMD_VALUE() {
        return 6;
    }

    static get CMD_GET_VALUES() {
        return 7;
    }

    static get CMD_MODE() {
        return 8;
    }

    // Light and Temp controllers.
    static get CMD_TIMEOUT() {
        return 9;
    }

    // Light controller.
    static get ACTIVITY_RATIO() {
        return 20;
    }

    static get ACTIVITY_LIMIT() {
        return 21;
    }

    static get RECALL_RATIO() {
        return 22;
    }

    static get RECALL_TIMEOUT() {
        return 23;
    }

    // Relays
    static get CMD_RELAY_00() {
        return 40;
    }

    static get CMD_RELAY_01() {
        return 41;
    }

    static get CMD_RELAY_02() {
        return 42;
    }

    static get CMD_RELAY_03() {
        return 43;
    }

    static get CMD_RELAY_04() {
        return 44;
    }

    static get CMD_RELAY_05() {
        return 45;
    }

    static get CMD_RELAY_06() {
        return 46;
    }

    static get CMD_RELAY_07() {
        return 47;
    }

    static get CMD_RELAY_08() {
        return 48;
    }

    static get CMD_RELAY_09() {
        return 49;
    }

    // Servos
    static get CMD_SERVO_00() {
        return 50;
    }

    static get CMD_SERVO_01() {
        return 51;
    }

    static get CMD_SERVO_02() {
        return 52;
    }

    static get CMD_SERVO_03() {
        return 53;
    }

    static get CMD_SERVO_04() {
        return 54;
    }

    static get CMD_SERVO_05() {
        return 55;
    }

    static get CMD_SERVO_06() {
        return 56;
    }

    static get CMD_SERVO_07() {
        return 57;
    }

    static get CMD_SERVO_08() {
        return 58;
    }

    static get CMD_SERVO_09() {
        return 59;
    }

    // Temp controller commands
    static get CMD_UP_LIMIT() {
        return 10;
    }

    static get CMD_DOWN_LIMIT() {
        return 11;
    }

    // Info
    static get CMD_INFO() {
        return 100;
    }

    // Data values.
    static get RELAY_DISABLED() {
        return 0;
    }

    static get RELAY_ON() {
        return 1;
    }

    static get RELAY_OFF() {
        return 2;
    }

    static get SERVO_DISABLED() {
        return 0;
    }

    static get SERVO_ANGLE() {
        return 1;
    }

    static get MODE_AUTO() {
        return 1;
    }

    static get MODE_MANUAL() {
        return 2;
    }
}

export default Net;