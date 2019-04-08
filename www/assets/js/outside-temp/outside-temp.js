import React, {Component} from 'react';
import {observable} from "mobx";
import {observer} from "mobx-react";
import {Card, Button, ListGroup, Spinner} from "react-bootstrap";
import Moment from "react-moment";

class OutsideTemp {

    @observable temperature_DS18B20 = 0;
    @observable temperature_BME280 = 0;
    @observable humidity = 0;
    @observable pressure = 0;
    @observable lux = 0;
    @observable vcc = 0;
    @observable lastUpdate = 0;
    @observable loading = false;

    constructor() {
        this.reload();
    }

    reload() {
        this.loading = true;
        fetch('/outside/temp')
        .then((resp) => {
            return resp.json();
        })
        .then((data) => {
            for (let prop in data) {
                this[prop] = data[prop];
            }
        }).finally(() => {
            this.loading = false;
        });
    }

}

const outsideTemp = new OutsideTemp();

@observer
export class OutsideTempView extends Component {
    render() {
        return <Card className="mt-1" style={{width: '20rem'}}>
            <Card.Header>
                <h4>Метеостанция</h4>
                <div className="card-header-info" hidden={outsideTemp.loading}>
                    <Moment format="DD.MM.YYYY HH:mm">{outsideTemp.lastUpdate}</Moment>&nbsp;/&nbsp;{outsideTemp.vcc}V
                </div>
                <div className="card-header-info" hidden={!outsideTemp.loading}>
                    ...
                </div>
            </Card.Header>
            <Card.Body>
                <Card.Text>
                    <ListGroup>
                        <ListGroup.Item variant="primary">
                            Температура 1:&nbsp;
                            <span hidden={outsideTemp.loading}><b>{outsideTemp.temperature_DS18B20}℃</b></span>
                            <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Температура 2:&nbsp;
                            <span hidden={outsideTemp.loading}><b>{outsideTemp.temperature_BME280}℃</b></span>
                            <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Влажность:&nbsp;
                            <span hidden={outsideTemp.loading}><b>{outsideTemp.humidity}%</b></span>
                            <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Давление:&nbsp;
                            <span hidden={outsideTemp.loading}><b>{outsideTemp.pressure} mmHg</b></span>
                            <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        </ListGroup.Item>
                        <ListGroup.Item variant="primary">
                            Освещенность:&nbsp;
                            <span hidden={outsideTemp.loading}><b>{outsideTemp.lux} Lux</b></span>
                            <span hidden={!outsideTemp.loading}>
                                <Spinner animation="border" variant="info" size="sm"/>
                            </span>
                        </ListGroup.Item>
                    </ListGroup>
                </Card.Text>
                <Button onClick={outsideTemp.reload.bind(outsideTemp)} variant="primary" size="lg">Обновить</Button>
            </Card.Body>
        </Card>;
    }
}