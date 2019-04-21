import '../css/app.scss';

import React, {Component} from 'react';
import ReactDom from 'react-dom';
import {Card, Col, Container, Row, Tab, Tabs} from "react-bootstrap";

import {OutsideTempView} from "./outside-temp/outside-temp";
import {LogTableView} from "./controls/log-table";

class App extends Component {
    render() {
        return (<Container>
            <Row noGutters={true}>
                <Col xs={5} md={3}><OutsideTempView/></Col>
                <Col>
                    <Tabs className="mt-1" defaultActiveKey="home" id="uncontrolled-tab-example">
                        <Tab eventKey="home" title="Дом">
                            FF
                        </Tab>
                        <Tab eventKey="greenHouse" title="Теплица">
                            AA
                        </Tab>
                        <Tab eventKey="contact" title="Гараж" disabled>
                            DD
                        </Tab>
                    </Tabs>
                </Col>
            </Row>
            <Row noGutters={true}>
                <Col>
                    <Card className="mt-1">
                        <Card.Header>
                            <h4>События</h4>
                        </Card.Header>
                        <Card.Body className="log-panel">
                            <LogTableView/>
                        </Card.Body>
                    </Card>
                </Col>
            </Row>
        </Container>);
    }
}

ReactDom.render(<App/>, document.getElementById('root'));