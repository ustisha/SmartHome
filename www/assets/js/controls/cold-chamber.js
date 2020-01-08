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
import Net from "../net/Net";
import TempControllerSettings from "../components/temp-controller-settings";
import AutoLightSettings from "../components/auto-light-settings";

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

    @observable temp_controller_down_limit = 0;
    @observable temp_controller_up_limit = 0;
    @observable temp_controller_servo_0 = 0;
    @observable temp_controller_mode = 1;
    @observable temp_controller_timeout = 0;

    @observable light_controller_00_relay_0 = 0;
    @observable light_controller_00_mode = 1;
    @observable light_controller_00_timeout = 0;
    @observable light_controller_00_activity_ratio = 0;
    @observable light_controller_00_activity_limit = 0;
    @observable light_controller_00_recall_ratio = 0;
    @observable light_controller_00_recall_timeout = 0;

    constructor(props) {
        super(props);
    }

    get moduleName() {
        return Net.getName(this.receiver);
    }

    get receiver() {
        return Net.COLD_CHAMBER;
    }

    loadData() {
        super.loadData().then(() => {
            this.sendCommand(Net.PORT_TEMP_CTRL, Net.CMD_INFO, Net.CMD_GET_VALUES).then(() => {
                this.sendCommand(Net.PORT_LIGHT_CTRL_00, Net.CMD_INFO, Net.CMD_GET_VALUES).then(() => {

                }).finally();
            })
        });
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
        return <ModuleCore title={"Холодный отсек"} getValues={coldChamber.loadData.bind(coldChamber)}>
            <Container label={"Инфо"} fluid={true}>
                <Row>
                    <Col xs={3} lg={3}>
                        <CommonValue value={coldChamber.bme280_temperature} formatValue={"+0.0"} unitName={"℃"} title={"Температура:"}/>
                        <CommonValue value={coldChamber.bme280_humidity} formatValue={"0.0"} unitName={"%"} title={"Влажность:"}/>
                    </Col>
                    <Col xs={6} lg={6}>
                        <CommonValue value={coldChamber.temp_controller_down_limit}
                                     formatValue={"+0.0"}
                                     unitName={"℃"}
                                     title={"Целевая температура:"}/>
                        <TempController module={coldChamber} maxAngle={160}/>
                    </Col>
                    <Col xs={2} lg={3}>
                        <AutoLight module={coldChamber}/>
                    </Col>
                </Row>
            </Container>
            <Container label={"Настройки"} fluid={true}>
                <Row>
                    <Col xs lg="3">
                    </Col>
                    <Col>
                        <TempControllerSettings module={coldChamber}/>
                    </Col>
                    <Col>
                        <AutoLightSettings module={coldChamber}/>
                    </Col>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default ColdChamber;