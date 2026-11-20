import json
import time
from PIL import Image, ImageDraw
import io
import base64
import requests
from src.core.action import Action
from src.core.logger import Logger

class Custom(Action):
    def __init__(self, action: str, context: str, settings: dict, plugin):
        super().__init__(action, context, settings, plugin)
        # Set up timer to update time every second
        
        self.plugin.set_global_settings({"test": "test"})
        # 1. 画图
        img = Image.new('RGB', (200, 200), color='white')
        draw = ImageDraw.Draw(img)
        draw.rectangle((50, 50, 150, 150), outline='blue', width=3)
        draw.text((60, 60), "Hello", fill='black')

        # 2. 转成 base64
        buffered = io.BytesIO()
        img.save(buffered, format="PNG")  # 保存成PNG格式到内存
        img_base64 = base64.b64encode(buffered.getvalue()).decode('utf-8')
        self.set_image( f"data:image/png;base64,{img_base64}")
        self.set_settings({"test": "test"})
        self.log_message("---------------test--------------")
        self.set_title("test")
        self.show_alert()
        Logger.info(f"[Custom] Initialized with context {context}")
    
    def on_will_disappear(self):
        # Clear the timer when action disappears
        self.plugin.timer.clear_interval(f'time_update_{self.context}')
        Logger.info(f"[Custom] Will disappear for context {self.context}")
    
    def on_did_receive_global_settings(self, settings: dict):
        # Handle global settings update
        Logger.info(f"[Custom] Received global settings: {settings}")

    def on_key_down(self, payload: dict):
        res = requests.get("https://localhost:8000/api", verify=False)
        response = requests.get("https://geoapi.qweather.com/v2/city/lookup?location=广州&key=bdd98ec1d87747f3a2e8b1741a5af796")
        if response.status_code == 200:
            Logger.info(response.json())  # 假设响应内容是 JSON 格式
        else:
            Logger.info(f"请求失败，状态码: {response.status_code}")
        Logger.info(f"[Custom] Key down event with payload: {payload}")

    def on_key_up(self, payload: dict):
        self.plugin.set_global_settings({"test": "tedasdasdst"})
        self.set_settings({"test": "dasdsada"})
        self.send_to_property_inspector({"test": "asdas"})
        self.open_url("https://wiki.mirabox.net/wiki/wiki-e7gidk72/page/page-7e4ad684-8540-4114-969c-eb9c9fa3e955")
        self.show_ok()
        Logger.info(f"[Custom] Key up event with payload: {payload}")
