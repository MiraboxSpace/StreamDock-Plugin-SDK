import { defineStore } from 'pinia';

// 插件钩子
export const usePluginStore = defineStore('pluginStore', () => {
  document.title = window.argv[3].plugin.uuid + ' - 插件';

  // 定时器线程
  const Timer = new Worker('interval.js');
  const TimerSubscribe: { uuid: string; fn: () => void }[] = [];
  Timer.addEventListener('message', ({ data: { event, uuid } }: { data: { event: string; uuid: string } }) => {
    const subIndex = TimerSubscribe.findIndex((item) => item.uuid === uuid);
    subIndex !== -1 && event === 'setInterval' && TimerSubscribe[subIndex].fn();
  });

  // 创建定时器
  const Interval = (uuid: string, delay: number, fn: () => void) => {
    TimerSubscribe.findIndex((item) => item.uuid === uuid) === -1 && TimerSubscribe.push({ uuid, fn });
    Timer.postMessage({ event: 'setInterval', uuid, delay });
  };

  // 销毁定时器
  const Unterval = (uuid: string) => {
    const subIndex = TimerSubscribe.findIndex((item) => item.uuid === uuid);
    subIndex !== -1 && TimerSubscribe.splice(subIndex, 1);
    Timer.postMessage({ event: 'clearInterval', uuid });
  };

  // 连接软件
  const message = ref<StreamDock.Message>();
  const server = new WebSocket('ws://127.0.0.1:' + window.argv[0]);
  server.onopen = () => server.send(JSON.stringify({ event: window.argv[2], uuid: window.argv[1] }));
  server.onmessage = (e) => {
    message.value = JSON.parse(e.data)
    // console.log(e.data)
  };

  //全局设置数据
  const globalSettings = ref<any>();
  const devices = ref<Set<string>>(new Set());
  const userInfo = ref<any>({});
  //设置全局设置数据
  const setGlobalSettings = (payload: any) => {
    server.send(JSON.stringify({ event: 'setGlobalSettings', context: window.argv[1], payload }));
    globalSettings.value = payload;
  };

  //获取全局设置数据
  const getGlobalSettings = () => {
    server.send(JSON.stringify({ event: 'getGlobalSettings', context: window.argv[1] }));
  }

  // Profile names must match manifest Profiles[].Name; pages start at 1.
  const switchToProfile = (device: string, profile = '', page?: number) => {
    server.send(JSON.stringify({ event: 'switchToProfile', context: window.argv[1], device,
      payload: { profile, ...(page === undefined ? {} : { page }) } }));
  };
  const logMessage = (message: string) => {
    server.send(JSON.stringify({ event: 'logMessage', payload: { message } }));
  };

  // 设置背景
  const setBackground = (img: string, device: string, clearIcon = true) => {
    server.send(JSON.stringify({
      "event": "setBackground",
      "device": device,
      "payload": {
        "image": img,
        "clearIcon": clearIcon
      }
    }));
  };

  // 通知软件背景停了
  const stopBackground = (device: string) => {
    server.send(JSON.stringify({
      "event": "stopBackground",
      "device": device,
      "payload": {
        "clearIcon": true// 如果设置背景的时候设置了清除图标true就需要带上这个参数并设置为true(告诉软件需要恢复图标)
      }
    }));
  };

  // 获取用户信息
  const getUserInfo = () => {
    server.send(JSON.stringify({
      "event": "getUserInfo"
    }));
  };


  // 操作数据存储
  class Actions {
    settings: {};
    action: string;
    context: string;
    title: string;
    titleParameters = {} as titleParameters;
    constructor(action: string, context: string, settings: {}) {
      this.action = action;
      this.context = context;
      this.settings = settings;
    }

    // 添加操作
    static list: Actions[] = [];
    static addAction = (action: string, context: string, settings: {}) => {
      const instance = new Actions(action, context, settings);
      this.list.push(instance);
      return instance;
    };

    // 删除操作
    static delAction = (context: string) => {
      const i = this.list.findIndex((item) => item.context === context);
      i !== -1 && this.list.splice(i, 1);
    };

    // 获取操作数据
    static getAction = (context: string) => {
      return Actions.list.find((item) => item.context === context);
    };

    // 获取所有操作数据
    static getActions = (action: string) => {
      return this.list.filter((item) => item.action === action);
    };

    // 通知属性检查器
    sendToPropertyInspector = (payload: any) => {
      server.send(JSON.stringify({ event: 'sendToPropertyInspector', action: this.action, context: this.context, payload }));
    };

    triggerKey = (keyList: { key: number; downUp: 0 | 1 }[]) => {
      server.send(JSON.stringify({ event: 'triggerKey', context: this.context, payload: { keyList } }));
    };
    showOk = () => server.send(JSON.stringify({ event: 'showOk', context: this.context }));
    showAlert = () => server.send(JSON.stringify({ event: 'showAlert', context: this.context }));

    // 切换状态
    setState = (state: number) => {
      server.send(JSON.stringify({ event: 'setState', context: this.context, payload: { state } }));
    };

    // 设置标题
    setTitle = (title: string) => {
      server.send(JSON.stringify({ event: 'setTitle', context: this.context, payload: { title, target: 0 } }));
    };

    // 设置图片
    setImage = (url: string) => {
      if (url.includes('data:')) {
        server.send(JSON.stringify({ event: 'setImage', context: this.context, payload: { target: 0, image: url } }));
        return;
      }
      const image = new Image();
      image.src = url;
      image.onload = () => {
        const canvas = document.createElement('canvas');
        canvas.width = image.naturalWidth;
        canvas.height = image.naturalHeight;
        const ctx = canvas.getContext('2d');
        ctx.drawImage(image, 0, 0);
        server.send(JSON.stringify({ event: 'setImage', context: this.context, payload: { target: 0, image: canvas.toDataURL('image/png') } }));
      };
    };

    // 设置持久化数据
    setSettings = (payload: any) => {
      this.settings = payload;
      server.send(JSON.stringify({ event: 'setSettings', context: this.context, payload }));
    };

    // 默认浏览器打开
    openUrl = (url: string) => {
      server.send(JSON.stringify({ event: 'openUrl', payload: { url } }));
    };

    // 注册屏保事件（告诉软件需要独占屏保，可以自行维护进入屏保的时间（unLockScreen唤醒后仍然可以自行根据设置的锁屏时间触发锁屏），当收到unRegistrationScreenSaverEvent后则标识品保独占给到了其他的插件此插件应该停止屏保的逻辑（清除进入屏保的定时器））
    registrationScreenSaverEvent = (device: string) => {
      server.send(
        JSON.stringify({
          event: 'registrationScreenSaverEvent',
          device: device,
          context: this.context,
        }),
      );
    };
  }

  class EventEmitter {

    events: { [key: string]: any[] };
    constructor() {
      this.events = {};
    }

    // 订阅事件
    subscribe(event: string, listener: Function) {
      if (!this.events[event]) {
        this.events[event] = [];
      }
      this.events[event].push(listener);
    }

    // 取消订阅
    unsubscribe(event: string) {
      if (!this.events[event]) return;

      this.events[event] = null;
    }

    // 发布事件
    emit(event: string, data: any) {
      if (!this.events[event]) return;
      this.events[event].forEach(listener => listener(data));
    }
  }

  // 非响应式事件总线，仅供全局使用
  const eventEmitter = new EventEmitter();

  return {
    message,
    globalSettings,
    eventEmitter, // 仅限逻辑使用，不用于组件响应
    devices,
    userInfo,
    Interval,
    Unterval,
    setGlobalSettings,
    getGlobalSettings,
    setBackground,
    stopBackground,
    getUserInfo,
    switchToProfile,
    logMessage,
    ActionArr: Actions.list,
    getAction: Actions.getAction,
    addAction: Actions.addAction,
    delAction: Actions.delAction,
    getActions: Actions.getActions
  };
});

