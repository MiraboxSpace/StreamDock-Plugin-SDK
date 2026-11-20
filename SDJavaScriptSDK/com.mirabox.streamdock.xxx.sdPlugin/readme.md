# Stream Dock Documentation

Only common simple APIs are recorded. Please read the official documentation for more detailed properties.

[Official StreamDock development guide](https://wiki.mirabox.net/wiki/wiki-e7gidk72/page/page-83dfae17-6163-4a98-a372-51a778babe55?lang=en)


## Property inspector HTML template

```html
<!-- input -->
<div class="sdpi-item"> 
  <div class="sdpi-item-label">xxx</div> 
  <input class="sdpi-item-value"></input>
</div>

<!-- button -->
<div class="sdpi-item"> 
  <div class="sdpi-item-label">Button</div> 
  <button class="sdpi-item-value">Click Me</button>
</div>

<!-- textarea -->
<div type="textarea" class="sdpi-item"> 
  <div class="sdpi-item-label">xxx</div> 
  <textarea class="sdpi-item-value" type="textarea"></textarea>
</div>

<!-- select -->
<div type="select" class="sdpi-item"> 
  <div class="sdpi-item-label">xxx</div> 
  <select class="sdpi-item-value"> 
    <option value="xxx">xxx</option> 
  </select>
</div>

<!-- checkbox -->
<div type="checkbox" class="sdpi-item"> 
  <div class="sdpi-item-label">Check Me</div> 
  <div class="sdpi-item-value"> 
    <span class="sdpi-item-child"> 
      <input id="chk1" type="checkbox" value="left"> 
      <label for="chk1"><span></span>left</label> 
    </span> 
    <span class="sdpi-item-child"> 
      <input id="chk2" type="checkbox" value="right"> 
      <label for="chk2"><span></span>right</label> 
    </span> 
  </div>
</div>

<!-- radio -->
<div type="radio" class="sdpi-item"> 
  <div class="sdpi-item-label">Radio</div> 
  <div class="sdpi-item-value"> 
    <span class="sdpi-item-child"> 
      <input id="rdio1" type="radio" name="rdio" checked> 
      <label for="rdio1" class="sdpi-item-label"><span></span>on</label> 
    </span> 
    <span class="sdpi-item-child"> 
      <input id="rdio2" type="radio" value="off" name="rdio"> 
      <label for="rdio2" class="sdpi-item-label"><span></span>off</label>
    </span>
    <span class="sdpi-item-child">
      <input id="rdio3" type="radio" value="mute" name="rdio">
      <label for="rdio3" class="sdpi-item-label"><span></span>mute</label>
    </span>
  </div>
</div>

<!-- range -->
<div type="range" class="sdpi-item" id="temperatureslider">
  <div class="sdpi-item-label">xxx</div>
  <input type="range" class="sdpi-item-value" min="0" max="100" value=37>
</div>
```
## Events that plugins can trigger

didReceiveSettings Operation persistent data trigger

```json
{
  "action": "com.example.action1",
  "event": "didReceiveSettings",
  "context": uniqueValue,
  "device": uniqueValue,
  "payload": {
    "settings": {<json data>
    },
    "coordinates": {
      "column": 3,
      "row": 1
    },
    "isInMultiAction": false
  }
}
```

keyDown/keyUp/touchTap triggers when pressed/released/touched

```json
{
  "action": "com.example.action1",
  "event": "keyUp",
  "context": uniqueValue,
  "device": uniqueValue,
  "payload": {
    "settings": {<json data>
    },
    "coordinates": {
      "column": 3,
      "row": 1
    },
    "state": 0,
    "userDesiredState": 1,
    "isInMultiAction": false
  }
}
```

willAppear/willDisappear Fired when a plugin is created/deleted

```json
{
  "action": "com.example.action1",
  "event": "willAppear",
  "context": uniqueValue,
  "device": uniqueValue,
  "payload": {
    "settings": {<json data>
    },
    "coordinates": {
      "column": 3,
      "row": 1
    },
    "state": 0,
    "isInMultiAction": false
  }
}
```

titleParametersDidChange Fired when the user changes the title/title parameters

```json
{
  "action": "com.example.action1",
  "event": "titleParametersDidChange",
  "context": "uniqueValue",
  "device": "uniqueValue",
  "payload": {
    "coordinates": {
      "column": 3,
      "row": 1
    },
    "settings": {<json data>
    },
    "state": 0,
    "title": "",
    "titleParameters": {
      "fontFamily": "",
      "fontSize": 12,
      "fontStyle": "",
      "fontUnderline": false,
      "showTitle": true,
      "titleAlignment": "bottom",
      "titleColor": "#ffffff"
    }
  }
}
```

deviceDidConnect/deviceDidDisconnect Fired when the device is plugged/unplugged from the computer

```json
{
  "event": "deviceDidConnect",
  "device": uniqueValue,
  "deviceInfo": {
    "name": "Device Name",
    "type": 0,
    "size": {
      "columns": 5,
      "rows": 3
    }
  },
}
```

propertyInspectorDidAppear/propertyInspectorDidDisappear Fired when the property selector appears/hides in the UI

```json
{
  "action": "com.example.action1",
  "event": "propertyInspectorDidAppear",
  "context": uniqueValue,
  "device": uniqueValue
}
```

sendToPlugin Fires when the property selector uses the sendToPlugin event

```json
{
  "action": "com.example.action1",
  "event": "sendToPlugin",
  "context": uniqueValue,
  "payload": {<json data>}
}
```

dialRotate Fires when the knob is rotated

```json
{
  "action": "com.example.action1",
  "event": "sendToPlugin",
  "context": uniqueValue,
  "payload": {<json data>}
}
```

dialDown Fires when the knob is pressed

```json
{
  "action": "com.example.action1",
  "event": "sendToPlugin",
  "context": uniqueValue,
  "payload": {<json data>}
}
```

## Events that can be triggered by attribute selectors

didReceiveSettings triggers persistent data

sendToPropertyInspector triggers when the plugin uses the sendToPropertyInspector event

```json
{
  "action": "com.example.action1",
  "event": "sendToPropertyInspector",
  "context": uniqueValue,
  "payload": {<json data>}
}
```

## Events that plugins can send

setSettings Persistently save the data of the action instance

openUrl Open the URL in the default browser

setTitle Dynamically change the title of the action instance

setImage Dynamically change the image displayed by the action instance

setState Change the state of an action instance that supports multiple states

sendToPropertyInspector Send a payload to the property inspector

## Events that property selectors can send

setSettings Persistently save the data of the action instance

openUrl Open the URL in the default browser

sendToPlugin Send a payload to the plugin
