import React, {Component} from 'react';
import {observable} from "mobx";
import {observer} from "mobx-react";
import {Card, Button, ListGroup, Spinner} from "react-bootstrap";
import Moment from "react-moment";
import NumeralFormat from "../components/react-numeral-format";
import TemperatureArrow from "../components/react-temperature-arrow";
import {VictoryAxis, VictoryChart, VictoryBar} from 'victory';
import classNames from "classnames";

class OutsideTemp {

    @observable temperature_DS18B20 = 0;
    @observable temperature_min_DS18B20 = 0;
    @observable temperature_max_DS18B20 = 0;
    @observable temperature_1hour_DS18B20 = 0;
    @observable temperature_BME280 = 0;
    @observable temperature_min_BME280 = 0;
    @observable temperature_max_BME280 = 0;
    @observable temperature_1hour_BME280 = 0;
    @observable humidity = 0;
    @observable humidity_min = 0;
    @observable humidity_max = 0;
    @observable humidity_1hour = 0;
    @observable pressure = 0;
    @observable lux = 0;
    @observable vcc = 0;
    @observable lastUpdate = 0;
    @observable loading = false;
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

    constructor() {
        this.reload();
        setInterval(this.loadData.bind(this), 60000);
    }

    reload() {
        this.loading = true;
        this.loadData();
    }

    loadData() {
        fetch('/outside/temp')
        .then((resp) => {
            return resp.json();
        })
        .then((data) => {
            for (let prop in data) {
                if (this.hasOwnProperty(prop)) {
                    this[prop] = data[prop];
                }
            }

            let history = [];
            if (data.pressure_each_hour) {
                let max = 0;
                let min = 1000;
                for (let hour in data.pressure_each_hour) {
                    if (data.pressure_each_hour[hour] > max) {
                        max = data.pressure_each_hour[hour];
                    }
                    if (data.pressure_each_hour[hour] < min) {
                        min = data.pressure_each_hour[hour];
                    }
                }
                for (let hour in data.pressure_each_hour) {
                    let yy = (data.pressure_each_hour[hour] - min + (max - min));
                    history.push({
                        x: hour,
                        y: parseFloat(yy.toFixed(2)),
                        mmHg: data.pressure_each_hour[hour]
                    });
                }
            }
            this.pressureHistory = history;

        }).finally(() => {
            if (this.loading) {
                this.loading = false;
            }
        });
    }

}

const outsideTemp = new OutsideTemp();

@observer
export class OutsideTempView extends Component {
    render() {
        var cardClass = classNames('mt-1', this.props.className);
        return <Card className={cardClass}>
            <Card.Header>
                <h4>Метеостанция</h4>
                <div className="card-header-info" hidden={outsideTemp.loading}>
                    <Moment format="DD.MM.YYYY HH:mm">{outsideTemp.lastUpdate}</Moment>&nbsp;/&nbsp;{outsideTemp.vcc}V
                </div>
                <div className="card-header-info" hidden={!outsideTemp.loading}>
                    ...
                </div>
            </Card.Header>
            <Card.Body>
                <ListGroup>
                    <ListGroup.Item variant="primary">
                        Температура&nbsp;1:&nbsp;
                        <span hidden={outsideTemp.loading}>
                                <b><NumeralFormat value={outsideTemp.temperature_DS18B20} format="+0.0"/>℃</b>&nbsp;
                            </span>
                        <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        <div>
                                <span className="card-item-info" hidden={outsideTemp.loading}>
                                    Мин.:&nbsp;<b><NumeralFormat value={outsideTemp.temperature_min_DS18B20}
                                                                 format="+0.0"/>℃</b>&nbsp;/
                                    Макс.:&nbsp;<b><NumeralFormat value={outsideTemp.temperature_max_DS18B20}
                                                                  format="+0.0"/>℃</b>&nbsp;/
                                    Изм.:&nbsp;
                                    <TemperatureArrow value={outsideTemp.temperature_1hour_DS18B20}/>
                                    <b><NumeralFormat value={outsideTemp.temperature_1hour_DS18B20} format="0.0"/>℃</b>
                                </span>
                            <span hidden={!outsideTemp.loading}>...</span>
                        </div>
                    </ListGroup.Item>
                    <ListGroup.Item variant="primary">
                        Температура 2:&nbsp;
                        <span hidden={outsideTemp.loading}><b><NumeralFormat value={outsideTemp.temperature_BME280}
                                                                             format="+0.0"/>℃</b></span>
                        <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        <div>
                                <span className="card-item-info" hidden={outsideTemp.loading}>
                                    Мин.:&nbsp;<b><NumeralFormat value={outsideTemp.temperature_min_BME280}
                                                                 format="+0.0"/>℃</b>&nbsp;/
                                    Макс.:&nbsp;<b><NumeralFormat value={outsideTemp.temperature_max_BME280}
                                                                  format="+0.0"/>℃</b>&nbsp;/
                                    Изм.:&nbsp;
                                    <TemperatureArrow value={outsideTemp.temperature_1hour_BME280}/>
                                    <b><NumeralFormat value={outsideTemp.temperature_1hour_BME280} format="0.0"/>℃</b>
                                </span>
                            <span hidden={!outsideTemp.loading}>...</span>
                        </div>
                    </ListGroup.Item>
                    <ListGroup.Item variant="primary">
                        Влажность:&nbsp;
                        <span hidden={outsideTemp.loading}><b><NumeralFormat value={outsideTemp.humidity}
                                                                             format="0.0"/>%</b></span>
                        <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        <div>
                                <span className="card-item-info" hidden={outsideTemp.loading}>
                                    Мин.:&nbsp;<b><NumeralFormat value={outsideTemp.humidity_min}
                                                                 format="0.0"/>%</b>&nbsp;/
                                    Макс.:&nbsp;<b><NumeralFormat value={outsideTemp.humidity_max}
                                                                  format="0.0"/>%</b>&nbsp;/
                                    Изм.:&nbsp;
                                    <TemperatureArrow value={outsideTemp.humidity_1hour}/>
                                    <b><NumeralFormat value={outsideTemp.humidity_1hour} format="0.0"/>%</b>
                                </span>
                            <span hidden={!outsideTemp.loading}>...</span>
                        </div>
                    </ListGroup.Item>
                    <ListGroup.Item variant="primary">
                        Давление:&nbsp;
                        <span hidden={outsideTemp.loading}><b><NumeralFormat value={outsideTemp.pressure}
                                                                             format="0.0"/> mmHg</b></span>
                        <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        <div>
                            <VictoryChart
                                domainPadding={{x: 25}}>
                                <VictoryBar
                                    data={outsideTemp.pressureHistory}
                                    barRatio={1.1}
                                    labels={(d) => `${d.mmHg}`}
                                    domainPadding={{x: 0}}
                                    padding={{
                                        top: 60,
                                        bottom: 0,
                                        left: 0,
                                        right: 0
                                    }}
                                />
                                <VictoryAxis
                                    domainPadding={{x: 0}}
                                />
                            </VictoryChart>
                        </div>
                    </ListGroup.Item>
                    <ListGroup.Item variant="primary">
                        Освещенность:&nbsp;
                        <span hidden={outsideTemp.loading}><b>{outsideTemp.lux} Lux</b></span>
                        <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                    </ListGroup.Item>
                </ListGroup>
                <Button className="mt-1" onClick={outsideTemp.reload.bind(outsideTemp)} variant="primary" size="lg">Обновить</Button>
            </Card.Body>
        </Card>;
    }
}