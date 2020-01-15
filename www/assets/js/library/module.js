import {observable, ObservableMap} from "mobx";
import Back from "back";
import Net from "../net/Net";
import {MapProcessor} from "./data-processor/map-processor";

export class Module {

    @observable loading = false;
    @observable lastUpdate = 0;
    @observable errorMessage = '';

    @observable ds18b20_temperature = 0;
    @observable ds18b20_temperature_min = 0;
    @observable ds18b20_temperature_max = 0;
    @observable ds18b20_temperature_hour = 0;

    @observable bme280_temperature = 0;
    @observable bme280_temperature_min = 0;
    @observable bme280_temperature_max = 0;
    @observable bme280_temperature_hour = 0;
    @observable bme280_pressure = 0;

    @observable bme280_humidity = 0;
    @observable bme280_humidity_min = 0;
    @observable bme280_humidity_max = 0;
    @observable bme280_humidity_hour = 0;

    @observable pressureHistory = [
        {
            x: '12h',
            y: 12,
            mmHg: 0
        }, {
            x: '6h',
            y: 6,
            mmHg: 0
        }, {
            x: '3h',
            y: 3,
            mmHg: 0
        }, {
            x: '2h',
            y: 2,
            mmHg: 0
        }, {
            x: '1h',
            y: 1,
            mmHg: 0
        }, {
            x: '0h',
            y: 0,
            mmHg: 0
        }
    ];

    /**
     *
     * @type {Map<Number, LightControllerStore>}
     */
    @observable light_controller = new Map([]);

    /**
     *
     * @type {Map<Number, TempControllerStore>}
     */
    @observable temp_controller = new Map([]);

    /**
     *
     * @type {number}
     */
    @observable bh1750_light = 0;

    /**
     *
     * @type {number}
     */
    @observable value_vcc = 0;

    static get TYPE_LORA() {
        return 'lora';
    }

    static get TYPE_NRF24() {
        return 'nrf24';
    }

    static get LIGHT_CTRL_MAP() {
        return 'light_controller';
    }

    static get TEMP_CTRL_MAP() {
        return 'temp_controller';
    }

    /**
     * @type {Back}
     */
    attempt;

    /**
     *
     * @type {DataProcessor[]}
     */
    processors = [];

    constructor() {
        this.initWs();
    }

    get moduleName() {
        return undefined;
    }

    get receiver() {
        return undefined;
    }

    get moduleType() {
        return Module.TYPE_NRF24;
    }

    get sender() {
        return Net.GATEWAY;
    }

    get senderPort() {
        return Net.PORT_HTTP_HANDLER;
    }

    initWs() {
        this.ws = new WebSocket(`ws://${window.location.hostname}:8081?component=${this.moduleName}`);
        this.ws.onopen = this.onWsOpen.bind(this);
        this.ws.onmessage = this.onWsMessage.bind(this);
        this.ws.onclose = this.onWsClose.bind(this);
        this.ws.onerror = this.onWsError.bind(this);
    }

    onWsOpen() {
        this.errorMessage = '';
        if (this.attempt) {
            this.attempt.close();
        }
        this.loadData();
    }

    onWsClose() {
        let back = this.attempt || (this.attempt = new Back());
        return back.backoff(function(fail) {
            if (fail) {
                process.exit(1);
            }
            this.errorMessage = `Переподключение #${back.settings.attempt}`;
            this.initWs();
        }.bind(this));
    }

    onWsError() {
        this.errorMessage = 'Ошибка подключения!';
    }

    /**
     *
     * @param {DataProcessor} processor
     */
    addDataProcessor(processor) {
        this.processors.push(processor);
    }

    /**
     *
     * @param {Number} port
     * @param {String} name
     * @param {Object} store
     */
    addMapProcessor(port, name, store) {
        if (!this[name] || !(this[name] instanceof ObservableMap)) {
            throw `Observable Map '${name}' not found!`;
        }
        this[name].set(port, store);
        let processor = new MapProcessor(port, name);
        this.addDataProcessor(processor);
    }

    /**
     *
     * @param {Object} data
     */
    processData(data) {
        for (let prop in data) {
            if (this.hasOwnProperty(prop) && !(this[prop] instanceof ObservableMap)) {
                this[prop] = data[prop];
            }
        }
        this.processors.forEach((value) => {
            value.process(this, data);
        });
    }

    onWsMessage(event) {
        const data = JSON.parse(event.data);
        this.processData(data);
    }

    sendCommand(rp, command, data) {
        let params = {
            s: this.sender,
            sp: this.senderPort,
            r: this.receiver,
            rp: rp,
            cmd: command,
            data: data
        };
        let body = Object.keys(params).map(key => `${key}=${encodeURIComponent(params[key])}`).join('&');
        return fetch(`/command/${this.moduleType}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded'
            },
            body: body
        })
        .then((resp) => {
            return resp.json();
        });
    }

    loadData() {
        this.loading = true;
        return fetch(`/data/${this.moduleName}`)
        .then((resp) => {
            return resp.json();
        })
        .then((data) => {
            this.processData(data);
        }).finally(() => {
            if (this.loading) {
                this.loading = false;
            }
        });
    }

}
