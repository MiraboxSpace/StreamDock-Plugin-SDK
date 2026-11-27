/// <reference path="./utils/common.js" />
/// <reference path="./utils/axios.js" />

const plugin = new Plugins("xxx");

// 操作一
plugin.action1 = new Actions({
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
    default: {},
    _willAppear({ context }) {
        window.socket.setTitle(context, "Hello world!");
    },
    _willDisappear({ context }) { },
    dialRotate(data) {//旋钮旋转
        console.log(data);
    },
    dialDown(data) {//旋钮按下
        console.log(data);
    }
});
// Plugin-wide events (messages may have no action/context).
plugin.applicationDidLaunch = (data) => { };
plugin.applicationDidTerminate = (data) => { };
plugin.deviceDidConnect = (data) => { };
plugin.deviceDidDisconnect = (data) => { };
plugin.systemDidWakeUp = (data) => { };
