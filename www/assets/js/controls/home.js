import React, {Component} from "react";
import ModuleCore from "../components/module-core";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import {inject, observer} from "mobx-react";
import Button from "react-bootstrap/Button";
import {withRouter} from "react-router-dom";

@inject("authStore")
@withRouter
@observer
class Home extends Component {

    constructor(props) {
        super(props)
    }

    handleLogout = e => {
        this.props.authStore.logout().then(() => {
            this.props.history.replace("/")
        });
    };

    render() {
        return <ModuleCore title={"Дом"} activeTab={0}>
            <Container type={"info"} label={"Инфо"}>
                <Row>
                    <Col>
                        Инфо 1
                    </Col>
                    <Col>
                        Инфо 2
                    </Col>
                    <Col>
                        Инфо 3
                    </Col>
                </Row>
            </Container>
            <Container type={"settings"} label={"Настройки"}>
                <Row>
                    <Col>Настройка 1</Col>
                    <Col>Настройка 2</Col>
                    <Col>
                        <b>Пользователь:</b> {this.props.authStore.username}
                        <Button className="float-right" size={"sm"} onClick={this.handleLogout}>Выход</Button>
                    </Col>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default Home;