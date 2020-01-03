import {action, observable} from "mobx";

class AuthStore {

    @observable currentUser;
    @observable errors;

    get currentUser() {
        return this.currentUser && this.currentUser.username ? this.currentUser : undefined;
    }

    get username() {
        return this.currentUser ? this.currentUser.username : undefined;
    }

    @action login(username, password) {
        return fetch('/login', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json;charset=utf-8'
            },
            body: JSON.stringify({
                username: username,
                password: password
            })
        })
        .then((response) => {
            return response.json();
        })
        .then(action((user) => {
            if (user && user['username']) {
                this.currentUser = user;
            }
        }))
        .finally(() => {

        });
    }

    @action pullUser() {
        return fetch('/user')
        .then((response) => {
            return response.json();
        })
        .then(action((user) => {
            if (user && user['username']) {
                this.currentUser = user;
            }
        }))
        .finally(() => {

        });
    }

    @action logout() {
        return fetch('/logout')
        .then((response) => {
            return response.json();
        })
        .then(action(() => {
            this.currentUser = undefined;
        }));

    }
}

export default new AuthStore();