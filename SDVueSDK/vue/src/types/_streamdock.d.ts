/// <reference types="vite/client" />

interface Window {
  argv: StreamDock.Argv; // 入口参数
  connectSDSocket(): void; // 入口函数
  connectMiraBoxSDSocket(): void; // 入口函数
  connectSocket(): void; //兼容Elgato接口
  connectElgatoStreamDeckSocket(): void; //兼容Elgato接口
  onFilePickerReturn(files: string): void; // 文件上传触发 => 用于获取绝对路径
}

// 标题参数
type titleParameters = EventPayload.titleParametersDidChange['payload']['titleParameters'];

// 事件负载
declare namespace EventPayload {
  // 操作持久化数据
  type didReceiveSettings = {
    action: string;
    event: string;
    device: string;
    context: string;
    payload: {
      settings: {};
      coordinates: {
        column: number;
        row: number;
      };
    };
    isInMultiAction: boolean;
  };
  type didReceiveGlobalSettings = {
    event: string;
    payload: {
      settings: {};
    };
  };
  type applicationDidLaunch = {
    event: string;
    payload: {
      application: string
    };
  };
  type applicationDidTerminate = {
    event: string;
    payload: {
      application: string
    };
  };
  type systemDidWakeUp = {
    event: string;
  };
  // 按下|抬起|触摸
  type keyDownUpTouchTap = {
    action: string;
    event: string;
    context: string;
    device: string;
    payload: {
      settings: {};
      coordinates: {
        column: number;
        row: number;
      };
      state: number;
      userDesiredState: number;
      isInMultiAction: boolean;
    };
  };
  // 创建|注销 操作
  type willAppearDisappear = {
    action: string;
    event: string;
    context: string;
    device: string;
    payload: {
      settings: {};
      coordinates: {
        column: number;
        row: number;
      };
      state: number;
      isInMultiAction: boolean;
    };
  };
  // 修改标题参数
  type titleParametersDidChange = {
    action: string;
    event: string;
    context: string;
    device: string;
    payload: {
      coordinates: {
        column: number;
        row: number;
      };
      settings: {};
      state: number;
      title: string;
      titleParameters: {
        fontFamily: string;
        fontSize: number;
        fontStyle: string;
        fontUnderline: boolean;
        showTitle: boolean;
        titleAlignment: string;
        titleColor: string;
      };
    };
  };
  // 连接|断开 设备
  type deviceDidConnectDisconnect = {
    event: string;
    device: string;
    deviceInfo: {
      name: string;
      type: number;
      size: {
        columns: number;
        rows: number;
      };
    };
  };
  // 显示|注销 属性检查器
  type propertyInspectorDidAppearDisappear = {
    action: string;
    event: string;
    context: string;
    device: string;
  };
  // 接收属性检查器消息
  type sendToPlugin = {
    event: string;
    action: string;
    context: string;
    payload: { [k: string]: any };
  };
  // 接收插件消息
  type sendToPropertyInspector = {
    action: string;
    event: string;
    context: string;
    payload: { [k: string]: any };
  };
  //旋钮按下抬起
  type KnobUPDown = {
    action: string;
    event: string;
    device: string;
    context: string;
    payload: {
      controller: "Knob";
      isInMultiAction: boolean;
      coordinates: {
        column: number;
        row: number;
      };
      userDesiredState: number;
      setting: {};
      state: number;
    };
  };
  //旋钮旋转
  type dialRotate = {
    action: string;
    event: string;
    device: string;
    context: string;
    payload: {
      pressed: boolean;
      coordinates: {
        column: number;
        row: number;
      };
      setting: {};
      ticks: number;
    }
  };
}

// 软件相关
declare namespace StreamDock {
  // 入口参数
  type Argv = [
    string,
    string,
    string,
    {
      application: {
        font: string;
        language: string;
        platform: string;
        platformVersion: string;
        version: string;
      };
      plugin: {
        uuid: string;
        version: string;
      };
    },
    {
      action: string;
      context: string;
      payload: {
        controller: string;
        coordinates: {
          column: number;
          row: number;
        };
        isInMultiAction: boolean;
        settings: any;
        state: number;
      };
    }
  ];

  // 消息通信
  type Message = {
    event: string;
    action?: string;
    context?: string;
    payload?: unknown;
  };

  // 属性检查器事件
  type ProperMessage = {
    didReceiveSettings?(this: ProperMessage, data: EventPayload.didReceiveSettings): void;
    didReceiveGlobalSettings?(this: ProperMessage, data: EventPayload.didReceiveGlobalSettings): void;
    sendToPropertyInspector?(this: ProperMessage, data: EventPayload.sendToPropertyInspector): void;
  };

  // 操作触发事件
  type ActionMessage = {
    ActionID?: string;
    didReceiveSettings?(this: ActionMessage, data: EventPayload.didReceiveSettings): void;
    keyDown?(this: ActionMessage, data: EventPayload.keyDownUpTouchTap): void;
    keyUp?(this: ActionMessage, data: EventPayload.keyDownUpTouchTap): void;
    touchTap?(this: ActionMessage, data: EventPayload.keyDownUpTouchTap): void;
    willAppear?(this: ActionMessage, data: EventPayload.willAppearDisappear): void;
    willDisappear?(this: ActionMessage, data: EventPayload.willAppearDisappear): void;
    titleParametersDidChange?(this: ActionMessage, data: EventPayload.titleParametersDidChange): void;
    propertyInspectorDidAppear?(this: ActionMessage, data: EventPayload.propertyInspectorDidAppearDisappear): void;
    propertyInspectorDidDisappear?(this: ActionMessage, data: EventPayload.propertyInspectorDidAppearDisappear): void;
    sendToPlugin?(this: ActionMessage, data: EventPayload.sendToPlugin): void;
    dialDown?(this: ActionMessage, data: EventPayload.KnobUPDown): void;
    keyUp?(this: ActionMessage, data: EventPayload.KnobUPDown): void;
    dialRotate?(this: ActionMessage, data: EventPayload.dialRotate): void;
  };

  // 插件触发事件
  type PluginMessage = {
    // 设备连接
    deviceDidConnect?(this: PluginMessage, data: EventPayload.deviceDidConnectDisconnect): void;
    // 设备断开
    deviceDidDisconnect?(this: PluginMessage, data: EventPayload.deviceDidConnectDisconnect): void;
    didReceiveGlobalSettings?(this: PluginMessage, data: EventPayload.didReceiveGlobalSettings): void;
    applicationDidLaunch?(this: PluginMessage, data: EventPayload.applicationDidLaunch): void;
    applicationDidTerminate?(this: PluginMessage, data: EventPayload.applicationDidTerminate): void;
    systemDidWakeUp?(this: PluginMessage, data: EventPayload.systemDidWakeUp): void;
  };
}
