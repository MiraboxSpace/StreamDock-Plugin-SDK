/**
 * StreamDock Plugin Template V1.2.1 说明文件 =>
 *
 *      1 => 开发环境支持热更新 => 修改代码无需重启服务器和软件 ( 修改图片/配置文件时需要重启 ) !
 *      2 => 自动打包到插件目录 => 使用 pnpm dev/build 即可自动打包到插件目录，无需手动复制删除。
 *      3 => 数据持久化驱动视图 => 通过 v-model 绑定 settings 的值即可实现双向绑定持久化数据回显啦 !
 *      4 => 完美集成 Navie UI 组件库 => 主题可调，无需穿透样式，有超过 90 个组件，希望能帮你少写点代码。
 *
 *      !! 注意事项 !! => 自动化含有许多约定配置 => 以下内容请务必认真填写 => 祝你开发愉快 _> </>
 *
 * =========== Kriac =================================================================================== 于 2024.03.30 更新 ===========>
 */

const Plugin = {
  UUID: 'com.streamdock.demo',
  version: '1.0.0',
  Icon: 'images/icon.png',
  i18n: {
    en: {
      Name: 'Keyboard Actions',
      Description: 'StreamDock action examples'
    },
    zh_CN: {
      Name: '键盘操作',
      Description: 'StreamDock 操作示例'
    },
    zh_HK: {
      Name: '鍵盤操作',
      Description: 'StreamDock 操作示例'
    },
    de: {
      Name: 'Tastaturaktionen',
      Description: 'StreamDock-Aktionsbeispiele'
    },
    el: {
      Name: 'Ενέργειες πληκτρολογίου',
      Description: 'Παραδείγματα ενεργειών StreamDock'
    },
    es: {
      Name: 'Acciones de teclado',
      Description: 'Ejemplos de acciones de StreamDock'
    },
    fr: {
      Name: 'Actions du clavier',
      Description: 'Exemples d’actions StreamDock'
    },
    it: {
      Name: 'Azioni della tastiera',
      Description: 'Esempi di azioni StreamDock'
    },
    ja: {
      Name: 'キーボード操作',
      Description: 'StreamDock の操作例'
    },
    ko: {
      Name: '키보드 동작',
      Description: 'StreamDock 동작 예제'
    },
    pl: {
      Name: 'Akcje klawiatury',
      Description: 'Przykłady akcji StreamDock'
    },
    pt: {
      Name: 'Ações do teclado',
      Description: 'Exemplos de ações do StreamDock'
    },
    ru: {
      Name: 'Действия клавиатуры',
      Description: 'Примеры действий StreamDock'
    },
    tr: {
      Name: 'Klavye eylemleri',
      Description: 'StreamDock eylem örnekleri'
    }
  },
  Software: {
    MinimumVersion: '6.5'
  },
  ApplicationsToMonitor: {
    windows: []
  }
};

// 操作数组
const Actions = [
  {
    UUID: 'action1',
    Icon: 'images/icon.png',
    i18n: {
      en: {
        Name: 'Action 1',
        Tooltip: 'This is an action'
      },
      zh_CN: {
        Name: '操作 1',
        Tooltip: '这是一个操作'
      },
      zh_HK: {
        Name: '操作 1',
        Tooltip: '這是一個操作'
      },
      de: {
        Name: 'Aktion 1',
        Tooltip: 'Dies ist eine Aktion'
      },
      el: {
        Name: 'Ενέργεια 1',
        Tooltip: 'Αυτή είναι μια ενέργεια'
      },
      es: {
        Name: 'Acción 1',
        Tooltip: 'Esta es una acción'
      },
      fr: {
        Name: 'Action 1',
        Tooltip: 'Ceci est une action'
      },
      it: {
        Name: 'Azione 1',
        Tooltip: 'Questa è un’azione'
      },
      ja: {
        Name: 'アクション 1',
        Tooltip: 'アクションの例です'
      },
      ko: {
        Name: '동작 1',
        Tooltip: '동작 예제입니다'
      },
      pl: {
        Name: 'Akcja 1',
        Tooltip: 'To jest akcja'
      },
      pt: {
        Name: 'Ação 1',
        Tooltip: 'Esta é uma ação'
      },
      ru: {
        Name: 'Действие 1',
        Tooltip: 'Это действие'
      },
      tr: {
        Name: 'Eylem 1',
        Tooltip: 'Bu bir eylemdir'
      }
    },
    state: 0,
    States: [
      {
        FontSize: '10',
        TitleAlignment: 'bottom',
        Image: 'images/default.png'
      }
    ],
    Settings: {},
    UserTitleEnabled: false,
    SupportedInMultiActions: false,
    Controllers: ['Keypad', 'Information']
  }
];

// !! 请勿修改 !!
module.exports = {
  PUUID: Plugin.UUID,
  ApplicationsToMonitor: Plugin.ApplicationsToMonitor,
  Software: Plugin.Software,
  Version: Plugin.version,
  CategoryIcon: Plugin.Icon,
  i18n: Plugin.i18n,
  Actions
};
