export const useI18nStore = () => {
  const language = window.argv[3].application.language;
  const localString = {
    en: {
      操作选项: 'Operation options'
    },
    zh_CN: {
      操作选项: '操作选项'
    },
    zh_HK: {
      操作选项: '操作選項'
    },
    de: {
      操作选项: 'Aktionsoptionen'
    },
    el: {
      操作选项: 'Επιλογές ενέργειας'
    },
    es: {
      操作选项: 'Opciones de acción'
    },
    fr: {
      操作选项: 'Options de l’action'
    },
    it: {
      操作选项: 'Opzioni dell’azione'
    },
    ja: {
      操作选项: '操作オプション'
    },
    ko: {
      操作选项: '동작 옵션'
    },
    pl: {
      操作选项: 'Opcje akcji'
    },
    pt: {
      操作选项: 'Opções da ação'
    },
    ru: {
      操作选项: 'Параметры действия'
    },
    tr: {
      操作选项: 'Eylem seçenekleri'
    }
  };
  return localString[language] || localString['en'];
};
