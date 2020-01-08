import React, {Component} from 'react';
import Form from "react-bootstrap/Form";
import {observer} from "mobx-react";
import {observable} from "mobx";

const WAIT_INTERVAL = 1000;
const ENTER_KEY = 13;

@observer
class InputTimeout extends Component {

    @observable userValue = undefined;

    constructor(props) {
        super(props);
    }

    get currentValue() {
        if (this.userValue === undefined) {
            return this.props.value;
        }
        return this.props.value !== this.userValue ? this.userValue : this.props.value;
    }

    componentDidMount() {
        this.timer = null;
    }

    handleChange = e => {
        clearTimeout(this.timer);
        this.userValue = e.target.value;
        this.timer = setTimeout(this.triggerChange.bind(this), WAIT_INTERVAL);
    };

    handleKeyDown = e => {
        if (e.keyCode === ENTER_KEY) {
            clearTimeout(this.timer);
            this.triggerChange();
        }
    };

    triggerChange() {
        this.props.onChange(this.userValue);
    }

    render() {
        return <Form.Control
            {...this.props}
            value={this.currentValue}
            onChange={this.handleChange}
            onKeyDown={this.handleKeyDown}/>;
    }
}

export default InputTimeout;