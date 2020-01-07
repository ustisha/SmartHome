import React, {Component} from "react";
import ModuleCore from "../components/module-core";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import CommonValue from "../components/react-common-value";
import {observable} from "mobx";
import {observer} from "mobx-react";
import AutoLight from "../components/auto-light";
import {Module} from "../library/module";
import TempController from "../components/temp-controller";

class ColdChamberModule extends Module {

    @observable bme280_temperature = 0;
    @observable bme280_temperature_min = 0;
    @observable bme280_temperature_max = 0;
    @observable bme280_temperature_hour = 0;

    @observable bme280_humidity = 0;
    @observable bme280_humidity_min = 0;
    @observable bme280_humidity_max = 0;
    @observable bme280_humidity_hour = 0;

    @observable value_vcc = 0;

    @observable temp_controller_servo_0 = 0;
    @observable temp_controller_mode = 1;

    @observable light_controller_relay_0 = 0;
    @observable light_controller_mode = 1;

    constructor(props) {
        super(props);
    }

    get moduleName() {
        return 'coldchamber';
    }

    get receiver() {
        return 4;
    }

    onChangeState(rp, command, data) {
        /* @debug
        if (rp === 7) {
            if (command === 50) {
                this.temp_controller_servo_0 = data;
            } else if (command === 8) {
                this.temp_controller_mode = data;
            }
        }
        if (rp === 10) {
            if (command === 40) {
                this.light_controller_relay_0 = data;
            } else if (command === 8) {
                this.light_controller_mode = data;
            }
        }*/

        this.sendCommand(rp, command, data);
    }

}

const coldChamber = new ColdChamberModule({});

@observer
class ColdChamber extends Component {

    constructor(props) {
        super(props);
    }

    render() {
        return <ModuleCore title={"Холодный отсек"}>
            <Container label={"Инфо"} fluid={true}>
                <Row>
                    <Col xs lg="3">
                        <CommonValue value={coldChamber.temp_controller_servo_0} formatValue={"+0.0"} unitName={"℃"} title={"Температура:"}/>
                        <CommonValue value={coldChamber.temp_controller_mode} formatValue={"0.0"} unitName={"%"} title={"Влажность:"}/>
                    </Col>
                    <Col>
                        <TempController module={coldChamber}/>
                    </Col>
                    <Col>
                        <AutoLight module={coldChamber}/>
                    </Col>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default ColdChamber;