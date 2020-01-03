import {DataProcessor} from "../data-processor";

export class PressureGraph extends DataProcessor {

    constructor(key) {
        super();
        this.key = key;
    }

    process(module, data) {
        let history = [];
        if (data[this.key]) {
            let max = 0;
            let min = 1000;
            for (let hour in data[this.key]) {
                if (data[this.key][hour] > max) {
                    max = data[this.key][hour];
                }
                if (data[this.key][hour] < min) {
                    min = data[this.key][hour];
                }
            }
            for (let hour in data[this.key]) {
                let yy = (data[this.key][hour] - min + (max - min));
                history.push({
                    x: hour,
                    y: parseFloat(yy.toFixed(2)),
                    mmHg: data[this.key][hour]
                });
            }
            module.pressureHistory = history;
        }
    }
}