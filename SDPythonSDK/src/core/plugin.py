import json
import threading
import websocket
import logging
import os
import sys
from typing import Any, Dict, List, Optional
from http.server import HTTPServer, BaseHTTPRequestHandler
from .timer import Timer
from .action import Action
from .logger import Logger

class Plugin:
    """Stream Dock插件的核心类，负责管理WebSocket连接和处理Stream Dock事件。
    
    该类维护与Stream Dock软件的WebSocket连接，处理各种事件（如按钮出现、消失、设置更改等），
    并管理插件的动作(Action)实例。每个动作实例对应Stream Dock界面上的一个按钮。
    """
    
    def __init__(self, port: int, plugin_uuid: str, event: str, info: Dict[str, Any]):
        """初始化插件实例
        
        Args:
            port: WebSocket服务器端口号
            plugin_uuid: 插件的唯一标识符
            event: 事件类型
            info: 包含插件信息的对象
        """
        self.actions: Dict[str, Action] = {}
        self.global_settings: Any = None
        self.timer = Timer()
        self.plugin_uuid = plugin_uuid
        self.http_server = None
        self.http_server_thread = None
        
        # 启动HTTP服务
        # self._start_http_server()
        
        # Initialize WebSocket
        self.ws = websocket.WebSocketApp(
            f'ws://127.0.0.1:{port}',
            on_open=lambda ws: self._on_open(ws, event, plugin_uuid),
            on_message=self._on_message,
            on_error=lambda ws, error: Logger.error(f"WebSocket error: {error}")
        )
        
        # Start WebSocket connection in a separate thread
        threading.Thread(target=self.ws.run_forever, daemon=True).start()
    
    def _on_open(self, ws, event: str, plugin_uuid: str):
        """WebSocket连接建立时的回调函数
        
        向Stream Dock注册插件，发送初始化事件。
        
        Args:
            ws: WebSocket连接实例
            event: 事件类型
            plugin_uuid: 插件UUID
        """        
        Logger.info("WebSocket connected")
        
        ws.send(json.dumps({'event': event, 'uuid': plugin_uuid}))
    
    def _on_message(self, ws, message):
        """处理从Stream Dock接收到的WebSocket消息
        
        根据接收到的事件类型执行相应的操作，包括：
        - 处理全局settings更新
        - 处理按钮出现/消失事件
        - 处理按钮settings更改
        - 处理标题参数更改
        
        Args:
            ws: WebSocket连接实例
            message: 接收到的JSON消息
        """        
        data = json.loads(message)
        event = data.get('event')
        Logger.info(event)
        dimension_events = {
            'didReceiveSettings', 'keyDown', 'keyUp',
            'willAppear', 'willDisappear', 'titleParametersDidChange'
        }
        def update_dimensions(action):
            payload = data.get('payload', {})
            for dimension in ('width', 'height'):
                if dimension in payload:
                    setattr(action, dimension, payload[dimension])

        if event in dimension_events:
            action = self.actions.get(data.get('context'))
            if action:
                update_dimensions(action)
        if event == 'didReceiveGlobalSettings':
            self.global_settings = data.get('payload', {}).get('settings')
            for action in self.actions.values():
                if hasattr(action, 'on_did_receive_global_settings'):
                    action.on_did_receive_global_settings(self.global_settings)
        elif event == 'willAppear':
            context = data.get('context')
            if context not in self.actions:
                from .action_factory import ActionFactory
                action = ActionFactory.create_action(
                    data.get('action'),
                    context,
                    data.get('payload', {}).get('settings', {}),
                    self
                )
                if action:
                    update_dimensions(action)
                    self.actions[context] = action
                else:
                    Logger.error(f"Failed to create action for context: {context}")
        elif event == 'willDisappear':
            context = data.get('context')
            if context in self.actions:
                action = self.actions[context]
                if hasattr(action, 'on_will_disappear'):
                    action.on_will_disappear()
                del self.actions[context]
        elif event == 'didReceiveSettings':
            context = data.get('context')
            if context in self.actions:
                action = self.actions[context]
                settings = data.get('payload', {}).get('settings', {})
                if hasattr(action, 'on_did_receive_settings'):
                    action.on_did_receive_settings(settings)
                else:
                    action.settings = settings
        elif event == 'titleParametersDidChange':
            context = data.get('context')
            if context in self.actions:
                action = self.actions[context]
                payload = data.get('payload', {})
                if hasattr(action, 'on_title_parameters_did_change'):
                    action.on_title_parameters_did_change(payload)
                else:
                    action.title = payload.get('title', '')
                    action.title_parameters = payload.get('titleParameters', {})
        # Handle context-specific events
        context_events = {
            'keyDown': 'on_key_down',
            'keyUp': 'on_key_up',
            'dialDown': 'on_dial_down',
            'dialUp': 'on_dial_up',
            'dialRotate': 'on_dial_rotate'
        }
        
        if event in context_events:
            context = data.get('context')
            if context in self.actions:
                action = self.actions[context]
                handler = context_events[event]
                if hasattr(action, handler):
                    getattr(action, handler)(data.get('payload', {}))
        # Handle global events
        global_events = {
            'deviceDidConnect': 'on_device_did_connect',
            'deviceDidDisconnect': 'on_device_did_disconnect',
            'applicationDidLaunch': 'on_application_did_launch',
            'applicationDidTerminate': 'on_application_did_terminate',
            'systemDidWakeUp': 'on_system_did_wake_up'
        }
        
        if event in global_events:
            handler = global_events[event]
            for action in self.actions.values():
                if hasattr(action, handler):
                    getattr(action, handler)(data)
        elif event == 'propertyInspectorDidAppear':
            context = data.get('context')
            if context in self.actions:
                action = self.actions[context]
                if hasattr(action, 'on_property_inspector_did_appear'):
                    action.on_property_inspector_did_appear(data)
        elif event == 'propertyInspectorDidDisappear':
            context = data.get('context')
            if context in self.actions:
                action = self.actions[context]
                if hasattr(action, 'on_property_inspector_did_disappear'):
                    action.on_property_inspector_did_disappear(data)
        elif event == 'sendToPlugin':
            context = data.get('context')
            if context in self.actions:
                action = self.actions[context]
                if hasattr(action, 'on_send_to_plugin'):
                    action.on_send_to_plugin(data.get('payload', {}))
    
    def set_global_settings(self, payload: Any):
        """更新插件的全局设置
        
        Args:
            payload: 新的全局设置值
        """        
        self.ws.send(json.dumps({
            'event': 'setGlobalSettings',
            'context': self.plugin_uuid,
            'payload': payload
        }))
        self.global_settings = payload
    
    def get_global_settings(self):
        """请求获取插件的当前全局设置
        
        发送请求后，设置值将通过WebSocket消息返回
        """        
        self.ws.send(json.dumps({
            'event': 'getGlobalSettings',
            'context': self.plugin_uuid
        }))
    
    def get_action(self, context: str) -> Optional[Action]:
        """获取指定上下文的Action实例
        
        Args:
            context: Action的上下文标识符
            
        Returns:
            如果存在则返回Action实例，否则返回None
        """        
        return self.actions.get(context)
    
    def get_actions(self, action: str) -> List[Action]:
        """获取所有指定类型的Action实例列表
        
        Args:
            action: Action的类型标识符
            
        Returns:
            符合指定类型的Action实例列表
        """        
        return [a for a in self.actions.values() if a.action == action]
    
    def _start_http_server(self, port: int = 8000):
        """启动HTTPS服务器
        
        Args:
            port: HTTPS服务器端口号，默认为8000
        """
        import ssl
        import os
        
        class RequestHandler(BaseHTTPRequestHandler):
            def do_GET(self):
                if self.path == '/api':
                    self.send_response(200)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    response = {'status': 'ok', 'message': 'API endpoint'}
                    self.wfile.write(json.dumps(response).encode())
                else:
                    self.send_response(404)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    response = {'status': 'error', 'message': 'Not found'}
                    self.wfile.write(json.dumps(response).encode())
        
        try:
            self.http_server = HTTPServer(('0.0.0.0', port), RequestHandler)
            
            # 配置SSL
            import tempfile
            
            # 创建临时文件来存储证书和密钥
            cert_file = tempfile.NamedTemporaryFile(delete=False)
            key_file = tempfile.NamedTemporaryFile(delete=False)
            
            # 生成自签名证书和私钥
            from cryptography import x509
            from cryptography.x509.oid import NameOID
            from cryptography.hazmat.primitives import hashes
            from cryptography.hazmat.primitives.asymmetric import rsa
            from cryptography.hazmat.primitives import serialization
            from datetime import datetime, timedelta
            
            # 生成私钥
            private_key = rsa.generate_private_key(
                public_exponent=65537,
                key_size=2048
            )
            
            # 生成证书
            subject = issuer = x509.Name([
                x509.NameAttribute(NameOID.COMMON_NAME, u"localhost"),
                x509.NameAttribute(NameOID.ORGANIZATION_NAME, u"Stream Dock Plugin"),
                x509.NameAttribute(NameOID.ORGANIZATIONAL_UNIT_NAME, u"Development"),
                x509.NameAttribute(NameOID.COUNTRY_NAME, u"CN"),
                x509.NameAttribute(NameOID.STATE_OR_PROVINCE_NAME, u"Development State"),
                x509.NameAttribute(NameOID.LOCALITY_NAME, u"Development City"),
            ])
            
            cert = x509.CertificateBuilder().subject_name(
                subject
            ).issuer_name(
                issuer
            ).public_key(
                private_key.public_key()
            ).serial_number(
                x509.random_serial_number()
            ).not_valid_before(
                datetime.utcnow()
            ).not_valid_after(
                datetime.utcnow() + timedelta(days=365)
            ).sign(private_key, hashes.SHA256())
            
            # 将证书和私钥写入临时文件
            cert_file.write(cert.public_bytes(serialization.Encoding.PEM))
            key_file.write(private_key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=serialization.NoEncryption()
            ))
            cert_file.close()
            key_file.close()
            
            # 配置SSL上下文
            context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
            context.load_cert_chain(cert_file.name, key_file.name)
            
            # 包装socket
            self.http_server.socket = context.wrap_socket(self.http_server.socket, server_side=True)
            
            # 删除临时文件
            os.unlink(cert_file.name)
            os.unlink(key_file.name)
            
            self.http_server_thread = threading.Thread(target=self.http_server.serve_forever, daemon=True)
            self.http_server_thread.start()
            Logger.info(f"HTTPS server started on port {port}")
        except Exception as e:
            Logger.error(f"Failed to start HTTPS server: {e}")
    
    def stop(self):
        """停止插件服务
        
        停止HTTP服务器和WebSocket连接
        """
        if self.http_server:
            self.http_server.shutdown()
            self.http_server.server_close()
            Logger.info("HTTP server stopped")
