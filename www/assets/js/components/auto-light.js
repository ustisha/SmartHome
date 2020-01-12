import {observer} from "mobx-react";
import React, {Component} from "react";
import {computed} from "mobx";
import ButtonGroup from "react-bootstrap/ButtonGroup";
import Button from "react-bootstrap/Button";
import Net from "../net/Net";
import Alert from "react-bootstrap/Alert";

@observer
class AutoLight extends Component {

    constructor(props) {
        super(props);
    }

    set mode(m) {
        this.onChangeState(Net.CMD_MODE, m);
    }

    get mode() {
        return this.props.module.light_controller.get(this.props.port).mode;
    }

    set relay(r) {
        this.onChangeState(Net.CMD_MODE, Net.MODE_MANUAL).then(() => {
            this.onChangeState(Net.CMD_RELAY_00, r);
        });
    }

    get relay() {
        return this.props.module.light_controller.get(this.props.port).relay_0;
    }

    @computed get stateName() {
        return this.isOn ? 'ВКЛЮЧЕН' : 'ВЫКЛЮЧЕН';
    }

    @computed get stateVariant() {
        return this.isOn ? 'warning' : 'dark';
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
        return this.props.module.onChangeState(this.props.port, command, data);
    }

    render() {
        return <div className="float-right ml-2">
            <div className="w-100">
                <Alert className="p-2 text-center" variant={this.stateVariant}>{this.stateName}</Alert>
            </div>
            <ButtonGroup className="" size="sm" aria-label="Режим">
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