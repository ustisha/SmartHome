import '../css/app.scss';

import React, {Component} from 'react';
import ReactDom from 'react-dom';
import {Col, Container, Row} from "react-bootstrap";
import {observer} from 'mobx-react';

import {OutsideTempView} from "./outside-temp/outside-temp";


@observer
class App extends Component {
    render() {
        return (<Container>
                <Row>
                    <Col xs="auto"><OutsideTempView/></Col>
                </Row>
            </Container>);
    }
}

ReactDom.render(<App/>, document.getElementById('root'));