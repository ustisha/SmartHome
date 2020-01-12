import '../css/app.scss';

import React, {Component} from 'react';
import ReactDom from 'react-dom';
import {Col, Container, Row} from "react-bootstrap";
import Outside from "./controls/outside";
import ColdChamber from "./controls/cold-chamber";
import {BrowserRouter as Router, Switch, Route, Link, Redirect} from "react-router-dom";
import SignIn from "./controls/sign-in";

import authStore from "./stores/Auth";
import {inject, observer, Provider} from "mobx-react";
import {observable} from "mobx";
import Spinner from "react-bootstrap/Spinner";
import Home from "./controls/home";

const stores = {
    authStore
};

@inject("authStore")
class PrivateRoute extends Component {
    render() {
        return <Route
            render={({location}) =>
                this.props.authStore.currentUser ? (
                    this.props.children
                ) : (
                    <Redirect
                        to={{
                            pathname: "/signin",
                            state: {from: location}
                        }}
                    />
                )
            }
        />
    }
}

@inject("authStore")
@observer
class App extends Component {

    @observable loaded = false;

    componentDidMount() {
        this.props.authStore.pullUser().then(() => {
            this.loaded = true;
        });
    }

    render() {
        if (!this.loaded) {
            return <Container>
                <Row className="justify-content-md-center mt-5">
                    <Col lg={5} md={6}>
                        <Spinner animation="border" role="status">
                            <span className="sr-only">Загрузка...</span>
                        </Spinner>
                    </Col>
                </Row>
            </Container>;
        }

        return <Router>
            <Switch>
                <Route path="/signin">
                    <Container>
                        <Row className="justify-content-md-center">
                            <Col lg={10} md={12}>
                                <SignIn/>
                            </Col>
                        </Row>
                    </Container>
                </Route>
                <PrivateRoute path="/">
                    <Container fluid={true}>
                        <Row className="justify-content-md-center">
                            <Col lg={10} md={12}>
                                <Home/>
                                <Outside/>
                                <ColdChamber/>
                            </Col>
                        </Row>
                    </Container>
                </PrivateRoute>
            </Switch>
        </Router>;
    }
}

ReactDom.render((
        <Provider {...stores}>
            <App/>
        </Provider>),
    document.getElementById('root')
);