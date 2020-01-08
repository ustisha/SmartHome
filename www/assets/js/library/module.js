import {observable} from "mobx";
import Back from "back";
import Net from "../net/Net";

export class Module {

    @observable loading = false;

    @observable lastUpdate = 0;

    @observable errorMessage = '';

    static get TYPE_LORA() {
        return 'lora';
    }

    static get TYPE_NRF24() {
        return 'nrf24';
    }

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
        this.loadData();
    }

    onWsClose() {
        let back = new Back();
        return back.backoff(function(fail) {
            if (fail) {
                process.exit(1);
            }
            this.errorMessage = `Переподключение #${back.settings.attempt}`;
            this.initWs();
        }.bind(this));
    }

    onWsError() {
        this.errorMessage = 'Ошибка сети! Обновите страницу F5';
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
     * @param {Object} data
     */
    processData(data) {
        for (let prop in data) {
            if (this.hasOwnProperty(prop)) {
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
