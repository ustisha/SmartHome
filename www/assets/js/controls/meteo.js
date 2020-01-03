import React, {Component} from 'react';
import {observable} from "mobx";
import {observer} from "mobx-react";
import {Card, Button, ListGroup} from "react-bootstrap";
import Moment from "react-moment";
import {VictoryAxis, VictoryChart, VictoryBar} from 'victory';
import classNames from "classnames";
import {Module} from "../library/module";
import {PressureGraph} from "../library/data-processor/pressure-graph";
import CommonStat from "../components/react-common-stat";
import CommonValue from "../components/react-common-value";

class MeteoModule extends Module {

    @observable ds18b20_temperature = 0;
    @observable ds18b20_temperature_min = 0;
    @observable ds18b20_temperature_max = 0;
    @observable ds18b20_temperature_hour = 0;

    @observable bme280_temperature = 0;
    @observable bme280_temperature_min = 0;
    @observable bme280_temperature_max = 0;
    @observable bme280_temperature_hour = 0;

    @observable bme280_humidity = 0;
    @observable bme280_humidity_min = 0;
    @observable bme280_humidity_max = 0;
    @observable bme280_humidity_hour = 0;

    @observable bme280_pressure = 0;

    @observable bh1750_light = 0;

    @observable value_vcc = 0;

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

    get moduleName() {
        return 'outside';
    }
}

const pressureGraph = new PressureGraph('bme280_pressure_each_hour');
const meteo = new MeteoModule();
meteo.addDataProcessor(pressureGraph);

@observer
export class Meteo extends Component {
    render() {
        const cardClass = classNames('mt-1', 'temp-block', this.props.className);
        return <Card className={cardClass}>
            <Card.Header>
                <div className="overflow-hidden">
                    <div className="float-left"><h4>Метеостанция</h4></div>
                    <div className="float-right">
                        <a className="cursor-pointer" onClick={meteo.loadData.bind(meteo)}><h4>{'\u27f3'}</h4></a>
                    </div>
                </div>
                <div className="card-header-info" hidden={!meteo.errorMessage.length}>
                    <h6>{meteo.errorMessage}</h6>
                </div>
                <div className="card-header-info" hidden={meteo.loading}>
                    <Moment
                        format="DD.MM.YYYY HH:mm">{meteo.lastUpdate}</Moment>&nbsp;/&nbsp;{meteo.value_vcc}V
                </div>
                <div className="card-header-info" hidden={!meteo.loading}>
                    ...
                </div>
            </Card.Header>
            <Card.Body>
                <ListGroup>
                    <ListGroup.Item variant="primary">
                        <CommonStat
                            value={meteo.ds18b20_temperature}
                            valueMin={meteo.ds18b20_temperature_min}
                            valueMax={meteo.ds18b20_temperature_max}
                            valueHour={meteo.ds18b20_temperature_hour}
                            formatValue={"+0.0"}
                            unitName={"℃"}
                            loading={meteo.loading}
                            title={"Температура 1:"}
                        />
                    </ListGroup.Item>
                    <ListGroup.Item variant="primary">
                        <CommonStat
                            value={meteo.bme280_temperature}
                            valueMin={meteo.bme280_temperature_min}
                            valueMax={meteo.bme280_temperature_max}
                            valueHour={meteo.bme280_temperature_hour}
                            formatValue={"+0.0"}
                            unitName={"℃"}
                            loading={meteo.loading}
                            title={"Температура 2:"}
                        />
                    </ListGroup.Item>
                    <ListGroup.Item variant="primary">
                        <CommonStat
                            value={meteo.bme280_humidity}
                            valueMin={meteo.bme280_humidity_min}
                            valueMax={meteo.bme280_humidity_max}
                            valueHour={meteo.bme280_humidity_hour}
                            unitName={"%"}
                            loading={meteo.loading}
                            title={"Влажность:"}
                        />
                    </ListGroup.Item>
                    <ListGroup.Item variant="primary">
                        <CommonValue
                            value={meteo.bme280_pressure}
                            unitName={"mmHg"}
                            loading={meteo.loading}
                            title={"Давление:"}
                        />
                        <div>
                            <VictoryChart
                                domainPadding={{x: 25}}>
                                <VictoryBar
                                    data={meteo.pressureHistory}
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
                        <CommonValue
                            value={meteo.bh1750_light}
                            formatValue={"0"}
                            unitName={"Lux"}
                            loading={meteo.loading}
                            title={"Освещенность:"}
                        />
                    </ListGroup.Item>
                </ListGroup>
            </Card.Body>
        </Card>;
    }
}