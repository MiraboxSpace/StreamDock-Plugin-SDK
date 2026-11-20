/// <reference path="../utils/common.js" />
/// <reference path="../utils/action.js" />

/**
 * 基础参数说明:
 *      @global websocket uuid action context settings lang
 *      @settings local back 是否国际化 | 是否自行回显
 *      @policy dom propEvent 缓存文档元素 | 软件触发事件 - 策略模式
 * =======================================================================>
 */

const $local = true, $back = false, $dom = {
    main: $('.sdpi-wrapper')
};

const $propEvent = {
    didReceiveSettings(data) {
        console.log(data);
     },
    sendToPropertyInspector(data) { }
};
