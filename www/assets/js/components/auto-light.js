import {observer} from "mobx-react";
import React, {Component} from "react";
import {computed} from "mobx";
import ButtonGroup from "react-bootstrap/ButtonGroup";
import Button from "react-bootstrap/Button";
import Net from "../net/Net";
import Badge from "react-bootstrap/Badge";

@observer
class AutoLight extends Component {

    constructor(props) {
        super(props);
    }

    set mode(m) {
        this.onChangeState(Net.CMD_MODE, m);
    }

    get mode() {
        return this.props.module.light_controller_mode;
    }

    set relay(r) {
        this.onChangeState(Net.CMD_MODE, Net.MODE_MANUAL);
        this.onChangeState(Net.CMD_RELAY_00, r);
    }

    get relay() {
        return this.props.module.light_controller_relay_0;
    }

    @computed get isAuto() {
        return this.mode === 1;
    }

    @computed get isOn() {
        return this.relay !== 0 && this.relay === 1;
    }

    @computed get isOff() {
        return this.relay !== 0 && this.relay === 2;
    }

    @computed get autoVariant() {
        return this.mode === 1 ? 'success' : 'dark';
    }

    @computed get onVariant() {
        return !this.isAuto && this.isOn ? 'danger' : 'dark';
    }

    @computed get offVariant() {
        return !this.isAuto && this.isOff ? 'danger' : 'dark';
    }

    onChangeState(command, data) {
        this.props.module.onChangeState(this.props.rp || Net.PORT_LIGHT_CTRL_00, command, data);
    }

    render() {
        return <div>
            <div className="float-left">
                <h5>Состояние: <Badge className="p-2" variant="warning">ВКЛЮЧЕН</Badge></h5>
            </div>
            <ButtonGroup className="float-right" size="sm" aria-label="Режим">
                <Button variant={this.autoVariant} onClick={() => {
                    this.mode = 1
                }}>Авто</Button>
                <Button variant={this.onVariant} onClick={() => this.relay = 1}>Вкл.</Button>
                <Button variant={this.offVariant} onClick={() => this.relay = 2}>Выкл.</Button>
            </ButtonGroup>
        </div>;
    }
}

export default AutoLight;