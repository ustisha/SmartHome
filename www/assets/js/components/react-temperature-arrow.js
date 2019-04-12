import React, {Component} from 'react';

class TemperatureArrow extends Component {
    constructor(props) {
        super(props)
    }

    render() {
        const value = this.props.value;
        if (value === 0) {
            return '↔';
        } else if (value > 0) {
            return '↗';
        } else {
            return '↙';
        }
    }
}

export default TemperatureArrow;