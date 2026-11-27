const { Plugins, Actions, log, EventEmitter } = require('./utils/plugin');
const { execSync } = require('child_process');

const plugin = new Plugins('demo');


plugin.didReceiveGlobalSettings = ({ payload: { settings } }) => {
    log.info('didReceiveGlobalSettings', settings);
};

const createSvg = (text) => `<svg width="144" height="144" xmlns="http://www.w3.org/2000/svg">
    <text x="72" y="120" font-family="Arial" font-weight="bold" font-size="36" fill="white" text-anchor="middle"
        stroke="black" stroke-width="2" paint-order="stroke">
        ${text}
    </text>
</svg>`;
const timers = {};

plugin.demo = new Actions({
    // 旋钮释放；也兼容 N4 Pro 旧固件触摸屏。
    dialUp({ context, payload }) { },
    // 一次完整点击：payload.tapPos、hold、controller、width、height。
    touchTap({ context, payload }) { },
    // payload.phase: down / move / up / cancel；cancel 只清理临时状态。
    touchEvent({ context, payload }) { },
    // 仅用户删除实例触发；按 context 清理持续任务，不能用 willDisappear 代替。
    deleteAction({ context, payload }) { },
    // 标题参数改变。
    titleParametersDidChange({ context, payload }) { },
    // 属性检查器关闭。
    propertyInspectorDidDisappear({ context, payload }) { },
    default: {
    },
    async _willAppear({ context, payload }) {
        // log.info("demo: ", context);
        let n = 0;
        timers[context] = setInterval(async () => {
            const svg = createSvg(++n);
            plugin.setImage(context, `data:image/svg+xml;charset=utf8,${svg}`);
        }, 1000);
    },
    _willDisappear({ context }) {
        // log.info('willDisAppear', context)
        timers[context] && clearInterval(timers[context]);
    },
    _propertyInspectorDidAppear({ context }) {
    },
    sendToPlugin({ payload, context }) {
    },
    keyUp({ context, payload }) {
    },
    dialDown({ context, payload }) {},
    dialRotate({ context, payload }) {}
});
// Plugin-wide events (messages may have no action/context).
plugin.applicationDidLaunch = (data) => { };
plugin.applicationDidTerminate = (data) => { };
plugin.deviceDidConnect = (data) => { };
plugin.deviceDidDisconnect = (data) => { };
plugin.systemDidWakeUp = (data) => { };
