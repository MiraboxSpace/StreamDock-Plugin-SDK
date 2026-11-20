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
