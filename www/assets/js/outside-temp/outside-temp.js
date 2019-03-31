import React, {Component} from 'react';
import {observable} from "mobx";
import {observer} from "mobx-react";
import {Card, Button, ListGroup} from "react-bootstrap";
import Moment from "react-moment";

class OutsideTemp {

    @observable temperature_DS18B20 = 0;
    @observable temperature_BME280 = 0;
    @observable humidity = 0;
    @observable pressure = 0;
    @observable lux = 0;
    @observable vcc = 0;
    @observable lastUpdate = 0;

    constructor() {
        this.reload();
    }

    reload() {
        fetch('/outside/temp')
        .then((resp) => {
            return resp.json();
        })
        .then((data) => {
            for (let prop in data) {
                this[prop] = data[prop];
            }
        });
    }

}

const outsideTemp = new OutsideTemp();

@observer
export class OutsideTempView extends Component {
    render() {
        return <Card style={{width: '18rem'}}>
            <Card.Header>
                <h4>Метеостанция</h4><br/>
            </Card.Header>
            <Card.Body>
                <Card.Subtitle className="mb-2 text-muted">
                    <Moment format="DD.MM.YYYY HH:mm">{outsideTemp.lastUpdate}</Moment>&nbsp;/&nbsp;{outsideTemp.vcc}V
                </Card.Subtitle>
                <Card.Text>
                    <ListGroup>
                        <ListGroup.Item variant="primary">
                            Температура 1: <b>{outsideTemp.temperature_DS18B20}℃</b>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Температура 2: <b>{outsideTemp.temperature_BME280}℃</b>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Влажность: <b>{outsideTemp.humidity}%</b>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Давление: <b>{outsideTemp.pressure} mmHg</b>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Освещенность: <b>{outsideTemp.lux} Lux</b>
                        </ListGroup.Item>
                    </ListGroup>
                </Card.Text>
                <Button onClick={outsideTemp.reload.bind(outsideTemp)} variant="primary" size="lg">Обновить</Button>
            </Card.Body>
        </Card>;
    }
}