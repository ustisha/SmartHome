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
class TempControllerSettings extends Component {

    @observable inProcess;

    constructor(props) {
        super(props);
        this.inProcess = false;
    }

    componentDidMount() {
        const f = () => {
            this.inProcess = false;
        };
        reaction(() => this.props.module.temp_controller_down_limit, f);
        reaction(() => this.props.module.temp_controller_timeout, f);
    }

    @computed get downLimit() {
        return this.props.module.temp_controller_down_limit;
    }

    @computed get timeout() {
        return Math.round(this.props.module.temp_controller_timeout / 1000);
    }

    handleDownLimit = value => {
        this.inProcess = true;
        this.props.module.sendCommand(Net.PORT_TEMP_CTRL, Net.CMD_DOWN_LIMIT, parseFloat(value.toString()) * 100).then(() => {

        });
    };

    handleTimeout = value => {
        this.inProcess = true;
        this.props.module.sendCommand(Net.PORT_TEMP_CTRL, Net.CMD_TIMEOUT, value * 1000).then(() => {

        });
    };

    render() {
        const cardClass = classNames('mt-1', 'ml-1', this.props.cardClass);
        return <div className={cardClass}>
            <h5>Контроль температуры</h5>
            <Form>
                <Form.Group className="mb-0" as={Row} controlId="formDownLimit">
                    <Form.Label column sm={6}>
                        Нижний уровень (℃):
                    </Form.Label>
                    <Col sm={2}>
                        <InputTimeout size="sm"
                                      type="text"
                                      disabled={this.inProcess}
                                      value={this.downLimit}
                                      onChange={this.handleDownLimit}/>
                    </Col>
                </Form.Group>
                <Form.Group className="mb-0" as={Row} controlId="formTimeout">
                    <Form.Label column sm={6}>
                        Обновление (Сек.):
                    </Form.Label>
                    <Col sm={2}>
                        <InputTimeout size="sm"
                                      type="text"
                                      disabled={this.inProcess}
                                      value={this.timeout}
                                      onChange={this.handleTimeout}/>
                    </Col>
                </Form.Group>
            </Form>
        </div>;
    }
}

export default TempControllerSettings;