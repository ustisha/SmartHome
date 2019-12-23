import React, {Component} from 'react';
import NumeralFormat from "./react-numeral-format";
import {Spinner} from "react-bootstrap";

class CommonValue extends Component {

    constructor(props) {
        super(props)
    }

    render() {
        const title = this.props.title || 'Значение';
        const formatValue = this.props.formatValue || '0.0';
        const unitName = this.props.unitName || '';
        return <div>
            <h5>
                {title}&nbsp;
                <span hidden={this.props.loading}>
                <b><NumeralFormat value={this.props.value} format={formatValue}/>&nbsp;{unitName}</b>
            </span>
                <span hidden={!this.props.loading}>
                <Spinner animation="border" variant="info" size="sm"/>
            </span>
            </h5>
        </div>;
    }
}

export default CommonValue;