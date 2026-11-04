const path = require('path');
const fs = require('fs-extra');

const manifest = {};
const { PUUID, Actions, i18n, CategoryIcon, Version, Software, ApplicationsToMonitor } = require('../src/manifest.cjs');
console.log('开始执行自动化构建...');

// 开发环境处理
if (process.argv[2] === 'dev') {
  fs.removeSync('./dist') || fs.mkdirSync('./dist') || fs.copySync('./public', './dist');
  fs.copyFileSync('./script/_.html', './dist/_.html');
}

// 根据用户生成
manifest.Actions = Actions.map((item) => {
  item.Name = item.i18n['en'].Name;
  item.Tooltip = item.i18n['en'].Tooltip;
  item.UUID = `${PUUID}.` + item.UUID;
  item.PropertyInspectorPath = process.argv[2] === 'dev' ? '_.html' : 'index.html';
  return item;
});
manifest.Version = Version;
manifest.Name = i18n['en'].Name;
manifest.Icon = CategoryIcon;
manifest.CategoryIcon = CategoryIcon;
manifest.Category = i18n['en'].Name;
manifest.Description = i18n['en'].Description;
manifest.CodePath = process.argv[2] === 'dev' ? '_.html' : 'index.html';

// 版本固定生成
manifest.SDKVersion = 3;
manifest.Author = 'MiraBox';
manifest.URL = 'http://video.hotspotek.com.cn/';
manifest.OS = [
  {
    Platform: 'mac',
    MinimumVersion: '10.11'
  },
  {
    Platform: 'windows',
    MinimumVersion: '7'
  }
];

// 语言文件生成
Object.keys(i18n).forEach((item) => {
  const obj = {};
  obj.Name = i18n[item].Name;
  obj.Category = i18n[item].Name;
  obj.Description = i18n[item].Description;
  manifest.Actions.forEach((action) => {
    obj[action.UUID] = {
      Name: action.i18n[item].Name,
      Tooltip: action.i18n[item].Tooltip
    };
  });
  obj.Localization = {};
  fs.writeJSONSync(`./dist/${item}.json`, obj);
});

// 生成清单文件
manifest.Actions = manifest.Actions.map((item) => {
  delete item.i18n;
  return item;
});
manifest.Software = Software;
manifest.ApplicationsToMonitor = ApplicationsToMonitor
fs.writeJSONSync('./dist/manifest.json', manifest, { spaces: 2, EOL: '\r\n' });

// 复制到插件文件夹
const PluginName = `${PUUID}.sdPlugin`;
const PluginPath = path.join(process.env.APPDATA, 'HotSpot/StreamDock/plugins', PluginName);
fs.removeSync(PluginPath) || fs.mkdirSync(PluginPath) || fs.copySync('./dist', PluginPath);
