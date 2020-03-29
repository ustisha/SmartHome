import React, {Component} from "react";
import ModuleCore from "../components/module-core";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import {inject, observer} from "mobx-react";
import Button from "react-bootstrap/Button";
import {withRouter} from "react-router-dom";
import AutoLight from "../components/auto-light";
import Net from "../net/Net";
import {Module} from "../library/module";
import {LightControllerStore} from "../stores/LightController";
import AutoLightSettings from "../components/auto-light-settings";

class BathroomModule extends Module {

    constructor(props) {
        super(props);
    }

    get moduleName() {
        return Net.getName(this.receiver);
    }

    get receiver() {
        return Net.BATHROOM;
    }

    loadData() {
        super.loadData().then(() => {
            this.sendCommand(Net.PORT_LIGHT_CTRL_00, Net.CMD_INFO, Net.CMD_GET_VALUES).then(() => {
                this.sendCommand(Net.PORT_LIGHT_CTRL_01, Net.CMD_INFO, Net.CMD_GET_VALUES).finally();
            })
        });
    }

    onChangeState(rp, command, data) {
        return this.sendCommand(rp, command, data).finally();
    }

}

const bathroom = new BathroomModule({});
bathroom.addMapProcessor(Net.PORT_LIGHT_CTRL_00, Module.LIGHT_CTRL_MAP, new LightControllerStore());
bathroom.addMapProcessor(Net.PORT_LIGHT_CTRL_01, Module.LIGHT_CTRL_MAP, new LightControllerStore());

@inject("authStore")
@withRouter
@observer
class Home extends Component {

    constructor(props) {
        super(props)
    }

    handleLogout = e => {
        this.props.authStore.logout().then(() => {
            this.props.history.replace("/")
        });
    };

    render() {
        return <ModuleCore title={"Дом"} activeTab={0}>
            <Container type={"info"} label={"Инфо"} fluid={true}>
                <Row>
                    <Col>
                        Инфо 1
                    </Col>
                    <Col>
                        Инфо 2
                    </Col>
                    <Col>
                        {/* Туалет */}
                        <AutoLight module={bathroom} port={Net.PORT_LIGHT_CTRL_01}/>
                        {/* Ванна */}
                        <AutoLight module={bathroom} port={Net.PORT_LIGHT_CTRL_00}/>
                    </Col>
                </Row>
            </Container>
            <Container type={"settings"} label={"Настройки"}>
                <Row>
                    <Col xs={3} lg={3}>
                        <b>Пользователь:</b> {this.props.authStore.username}
                        <Button className="float-right" size={"sm"} onClick={this.handleLogout}>Выход</Button>
                    </Col>
                    <Col xs={4} lg={4}>
                        <AutoLightSettings module={bathroom} port={Net.PORT_LIGHT_CTRL_01} title={"Ванна"}/>
                    </Col>
                    <Col xs={4} lg={4}>
                        <AutoLightSettings module={bathroom} port={Net.PORT_LIGHT_CTRL_00} title={"Туалет"}/>
                    </Col>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default Home;