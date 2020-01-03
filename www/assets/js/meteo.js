// any CSS you require will output into a single css file (app.css in this case)
import '../css/meteo.scss';


import React, {Component} from 'react';
import ReactDom from 'react-dom';
import {Col, Container, Row} from "react-bootstrap";
import {observer} from 'mobx-react';
import {Meteo} from "./controls/meteo";


@observer
class App extends Component {
    render() {
        return (<Container>
                <Row className="justify-content-md-center">
                    <Col xs lg="2"/>
                    <Col xs="auto"><Meteo/></Col>
                    <Col xs lg="2"/>
                </Row>
            </Container>);
    }
}

ReactDom.render(<App/>, document.getElementById('root'));