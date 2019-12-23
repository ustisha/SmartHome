import '../css/app.scss';

import React, {Component} from 'react';
import ReactDom from 'react-dom';
import {Card, Col, Container, Row, Tab, Tabs} from "react-bootstrap";

import {OutsideTempView} from "./modules/outside-temp/main";
import {GreenhouseView} from "./modules/greenhouse/main";

class App extends Component {
    render() {
        return (<Container>
            <Row noGutters={false}>
                <Col xs={5} md={3}><OutsideTempView/></Col>
                <Col xs={5} md={4}><GreenhouseView/></Col>
            </Row>
            <Row noGutters={true}>
                <Col>
                    {/*<Card className="mt-1">*/}
                    {/*    <Card.Header>*/}
                    {/*        <h4>События</h4>*/}
                    {/*    </Card.Header>*/}
                    {/*    <Card.Body className="log-panel">*/}
                    {/*        <LogTableView/>*/}
                        {/*</Card.Body>*/}
                    {/*</Card>*/}
                </Col>
            </Row>
        </Container>);
    }
}

ReactDom.render(<App/>, document.getElementById('root'));