import React, {Component} from 'react';
import classNames from "classnames";
import ButtonToolbar from "react-bootstrap/ButtonToolbar";
import ToggleButtonGroup from "react-bootstrap/ToggleButtonGroup";
import ToggleButton from "react-bootstrap/ToggleButton";
import DropdownButton from "react-bootstrap/DropdownButton";
import DropdownItem from "react-bootstrap/DropdownItem";

class Relay extends Component {

    btnToolbarClassName;
    state;

    // Data values.
    //const RELAY_DISABLED = 0;
    //const RELAY_ON = 1;
    //const RELAY_OFF = 2;

    constructor(props) {
        super(props)
        this.onChangeState = this.onChangeState.bind(this);
    }

    static get RELAY_DISABLED() {
        return 0;
    }

    static get RELAY_ON() {
        return 1;
    }

    static get RELAY_OFF() {
        return 2;
    }

    static getStateTitle(state) {
        let result = 'авто';
        if (state === Relay.RELAY_ON) {
            result = 'включено'
        } else if (state === Relay.RELAY_OFF) {
            result = 'отключено'
        }
        return result;
    }

    onChangeState(state) {
        this.props.onChangeState(this.props.rp, this.props.command, state);
    }

    render() {
        const title = this.props.title || 'Реле';
        const btnToolbarClassName = classNames('float-right', this.props.btnToolbarClassName);
        return <div>
            <h5>{title}:&nbsp;<b>{Relay.getStateTitle(this.props.state)}</b></h5>
            <ButtonToolbar className={btnToolbarClassName}>
                <ToggleButtonGroup className="mr-2" type="radio" name="options" value={this.props.state}
                                   defaultValue={0}
                                   onChange={this.onChangeState}>
                    <ToggleButton value={1}>Вкл.</ToggleButton>
                    <ToggleButton value={2}>Выкл.</ToggleButton>
                </ToggleButtonGroup>
                <DropdownButton title="Управление">
                    <DropdownItem as="button">Задействовать</DropdownItem>
                    <DropdownItem as="button">Исключить</DropdownItem>
                </DropdownButton>

                {/*<ToggleButtonGroup size="sm" type="checkbox" value={this.props.state} defaultValue={1}*/}
                {/*                   onChange={this.onChangeState}>*/}
                {/*    <ToggleButton value={0}>Откл.</ToggleButton>*/}
                {/*</ToggleButtonGroup>*/}
                {/*<ButtonGroup size="sm">*/}
                {/*<ToggleButton  variant="info" onClick={this.disable} >Откл.</ToggleButton >*/}
                {/*</ButtonGroup>*/}
            </ButtonToolbar>
        </div>;
    }
}

export default Relay;