import React, {Component} from 'react';
import {observer} from "mobx-react";
import {Module} from "../../library/module";
import {observable} from "mobx";
import classNames from "classnames";
import {Card, ListGroup} from "react-bootstrap";
import CommonStat from "../../components/react-common-stat";
import TempController from "../../controls/temp-controller";

class Greenhouse extends Module {

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

    @observable value_vcc = 0;

    constructor(props) {
        super(props);
        this.onChangeState = this.onChangeState.bind(this);
    }

    get moduleName() {
        return 'greenhouse';
    }

    get receiver() {
        return 3;
    }

    onChangeState(rp, command, data) {
        this.sendCommand(rp, command, data);
    }
}

const greenhouse = new Greenhouse();

@observer
export class GreenhouseView extends Component {
    wrapperClass;
    cardClass;

    render() {
        const wrapperClass = classNames('mt-1', this.props.wrapperClass);
        const cardClass = classNames('mt-1', 'ml-1', this.props.cardClass);
        return <Card className={wrapperClass}>
            <Card.Header>
                <h4>Теплица</h4></Card.Header>
            <Card.Body>
                <Card className={cardClass}>
                    <Card.Header><h5>Температура</h5></Card.Header>
                    <Card.Body>
                        <ListGroup>
                            <ListGroup.Item variant="success">
                                <CommonStat
                                    value={greenhouse.ds18b20_temperature}
                                    valueMin={greenhouse.ds18b20_temperature_min}
                                    valueMax={greenhouse.ds18b20_temperature_max}
                                    valueHour={greenhouse.ds18b20_temperature_hour}
                                    formatValue={"+0.0"}
                                    unitName={"℃"}
                                    loading={greenhouse.loading}
                                    title={"Температура 1:"}
                                />
                            </ListGroup.Item>
                            <ListGroup.Item variant="success">
                                <CommonStat
                                    value={greenhouse.bme280_temperature}
                                    valueMin={greenhouse.bme280_temperature_min}
                                    valueMax={greenhouse.bme280_temperature_max}
                                    valueHour={greenhouse.bme280_temperature_hour}
                                    formatValue={"+0.0"}
                                    unitName={"℃"}
                                    loading={greenhouse.loading}
                                    title={"Температура 2:"}
                                />
                            </ListGroup.Item>
                            <ListGroup.Item variant="success">
                                <CommonStat
                                    value={greenhouse.bme280_humidity}
                                    valueMin={greenhouse.bme280_humidity_min}
                                    valueMax={greenhouse.bme280_humidity_max}
                                    valueHour={greenhouse.bme280_humidity_hour}
                                    unitName={"%"}
                                    loading={greenhouse.loading}
                                    title={"Влажность:"}
                                />
                            </ListGroup.Item>
                        </ListGroup>
                    </Card.Body>
                </Card>
                <TempController cardClass={cardClass} onChangeState={greenhouse.onChangeState}/>
            </Card.Body>
        </Card>;
    }
}