// !! 请勿更改此处 !!
type MessageTypes = { plugin: StreamDock.PluginMessage; action: StreamDock.ActionMessage };
type payload = {
  settings: any;
}
export const useWatchEvent = <T extends keyof MessageTypes>(type: T, MessageEvents: MessageTypes[T]) => {
  const plugin = usePluginStore();


  if (type === 'plugin') {
    return watch(
      () => plugin.message,
      () => {
        if (!plugin.message) return;
        if (plugin.message.action) return;
        MessageEvents[plugin.message.event]?.(JSON.parse(JSON.stringify(plugin.message)));
        if (plugin.message.event === 'didReceiveGlobalSettings') {
          plugin.globalSettings = (plugin.message.payload as payload).settings;
        } else if (plugin.message.event === 'deviceDidConnect') {
          // console.log('设备已连接:', plugin.message);
          plugin.devices.add(plugin.message.device);
        } else if (plugin.message.event === 'deviceDidDisconnect') {
          // console.log('deviceDidDisconnect:', plugin.message);
          plugin.devices.delete(plugin.message.device);
        } else if (plugin.message.event === 'sendUserInfo') {
          plugin.userInfo = plugin.message.payload;
        }
      },
      { flush: 'sync' }
    );
  }
  const Events: StreamDock.ActionMessage = {
    willAppear({ action, context, payload }) {
      !plugin.getAction(context) && plugin.addAction(action, context, payload.settings);
    },
    willDisappear({ context }) {
      plugin.delAction(context);
    },
    didReceiveSettings({ context, payload }) {
      const action = plugin.getAction(context);
      action.settings = payload.settings;
    },
    titleParametersDidChange({ context, payload }) {
      const action = plugin.getAction(context);
      action.title = payload.title;
      action.titleParameters = payload.titleParameters;
    }
  };
  return watch(
    () => plugin.message,
    () => {
      if (!plugin.message) return;
      if (plugin.message.action !== MessageEvents['ActionID']) return;
      const data = JSON.parse(JSON.stringify(plugin.message));
      Events[plugin.message.event]?.(data);
      MessageEvents[plugin.message.event]?.(data);
      if (plugin.message.event === 'deleteAction') plugin.delAction(data.context);
    },
    { flush: 'sync' }
  );
};