import React, {Component} from 'react';
import {observer} from "mobx-react";
import BootstrapTable from 'react-bootstrap-table-next';
import paginationFactory, {PaginationProvider, PaginationListStandalone} from 'react-bootstrap-table2-paginator';
import {observable} from "mobx";
import Moment from "react-moment";
import {RadioLog} from "../net/RadioLog";

class LogTable {

    columns = [
        {
            dataField: 'date',
            text: 'Дата',
            formatter: function rankFormatter(cell) {
                return (<Moment format="DD.MM.YYYY HH:mm:ss">{cell}</Moment>);
            }
        }, {
            dataField: 'sender',
            text: 'Отправитель',
            formatter: function (cell) {
                return new RadioLog().getSenderTitle(cell);
            }
        }, {
            dataField: 'senderPort',
            text: 'Компонент',
            formatter: function (cell, row) {
                return new RadioLog().getSenderPortTitle(row.sender, cell);
            }
        }, {
            dataField: 'command',
            text: 'Комманда',
            formatter: function (cell) {
                return new RadioLog().getCommandTitle(cell);
            }
        }, {
            dataField: 'data',
            text: 'Значение',
            formatter: function (cell, row) {
                return new RadioLog().getCommandValueTitle(row.command, cell);
            }
        }
    ];
    @observable data = [];
    @observable page = 1;
    @observable length = 0;
    sizePerPage = 12;

    constructor() {
        this.loadData();
    }

    handleTableChange(type, {page}) {
        this.loadData(page);
    }

    loadData(page) {
        page = parseInt(page) || 1;
        fetch('/log/table/' + page.toString())
        .then((resp) => {
            return resp.json();
        })
        .then((data) => {
            this.data = data.rows;
            this.page = data.page;
            this.length = data.length;
        }).finally(() => {

        });
    }

}

const logTable = new LogTable();

const RemotePagination = ({data, columns, page, sizePerPage, onTableChange, totalSize}) => (<div>
    <PaginationProvider
        pagination={paginationFactory({
            custom: true,
            page,
            sizePerPage,
            totalSize
        })}
    >
        {({
              paginationProps, paginationTableProps
          }) => (<div>
            <BootstrapTable classes="log-table"
                            striped hover condensed remote
                            keyField="id"
                            data={data}
                            columns={columns}
                            onTableChange={onTableChange}
                            {...paginationTableProps}
            />
            <div>
                <PaginationListStandalone
                    {...paginationProps}
                />
            </div>
        </div>)}
    </PaginationProvider>
</div>);

@observer
export class LogTableView extends Component {
    render() {
        return <RemotePagination data={logTable.data}
                                 columns={logTable.columns}
                                 page={logTable.page}
                                 sizePerPage={logTable.sizePerPage}
                                 totalSize={logTable.length}
                                 onTableChange={logTable.handleTableChange.bind(logTable)}
        />
    }
}