const Timer = new Worker('./utils/worker.js');
const TimerSubscribe = { setTimeout: {}, setInterval: {} };
Timer.addEventListener('message', function ({ data }) {
    TimerSubscribe[data.event][data.id]?.();
});

class Actions {
    constructor(data) {
        this.data = {};
        this.default = {};
        Object.assign(this, data);
    }
    // 属性检查器显示时
    static currentAction = null;
    static currentContext = null;
    propertyInspectorDidAppear(data) {
        const { action, context } = data;
        Actions.currentAction = action;
        Actions.currentContext = context;
        this._propertyInspectorDidAppear?.(data);
    }
    // 初始化数据
    willAppear(data) {
        const { context, payload: { settings } } = data;
        this.data[context] = Object.assign({ ...this.default }, settings);
        this._willAppear?.(data);
    }
    // 行动销毁
    willDisappear(data) {
        this._willDisappear?.(data);
        delete this.data[data.context];
    }
}

class Plugins {
    constructor(name) {
        this.name = name;
    }
    // 延时器/清除延时
    clearTimeout(id) { Timer.postMessage({ event: 'clearTimeout', id }); }
    setTimeout(id, callback, delay) {
        this.clearTimeout(id);
        TimerSubscribe.setTimeout[id] = callback;
        Timer.postMessage({
            event: 'setTimeout',
            id, delay
        });
    }
    // 定时器/清除定时器
    clearInterval(id) { Timer.postMessage({ event: 'clearInterval', id }); }
    setInterval(id, callback, delay) {
        this.clearInterval(id);
        TimerSubscribe.setInterval[id] = callback;
        Timer.postMessage({
            event: 'setInterval',
            id, delay
        });
    }
}

// 软件通信
window.connectElgatoStreamDeckSocket = function () {
    const uuid = arguments[1], event = arguments[2];
    window.info = JSON.parse(arguments[3]);
    window.socket = new WebSocket("ws://127.0.0.1:" + arguments[0]);


    // Protocol commands; profile page numbers start at 1.
    WebSocket.prototype.switchToProfile = function (device, profile = '', page) {
        this.send(JSON.stringify({ event: 'switchToProfile', context: uuid, device, payload: { profile, ...(page === undefined ? {} : { page }) } }));
    };
    WebSocket.prototype.triggerKey = function (context, keyList) {
        this.send(JSON.stringify({ event: 'triggerKey', context, payload: { keyList } }));
    };
    WebSocket.prototype.logMessage = function (message) {
        this.send(JSON.stringify({ event: 'logMessage', payload: { message } }));
    };
    WebSocket.prototype.setGlobalSettings = function (payload) {
        this.send(JSON.stringify({ event: 'setGlobalSettings', context: uuid, payload }));
    };
    WebSocket.prototype.getGlobalSettings = function () {
        this.send(JSON.stringify({ event: 'getGlobalSettings', context: uuid }));
    };
    WebSocket.prototype.showOk = function (context) {
        this.send(JSON.stringify({ event: 'showOk', context }));
    };
    WebSocket.prototype.showAlert = function (context) {
        this.send(JSON.stringify({ event: 'showAlert', context }));
    };

    // 打开网页
    WebSocket.prototype.openUrl = function (url) {
        this.send(JSON.stringify({
            event: "openUrl",
            payload: { url }
        }));
    };

    // 与当前属性检查器通信
    WebSocket.prototype.sendToPropertyInspector = function (payload) {
        this.send(JSON.stringify({
            payload,
            event: "sendToPropertyInspector",
            action: Actions.currentAction,
            context: Actions.currentContext
        }));
    };

    // 保存持久化数据
    WebSocket.prototype.setSettings = function (context, payload) {
        this.send(JSON.stringify({
            event: "setSettings",
            context, payload
        }));
    };

    // 设置背景
    WebSocket.prototype.setImage = function (context, url) {
        const image = new Image();
        image.src = url;
        image.onload = () => {
            const canvas = document.createElement("canvas");
            canvas.width = image.naturalWidth;
            canvas.height = image.naturalHeight;
            const ctx = canvas.getContext("2d");
            ctx.drawImage(image, 0, 0);
            this.send(JSON.stringify({
                event: "setImage",
                context, payload: {
                    target: 0,
                    image: canvas.toDataURL("image/png")
                }
            }));
        };
    };

    // 设置标题
    WebSocket.prototype.setTitle = function (context, str, row = 0, num = 6) {
        let newStr = '';
        if (row) {
            let nowRow = 1, strArr = str.split('');
            strArr.forEach((item, index) => {
                if (nowRow < row && index >= nowRow * num) {
                    nowRow++;
                    newStr += '\n';
                }
                if (nowRow <= row && index < nowRow * num) {
                    newStr += item;
                }
            });
            if (strArr.length > row * num) {
                newStr = newStr.substring(0, newStr.length - 1);
                newStr += '..';
            }
        }
        this.send(JSON.stringify({
            event: "setTitle",
            context, payload: {
                target: 0,
                title: newStr || str
            }
        }));
    };

    // 设置状态
    WebSocket.prototype.setState = function (context, state) {
        this.send(JSON.stringify({
            event: "setState",
            payload: { state },
            context
        }));
    };

    window.socket.onopen = () => window.socket.send(JSON.stringify({ uuid, event }));
    window.socket.onmessage = e => {
        const data = JSON.parse(e.data);
        plugin[data.action?.split('.').pop()]?.[data.event]?.(data);
        plugin[data.event]?.(data);
    };
};