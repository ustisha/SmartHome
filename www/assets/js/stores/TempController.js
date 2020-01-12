import {observable} from "mobx";

export class TempControllerStore {

    @observable mode = 0;

    @observable timeout = 0;

    @observable down_limit = 0;

    @observable up_limit = 0;

    @observable servo_0 = 0;
}