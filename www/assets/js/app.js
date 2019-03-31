/*
 * Welcome to your app's main JavaScript file!
 *
 * We recommend including the built version of this JavaScript file
 * (and its CSS file) in your base layout (base.html.twig).
 */

// any CSS you require will output into a single css file (app.css in this case)
import '../css/app.scss';

//const $ = require('jquery');
// this "modifies" the jquery module: adding behavior to it
// the bootstrap module doesn't export/return anything
//require('bootstrap');

// or you can include specific pieces
// require('bootstrap/js/dist/tooltip');
// require('bootstrap/js/dist/popover');

//$(document).ready(function() {
//    $('[data-toggle="popover"]').popover();
//});

import React, {Component} from 'react';
import ReactDom from 'react-dom';
import DevTools from "mobx-react-devtools";
import {observer} from 'mobx-react';

import {OutsideTempView} from "./outside-temp/outside-temp";

@observer
class App extends Component {
    render() {
        return <div>
            <OutsideTempView/>
            <DevTools/>
        </div>
    }
}

ReactDom.render(<App/>, document.getElementById('root'));