import React, {Component} from "react";
import ModuleCore from "../components/module-core";
import CommonStat from "../components/react-common-stat";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import {Module} from "../library/module";
import {observable} from "mobx";
import Net from "../net/Net";
import {observer} from "mobx-react";
import CommonValue from "../components/react-common-value";

class OutsideModule extends Module {

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

    constructor(props) {
        super(props);
    }

    get moduleName() {
        return Net.getName(this.receiver);
    }

    get receiver() {
        return Net.OUTSIDE_TEMP;
    }
}

const outside = new OutsideModule();

@observer
class Outside extends Component {

    constructor(props) {
        super(props)
    }

    render() {
        return <ModuleCore title={"Метеостанция"} activeTab={0}>
            <Container type={"info"} label={"Инфо"} fluid={true}>
                <Row>
                    <Col>
                        <CommonStat
                            value={outside.bme280_temperature}
                            valueMin={outside.bme280_temperature_min}
                            valueMax={outside.bme280_temperature_max}
                            valueHour={outside.bme280_temperature_hour}
                            formatValue={"+0.0"}
                            unitName={"℃"}
                            title={"Температура:"}
                        />
                    </Col>
                    <Col>
                        <CommonStat
                            value={outside.bme280_humidity}
                            valueMin={outside.bme280_humidity_min}
                            valueMax={outside.bme280_humidity_max}
                            valueHour={outside.bme280_humidity_hour}
                            unitName={"%"}
                            title={"Влажность:"}
                        />
                    </Col>
                    <Col>
                        <CommonValue
                            value={outside.bme280_pressure}
                            unitName={"mmHg"}
                            loading={outside.loading}
                            title={"Давление:"}
                        />
                        <CommonValue
                            value={outside.bh1750_light}
                            formatValue={"0"}
                            unitName={"Lux"}
                            loading={outside.loading}
                            title={"Освещенность:"}
                        />
                    </Col>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default Outside;