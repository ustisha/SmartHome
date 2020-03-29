import React, {Component} from "react";
import ModuleCore from "../components/module-core";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import CommonValue from "../components/react-common-value";
import {observer} from "mobx-react";
import AutoLight from "../components/auto-light";
import {Module} from "../library/module";
import TempController from "../components/temp-controller";
import Net from "../net/Net";
import TempControllerSettings from "../components/temp-controller-settings";
import AutoLightSettings from "../components/auto-light-settings";
import {LightControllerStore} from "../stores/LightController";
import {TempControllerStore} from "../stores/TempController";

class ColdChamberModule extends Module {

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
            this.sendCommand(Net.PORT_TEMP_CTRL_00, Net.CMD_INFO, Net.CMD_GET_VALUES).then(() => {
                this.sendCommand(Net.PORT_LIGHT_CTRL_00, Net.CMD_INFO, Net.CMD_GET_VALUES).then(() => {
                    this.sendCommand(Net.PORT_LIGHT_CTRL_01, Net.CMD_INFO, Net.CMD_GET_VALUES).finally();
                })
            })
        });
    }

    onChangeState(rp, command, data) {
        return this.sendCommand(rp, command, data).finally();
    }

}

const coldChamber = new ColdChamberModule({});
coldChamber.addMapProcessor(Net.PORT_TEMP_CTRL_00, Module.TEMP_CTRL_MAP, new TempControllerStore());
coldChamber.addMapProcessor(Net.PORT_LIGHT_CTRL_00, Module.LIGHT_CTRL_MAP, new LightControllerStore());
coldChamber.addMapProcessor(Net.PORT_LIGHT_CTRL_01, Module.LIGHT_CTRL_MAP, new LightControllerStore());

@observer
class ColdChamber extends Component {

    constructor(props) {
        super(props);
    }

    render() {
        return <ModuleCore title={"Холодный отсек"} errors={coldChamber.errorMessage} getValues={coldChamber.loadData.bind(coldChamber)}>
            <Container label={"Инфо"} fluid={true}>
                <Row>
                    <Col xs={3} lg={3}>
                        <CommonValue value={coldChamber.bme280_temperature} formatValue={"+0.0"} unitName={"℃"} title={"Температура:"}/>
                        <CommonValue value={coldChamber.bme280_humidity} formatValue={"0.0"} unitName={"%"} title={"Влажность:"}/>
                    </Col>
                    <Col xs={6} lg={5}>
                        <TempController module={coldChamber} port={Net.PORT_TEMP_CTRL_00} minAngle={18} maxAngle={155}/>
                    </Col>
                    <Col xs={2} lg={4}>
                        {/* Камера */}
                        <AutoLight module={coldChamber} port={Net.PORT_LIGHT_CTRL_01}/>
                        {/* Подвал */}
                        <AutoLight module={coldChamber} port={Net.PORT_LIGHT_CTRL_00}/>
                    </Col>
                </Row>
            </Container>
            <Container label={"Настройки"} fluid={true}>
                <Row>
                    <Col xs={3} lg={3}>
                        <TempControllerSettings module={coldChamber} port={Net.PORT_TEMP_CTRL_00}/>
                    </Col>
                    <Col xs={4} lg={4}>
                        <AutoLightSettings module={coldChamber} port={Net.PORT_LIGHT_CTRL_00} title={"Освещение камера"}/>
                    </Col>
                    <Col xs={4} lg={4}>
                        <AutoLightSettings module={coldChamber} port={Net.PORT_LIGHT_CTRL_01} title={"Освещение подвал"}/>
                    </Col>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default ColdChamber;