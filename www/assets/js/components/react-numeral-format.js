import React, {Component} from 'react';
import numeral from 'numeral'

class NumeralFormat extends Component {
    constructor(props) {
        super(props)
    }

    render() {
        const className = this.props.className;
        const value = this.props.value;
        const format = this.props.format;
        const formated = (format)
            ? numeral(value).format(format)
            : numeral(value);
        return <span className={`${(className) ? className : ''}`}>{formated}</span>;
    }
}

export default NumeralFormat;