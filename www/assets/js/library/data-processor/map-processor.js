import {DataProcessor} from "../data-processor";

export class MapProcessor extends DataProcessor {

    /**
     * @var {Number}
     */
    mapKey;

    /**
     * @var {String}
     */
    mapName;

    static get COMMAND_KEY() {
        return 'command';
    }

    static get VALUE_KEY() {
        return 'value';
    }

    constructor(port, name) {
        super();
        this.mapKey = port;
        this.mapName = name;
    }

    process(module, data) {
        if (data && data[this.mapName] && data[this.mapName][this.mapKey]) {
            let values = data[this.mapName][this.mapKey];
            let cmd = values[MapProcessor.COMMAND_KEY];
            let value = values[MapProcessor.VALUE_KEY];
            if (!module[this.mapName].has(this.mapKey)) {
                throw "Map module not found!";
            }
            module[this.mapName].get(this.mapKey)[cmd] = value;
        }
    }
}