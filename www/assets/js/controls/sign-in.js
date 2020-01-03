import {inject, observer} from "mobx-react";
import React, {Component} from "react";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";
import Form from "react-bootstrap/Form";
import Button from "react-bootstrap/Button";
import classNames from "classnames";
import {withRouter} from "react-router-dom";
import ModuleCore from "../components/module-core";
import {computed, observable} from "mobx";

@inject("authStore")
@withRouter
@observer
class SignIn extends Component {

    @observable username;
    @observable password;
    @observable inProcess;

    constructor(props) {
        super(props);
    }

    componentDidMount() {
        if (this.props.authStore.currentUser) {
            this.props.history.replace('/');
        }
    }

    @computed get isDisabled() {
        return this.inProcess;
    }

    /**
     * @param e
     */
    handleSubmit = e => {
        e.preventDefault();
        let {from} = this.props.location.state || {from: {pathname: "/"}};
        this.inProcess = true;
        this.props.authStore.login(this.username, this.password).then(() => {
            this.inProcess = false;
            this.props.history.replace(from);
        });
    };

    handleUsernameChange = e => {
        this.username = e.target.value;
    };

    handlePasswordChange = e => {
        this.password = e.target.value;
    };

    render() {
        const cardClass = classNames('mt-2', 'border-color-4', this.props.className);

        return <ModuleCore title={"Умный дом"} activeTab={0}>
            <Container type={"info"} label={"Вход"} fluid={true}>
                <Row className="justify-content-md-center">
                    <Col xs lg="2"/>
                    <Col xs="6">
                        <Form className={cardClass} disabled={this.isDisabled} onSubmit={this.handleSubmit}>
                            <Form.Group as={Row} controlId="formBasicEmail">
                                <Form.Label column sm="2"><b>Логин</b></Form.Label>
                                <Col sm="10">
                                    <Form.Control
                                        onChange={this.handleUsernameChange}
                                        disabled={this.isDisabled}
                                        type="email"
                                        placeholder="Адрес электронной почты"/>
                                </Col>
                            </Form.Group>
                            <Form.Group as={Row} controlId="formBasicPassword">
                                <Form.Label column sm="2"><b>Пароль</b></Form.Label>
                                <Col sm="10">
                                    <Form.Control
                                        onChange={this.handlePasswordChange}
                                        disabled={this.isDisabled}
                                        type="password"
                                        placeholder="Пароль"/>
                                </Col>
                            </Form.Group>
                            <Button disabled={this.isDisabled} variant="primary" type="submit">
                                Вход
                            </Button>
                        </Form>
                    </Col>
                    <Col xs lg="2"/>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default SignIn;