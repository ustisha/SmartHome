import React, {Component} from 'react';
import {Card, ListGroup} from "react-bootstrap";
import classNames from "classnames";
import Relay from "../components/react-relay";

class TempController extends Component {

    constructor(props) {
        super(props)
        this.onChangeState = this.onChangeState.bind(this);
    }

    onChangeState(rp, command, data) {
        this.props.onChangeState(rp, command, data);
    }

    render() {
        const cardClass = classNames('mt-1', 'ml-1', this.props.cardClass);
        return <Card className={cardClass}>
            <Card.Header><h5>Термостат</h5></Card.Header>
            <Card.Body>
                <ListGroup>
                    <ListGroup.Item variant="success">
                        <Relay title={"Реле 1"} rp={1002} command={40}
                               onChangeState={this.onChangeState}/>
                    </ListGroup.Item>
                    <ListGroup.Item variant="success">
                        <Relay title={"Реле 2"} rp={1002} command={41}
                               onChangeState={this.onChangeState}/>
                    </ListGroup.Item>
                </ListGroup>
            </Card.Body>
        </Card>;
    }
}

export default TempController;