import React, {Component} from "react";
import ModuleCore from "../components/module-core";
import CommonStat from "../components/react-common-stat";
import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";

class Outside extends Component {

    constructor(props) {
        super(props)
    }

    render() {
        return <ModuleCore title={"Метеостанция"} activeTab={0}>
            <Container type={"info"} label={"Инфо"}>
                <Row>
                    <Col>
                        <CommonStat
                            value={22}
                            valueMin={1}
                            valueMax={28}
                            valueHour={4}
                            formatValue={"+0.0"}
                            unitName={"℃"}
                            title={"Температура:"}
                        />
                    </Col>
                    <Col>
                        <CommonStat
                            value={30}
                            valueMin={12}
                            valueMax={64}
                            valueHour={-16}
                            unitName={"%"}
                            title={"Влажность:"}
                        />
                    </Col>
                    <Col>3 of 3</Col>
                </Row>
            </Container>
        </ModuleCore>;
    }
}

export default Outside;