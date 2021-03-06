import React, {Component} from 'react';
import classNames from "classnames";
import ProgressBar from "react-bootstrap/ProgressBar";
import ButtonGroup from "react-bootstrap/ButtonGroup";
import Button from "react-bootstrap/Button";
import DropdownButton from "react-bootstrap/DropdownButton";
import Dropdown from "react-bootstrap/Dropdown";
import {computed} from "mobx";
import {observer} from "mobx-react";
import Net from "../net/Net";
import CommonValue from "./react-common-value";

@observer
class TempController extends Component {

    minAngle;

    maxAngle;

    constructor(props) {
        super(props);
        this.minAngle = props.minAngle || 0;
        this.maxAngle = props.maxAngle || 90;
    }

    get mode() {
        return this.props.module.temp_controller.get(this.props.port).mode;
    }

    set mode(m) {
        this.onChangeState(Net.CMD_MODE, m);
    }

    get angle() {
        return this.props.module.temp_controller.get(this.props.port).servo_0;
    }

    set angle(a) {
        this.onChangeState(Net.CMD_SERVO_00, a);
    }

    setPercent(percent) {
        this.angle = Math.ceil(percent * this.maxAngle / 100);
    }

    @computed get downLimit() {
        return this.props.module.temp_controller.get(this.props.port).down_limit;
    }

    @computed get anglePercent() {
        return Math.ceil(this.angle * 100 / this.maxAngle);
    }

    @computed get isAuto() {
        return this.mode === Net.MODE_AUTO;
    }

    @computed get autoVariant() {
        return this.mode === Net.MODE_AUTO ? 'success' : 'dark';
    }

    @computed get manualVariant() {
        return this.mode === Net.MODE_MANUAL ? 'danger' : 'dark';
    }

    onChangeState(command, data) {
        return this.props.module.onChangeState(this.props.port, command, data);
    }

    render() {
        const cardClass = classNames('mt-1', 'ml-1', this.props.cardClass);
        return <div>
            <CommonValue value={this.downLimit}
                         formatValue={"+0.0"}
                         unitName={"℃"}
                         title={"Целевая температура:"}/>
            <ProgressBar variant="warning" now={this.angle} label={`${this.anglePercent}%`} min={this.minAngle} max={this.maxAngle}/>
            <ButtonGroup className="mt-2" size="sm" aria-label="Управление">
                <Button variant={"info"} disabled={this.isAuto} onClick={() => this.angle = this.maxAngle}>Открыть</Button>
                <DropdownButton as={ButtonGroup} variant={"success"} title="Установить" disabled={this.isAuto}>
                    {[...Array(10)].map((e, i) => {
                        const angleText = ((i + 1) * 10).toString() + '%';
                        return <Dropdown.Item
                            key={`angle-item-${i + 1}`}
                            eventKey={(i + 1) * 10}
                            onSelect={this.setPercent.bind(this)}>Угол открытия {angleText}</Dropdown.Item>
                    })}
                </DropdownButton>
                <Button variant={"warning"} disabled={this.isAuto} onClick={() => this.angle = this.minAngle}>Закрыть</Button>
            </ButtonGroup>
            <ButtonGroup className="float-right mt-2" size="sm" aria-label="Режим">
                <Button variant={this.autoVariant} onClick={() => this.mode = 1}>Авто</Button>
                <Button variant={this.manualVariant} onClick={() => this.mode = 2}>Ручной</Button>
            </ButtonGroup>
        </div>;
    }
}

export default TempController;