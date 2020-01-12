import {observable} from "mobx";

export class LightControllerStore {

    @observable mode = 0;

    @observable timeout = 0;

    @observable relay_0 = 0;

    @observable activity_ratio = 0;

    @observable activity_limit = 0;

    @observable recall_ratio = 0;

    @observable recall_timeout = 0;
}