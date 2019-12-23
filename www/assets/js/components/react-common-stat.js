import React, {Component} from 'react';
import NumeralFormat from "./react-numeral-format";
import {Spinner} from "react-bootstrap";
import TemperatureArrow from "./react-temperature-arrow";

class CommonStat extends Component {

    constructor(props) {
        super(props)
    }

    render() {
        const title = this.props.title || 'Температура';
        const formatValue = this.props.formatValue || '0.0';
        const formatHour = this.props.formatHour || '0.0';
        const unitName = this.props.unitName || '';
        return <div>
            <h5>
                {title}&nbsp;&nbsp;
                <span hidden={this.props.loading}>
                <b><NumeralFormat value={this.props.value} format={formatValue}/>{unitName}</b>&nbsp;
            </span>
                <span hidden={!this.props.loading}>
                <Spinner animation="border" variant="info" size="sm"/>
            </span>
            </h5>
            <div>
                <span className="card-item-info" hidden={this.props.loading}>
                    Мин.:&nbsp;<b><NumeralFormat value={this.props.valueMin} format={formatValue}/>{unitName}</b>&nbsp;/
                    Макс.:&nbsp;<b><NumeralFormat value={this.props.valueMax}
                                                  format={formatValue}/>{unitName}</b>&nbsp;/
                    Изм.:&nbsp;<TemperatureArrow value={this.props.valueHour}/>
                    <b><NumeralFormat value={this.props.valueHour} format={formatHour}/>{unitName}</b>
                </span>
                <span hidden={!this.props.loading}>...</span>
            </div>
        </div>;
    }
}

export default CommonStat;