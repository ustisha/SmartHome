import React, {Component} from 'react';
import classNames from "classnames";
import {observer} from "mobx-react";
import Form from "react-bootstrap/Form";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import {computed, observable, reaction} from "mobx";
import InputTimeout from "./input-timeout";
import Net from "../net/Net";

@observer
class AutoLightSettings extends Component {

    @observable inProcess;

    constructor(props) {
        super(props);
        this.inProcess = false;
    }

    componentDidMount() {
        const f = () => {
            this.inProcess = false;
        };
        reaction(() => this.props.module.light_controller_00_timeout, f);
        reaction(() => this.props.module.light_controller_00_activity_ratio, f);
        reaction(() => this.props.module.light_controller_00_activity_limit, f);
        reaction(() => this.props.module.light_controller_00_recall_ratio, f);
        reaction(() => this.props.module.light_controller_00_recall_timeout, f);
    }

    @computed get timeout() {
        return Math.round(this.props.module.light_controller_00_timeout / 1000);
    }

    @computed get activityRatio() {
        return this.props.module.light_controller_00_activity_ratio;
    }

    @computed get activityLimit() {
        return this.props.module.light_controller_00_activity_limit;
    }

    @computed get recallRatio() {
        return this.props.module.light_controller_00_recall_ratio;
    }

    @computed get recallTimeout() {
        return Math.round(this.props.module.light_controller_00_recall_timeout / 1000);
    }

    sendValue(cmd, value) {
        this.inProcess = true;
        this.props.module.sendCommand(Net.PORT_LIGHT_CTRL_00, cmd, parseInt(value.toString())).then(() => {

        });
    }

    sendFloatValue(cmd, value) {
        this.inProcess = true;
        this.props.module.sendCommand(Net.PORT_LIGHT_CTRL_00, cmd, parseFloat(value.toString()) * 100).then(() => {

        });
    }

    sendSecondValue(cmd, value) {
        this.inProcess = true;
        this.props.module.sendCommand(Net.PORT_LIGHT_CTRL_00, cmd, value * 1000).then(() => {

        });
    }

    handleTimeout = value => {
        this.sendSecondValue(Net.CMD_TIMEOUT, value);
    };

    handleActivityRatio = value => {
        this.sendFloatValue(Net.ACTIVITY_RATIO, value);
    };

    handleActivityLimit = value => {
        this.sendValue(Net.ACTIVITY_LIMIT, value);
    };

    handleRecallRatio = value => {
        this.sendFloatValue(Net.RECALL_RATIO, value);
    };

    handleRecallTimeout = value => {
        this.sendSecondValue(Net.CMD_TIMEOUT, value);
    };

    render() {
        const cardClass = classNames('mt-1', 'ml-1', this.props.cardClass);
        const labelSpan = 8;
        return <div className={cardClass}>
            <h5>Контроль освещения</h5>
            <Form>
                <Form.Group className="mb-0" as={Row} controlId="formLightTimeout">
                    <Form.Label column sm={labelSpan}>
                        Задержка выключения (Сек.):
                    </Form.Label>
                    <Col sm={2}>
                        <InputTimeout size="sm"
                                      type="text"
                                      disabled={this.inProcess}
                                      value={this.timeout}
                                      onChange={this.handleTimeout}/>
                    </Col>
                </Form.Group>
                <Form.Group className="mb-0" as={Row} controlId="formActivityRatio">
                    <Form.Label column sm={labelSpan}>
                        Множитель присутствия:
                    </Form.Label>
                    <Col sm={2}>
                        <InputTimeout size="sm"
                                      type="text"
                                      disabled={this.inProcess}
                                      value={this.activityRatio}
                                      onChange={this.handleActivityRatio}/>
                    </Col>
                </Form.Group>
                <Form.Group className="mb-0" as={Row} controlId="formActivityLimit">
                    <Form.Label column sm={labelSpan}>
                        Ограничитель присутствия:
                    </Form.Label>
                    <Col sm={2}>
                        <InputTimeout size="sm"
                                      type="text"
                                      disabled={this.inProcess}
                                      value={this.activityLimit}
                                      onChange={this.handleActivityLimit}/>
                    </Col>
                </Form.Group>
                <Form.Group className="mb-0" as={Row} controlId="formRecallRatio">
                    <Form.Label column sm={labelSpan}>
                        Множитель повторного включения:
                    </Form.Label>
                    <Col sm={2}>
                        <InputTimeout size="sm"
                                      type="text"
                                      disabled={this.inProcess}
                                      value={this.recallRatio}
                                      onChange={this.handleRecallRatio}/>
                    </Col>
                </Form.Group>
                <Form.Group className="mb-0" as={Row} controlId="formRecallTimeout">
                    <Form.Label column sm={labelSpan}>
                        Задержка повторного включения (Сек.):
                    </Form.Label>
                    <Col sm={2}>
                        <InputTimeout size="sm"
                                      type="text"
                                      disabled={this.inProcess}
                                      value={this.recallTimeout}
                                      onChange={this.handleRecallTimeout}/>
                    </Col>
                </Form.Group>
            </Form>
        </div>;
    }
}

export default AutoLightSettings;