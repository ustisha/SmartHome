import React, {Component} from 'react';
import {Card} from "react-bootstrap";
import Nav from "react-bootstrap/Nav";
import classNames from "classnames";
import {observer} from "mobx-react";
import {observable} from "mobx";

@observer
class ModuleCore extends Component {

    controls;

    onTabClick(selected) {
        this.controls.forEach(function(ctrl, idx) {
            ctrl.active = selected === idx;
        });
    }

    constructor(props) {
        super(props);

        this.controls = observable([]);
        React.Children.forEach(props.children, (child, idx) => {
            this.controls.push({
                idx: idx,
                title: child.props.label,
                active: idx === (props.activeTab !== undefined ? props.activeTab : 0)
            });
        });
    }

    getValues() {
        if (this.props.getValues) {
            this.props.getValues.call(this);
        }
    }

    render() {
        const cardClass = classNames('mt-2', 'border-color-4', this.props.className);
        const tabs = React.Children.map(this.props.children, (child, idx) => {
            return (
                <Nav.Item>
                    <Nav.Link onClick={this.onTabClick.bind(this, idx)} active={this.controls[idx].active}>
                        {this.controls[idx].title}
                    </Nav.Link>
                </Nav.Item>
            );
        });
        const panels = React.Children.map(this.props.children, (child, idx) => {
            return React.cloneElement(child, {
                hidden: !this.controls[idx].active
            })
        });

        return <Card className={cardClass}>
            <Card.Header className={"color-3"}>
                <div className="float-left text-color-4 pl-2">
                    <h4 className="mb-0 mt-1">
                        {this.props.title}&nbsp;<a className="cursor-pointer" onClick={this.getValues.bind(this)}>{'\u27f3'}</a>
                    </h4>
                </div>
                <div className="float-right">
                    <Nav className="justify-content-end" variant="tabs" defaultActiveKey="#first">
                        {tabs}
                    </Nav>
                </div>
            </Card.Header>
            <Card.Body className={"color-2"}>
                {panels}
            </Card.Body>
        </Card>;
    }
}

export default ModuleCore;