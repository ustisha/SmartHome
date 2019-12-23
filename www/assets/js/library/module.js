import {observable} from "mobx";
import Back from "back";

export class Module {

    @observable loading = false;

    @observable lastUpdate = 0;

    @observable errorMessage = '';

    /**
     *
     * @type {DataProcessor[]}
     */
    processors = [];

    /**
     * @type {Back}
     */
    attempt;

    constructor() {
        this.initWs();
    }

    get moduleName() {
        return undefined;
    }

    get receiver() {
        return undefined;
    }

    get sender() {
        return 1;
    }

    get senderPort() {
        return 1001;
    }

    initWs() {
        this.ws = new WebSocket(`ws://${window.location.hostname}:8081?component=${this.moduleName}`);
        this.ws.onopen = this.onWsOpen.bind(this);
        this.ws.onmessage = this.onMessage.bind(this);
        this.ws.onclose = this.onWsClose.bind(this);
        this.ws.onerror = this.onWsError.bind(this);
    }

    onWsOpen() {
        this.errorMessage = '';
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

    onMessage(event) {
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
        fetch(`/${this.moduleName}/command`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded'
            },
            body: body
        })
        .then((resp) => {
            return resp.json();
        })
        .then((data) => {
            console.log(data);

        }).finally(() => {

        });
    }

    loadData() {
        this.loading = true;
        fetch(`/${this.moduleName}/data`)
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
