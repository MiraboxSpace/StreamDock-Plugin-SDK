//==============================================================================
/**
@file       HSDConnectionManager.cpp

@brief      Wrapper to implement the communication with the Stream Dock application
**/
//==============================================================================

#include "HSDConnectionManager.h"

#include "NlohmannJSONUtils.h"
#include "HSDLogger.h"

void HSDConnectionManager::OnOpen(WebsocketClient* inClient, websocketpp::connection_hdl inConnectionHandler) {
    HSDLogger::LogMessage("OnOpen");

    // Register plugin with StreamDock
    json jsonObject;
    jsonObject["event"] = mRegisterEvent;
    jsonObject["uuid"] = mPluginUUID;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::OnFail(WebsocketClient* inClient, websocketpp::connection_hdl inConnectionHandler) {
    std::string reason;

    if (inClient) {
        WebsocketClient::connection_ptr connection = inClient->get_con_from_hdl(inConnectionHandler);
        if (connection) {
            reason = connection->get_ec().message();
        }
    }

    HSDLogger::LogMessage("Failed with reason: {}" + reason);

    if (mAsioContext) {
        HSDLogger::LogMessage("Stopping ASIO context");
        mAsioContext->stop();
    }
}

void HSDConnectionManager::OnClose(WebsocketClient* inClient, websocketpp::connection_hdl inConnectionHandler) {
    std::string reason;

    if (inClient) {
        WebsocketClient::connection_ptr connection = inClient->get_con_from_hdl(inConnectionHandler);
        if (connection) {
            reason = connection->get_remote_close_reason();
        }
    }

    HSDLogger::LogMessage("Close with reason: " + reason);

    if (mAsioContext) {
        HSDLogger::LogMessage("Stopping ASIO context");
        mAsioContext->stop();
    }
}

void HSDConnectionManager::OnMessage(websocketpp::connection_hdl, WebsocketClient::message_ptr inMsg) {
    if (inMsg && inMsg->get_opcode() == websocketpp::frame::opcode::text) {
        std::string message = inMsg->get_payload();
        HSDLogger::LogMessage("OnMessage: " + message);

        try {
            json receivedJson = json::parse(message);

            std::string event = NlohmannJSONUtils::GetStringByName(receivedJson, kESDSDKCommonEvent);
            std::string context = NlohmannJSONUtils::GetStringByName(receivedJson, kESDSDKCommonContext);
            std::string action = NlohmannJSONUtils::GetStringByName(receivedJson, kESDSDKCommonAction);
            std::string deviceID = NlohmannJSONUtils::GetStringByName(receivedJson, kESDSDKCommonDevice);
            json payload;
            NlohmannJSONUtils::GetObjectByName(receivedJson, kESDSDKCommonPayload, payload);

            if (event == kESDSDKEventTouchTap) {
                mPlugin->TouchTapForAction(action, context, payload, deviceID);
            } else if (event == kESDSDKEventTouchEvent) {
                mPlugin->TouchEventForAction(action, context, payload, deviceID);
            } else if (event == kESDSDKEventDeleteAction) {
                mPlugin->DeleteActionForAction(action, context, payload, deviceID);
            } else if (event == kESDSDKEventTitleParametersDidChange) {
                mPlugin->TitleParametersDidChangeForAction(action, context, payload, deviceID);
            } else if (event == kESDSDKEventPropertyInspectorDidAppear) {
                mPlugin->PropertyInspectorDidAppearForAction(action, context, payload, deviceID);
            } else if (event == kESDSDKEventPropertyInspectorDidDisappear) {
                mPlugin->PropertyInspectorDidDisappearForAction(action, context, payload, deviceID);
            } else if (event == kESDSDKEventApplicationDidLaunch) {
                mPlugin->ApplicationDidLaunch(payload);
            } else if (event == kESDSDKEventApplicationDidTerminate) {
                mPlugin->ApplicationDidTerminate(payload);
            } else if (event == kESDSDKEventKeyDown) {
                mPlugin->KeyDownForAction(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventKeyUp) {
                mPlugin->KeyUpForAction(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventWillAppear) {
                mPlugin->WillAppearForAction(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventWillDisappear) {
                mPlugin->WillDisappearForAction(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventDidReceiveSettings) {
                mPlugin->DidReceiveSettings(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventDidReceiveGlobalSettings) {
                mPlugin->DidReceiveGlobalSettings(payload);
            }
            else if (event == kESDSDKEventDeviceDidConnect) {
                json deviceInfo;
                NlohmannJSONUtils::GetObjectByName(receivedJson, kESDSDKCommonDeviceInfo, deviceInfo);
                mPlugin->DeviceDidConnect(deviceID, deviceInfo);
            }
            else if (event == kESDSDKEventDeviceDidDisconnect) {
                mPlugin->DeviceDidDisconnect(deviceID);
            }
            else if (event == kESDSDKEventSendToPlugin) {
                mPlugin->SendToPlugin(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventSystemDidWakeUp) {
                mPlugin->SystemDidWakeUp();
            }
            else if (event == kESDSDKEventDialDown) {
                mPlugin->DialDownForAction(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventDialUp) {
                mPlugin->DialUpForAction(action, context, payload, deviceID);
            }
            else if (event == kESDSDKEventDialRotate) {
                mPlugin->DialRotateForAction(action, context, payload, deviceID);
            }
        }
        catch (...) {
        }
    }
}

HSDConnectionManager::HSDConnectionManager(
    int inPort,
    const std::string& inPluginUUID,
    const std::string& inRegisterEvent,
    const std::string& inInfo,
    std::shared_ptr<HSDBasePlugin> inPlugin)
    : mPort(inPort),
    mPluginUUID(inPluginUUID),
    mRegisterEvent(inRegisterEvent),
    mPlugin(inPlugin) {
    if (inPlugin) {
        inPlugin->SetConnectionManager(this);
    }
}

void HSDConnectionManager::Run() {
    try {
        // Create the endpoint
        mWebsocket.clear_access_channels(websocketpp::log::alevel::all);
        mWebsocket.clear_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        auto ctx = std::make_shared<asio::io_context>();
        mWebsocket.init_asio(ctx.get());
        mAsioContext = ctx;

        // Register our message handler
        mWebsocket.set_open_handler(websocketpp::lib::bind(
            &HSDConnectionManager::OnOpen, this,
            &mWebsocket, websocketpp::lib::placeholders::_1));
        mWebsocket.set_fail_handler(websocketpp::lib::bind(
            &HSDConnectionManager::OnFail, this,
            &mWebsocket, websocketpp::lib::placeholders::_1));
        mWebsocket.set_close_handler(websocketpp::lib::bind(
            &HSDConnectionManager::OnClose, this,
            &mWebsocket, websocketpp::lib::placeholders::_1));
        mWebsocket.set_message_handler(websocketpp::lib::bind(
            &HSDConnectionManager::OnMessage, this,
            websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

        // Get connection handle
        websocketpp::lib::error_code ec;
        std::string uri = "ws://127.0.0.1:" + std::to_string(mPort);
        WebsocketClient::connection_ptr connection = mWebsocket.get_connection(uri, ec);
        if (ec) {
            HSDLogger::LogMessage("Connect initialization error: " + ec.message());
            return;
        }
        mConnectionHandle = connection->get_handle();

        // Note that connect here only requests a connection. No network messages
        // are exchanged until the event loop starts running in the next line.
        mWebsocket.connect(connection);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server.
        // mWebsocket.run() will exit when this connection is closed.
        mWebsocket.run();
    }
    catch (websocketpp::exception const& e) {
        // Prevent an unused variable warning in release builds
        (void)e;
        std::string whatString = e.what();
        HSDLogger::LogMessage("Websocket threw an exception: " + whatString);
    }
}

void HSDConnectionManager::SetTitle(
    const std::string& inTitle,
    const std::string& inContext,
    ESDSDKTarget inTarget,
    int inState) {
    json jsonObject;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventSetTitle;
    jsonObject[kESDSDKCommonContext] = inContext;

    json payload;
    payload[kESDSDKPayloadTarget] = inTarget;
    payload[kESDSDKPayloadTitle] = inTitle;
    if (inState >= 0) {
        payload[kESDSDKPayloadState] = inState;
    }
    jsonObject[kESDSDKCommonPayload] = payload;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::SetImage(
    const std::string& inBase64ImageString,
    const std::string& inContext,
    ESDSDKTarget inTarget,
    int inState) {
    json jsonObject;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventSetImage;
    jsonObject[kESDSDKCommonContext] = inContext;

    json payload;
    payload[kESDSDKPayloadTarget] = inTarget;
    const std::string prefix = "data:image/png;base64,";
    if (inBase64ImageString.empty() ||
        inBase64ImageString.substr(0, prefix.length()).find(prefix) == 0) {
        payload[kESDSDKPayloadImage] = inBase64ImageString;
    }
    else {
        payload[kESDSDKPayloadImage] = "data:image/png;base64," + inBase64ImageString;
    }
    if (inState >= 0) {
        payload[kESDSDKPayloadState] = inState;
    }
    jsonObject[kESDSDKCommonPayload] = payload;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::ShowAlertForContext(const std::string& inContext) {
    json jsonObject;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventShowAlert;
    jsonObject[kESDSDKCommonContext] = inContext;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::ShowOKForContext(const std::string& inContext) {
    json jsonObject;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventShowOK;
    jsonObject[kESDSDKCommonContext] = inContext;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::SetSettings(
    const json& inSettings,
    const std::string& inContext) {
    json jsonObject;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventSetSettings;
    jsonObject[kESDSDKCommonContext] = inContext;
    jsonObject[kESDSDKCommonPayload] = inSettings;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::SetState(int inState, const std::string& inContext) {
    json jsonObject;

    json payload;
    payload[kESDSDKPayloadState] = inState;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventSetState;
    jsonObject[kESDSDKCommonContext] = inContext;
    jsonObject[kESDSDKCommonPayload] = payload;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::SendToPropertyInspector(
    const std::string& inAction,
    const std::string& inContext,
    const json& inPayload) {
    json jsonObject;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventSendToPropertyInspector;
    jsonObject[kESDSDKCommonContext] = inContext;
    jsonObject[kESDSDKCommonAction] = inAction;
    jsonObject[kESDSDKCommonPayload] = inPayload;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::SwitchToProfile(const std::string& inDeviceID, const std::string &inProfileName, int page) {
    if (!inDeviceID.empty()) {
        json jsonObject;

        jsonObject[kESDSDKCommonEvent] = kESDSDKEventSwitchToProfile;
        jsonObject[kESDSDKCommonContext] = mPluginUUID;
        jsonObject[kESDSDKCommonDevice] = inDeviceID;

        nlohmann::json payload;
        payload[kESDSDKPayloadProfile] = inProfileName;
        if (page >= 0) payload["page"] = page;
        jsonObject[kESDSDKCommonPayload] = payload;

        websocketpp::lib::error_code ec;
        mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
    }
}

void HSDConnectionManager::LogMessage(const std::string& inMessage) {
    if (!inMessage.empty()) {
        json jsonObject;

        jsonObject[kESDSDKCommonEvent] = kESDSDKEventLogMessage;

        json payload;
        payload[kESDSDKPayloadMessage] = inMessage;
        jsonObject[kESDSDKCommonPayload] = payload;

        websocketpp::lib::error_code ec;
        mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
    }
}

void HSDConnectionManager::GetGlobalSettings() {
    json jsonObject{ {kESDSDKCommonEvent, kESDSDKEventGetGlobalSettings},
                    {kESDSDKCommonContext, mPluginUUID} };
    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

void HSDConnectionManager::SetGlobalSettings(const json& inSettings) {
    json jsonObject;

    jsonObject[kESDSDKCommonEvent] = kESDSDKEventSetGlobalSettings;
    jsonObject[kESDSDKCommonContext] = mPluginUUID;
    jsonObject[kESDSDKCommonPayload] = inSettings;

    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}

std::shared_ptr<asio::io_context> HSDConnectionManager::GetAsioContext() const {
    return mAsioContext;
}

void HSDConnectionManager::TriggerKey(const std::string &inContext, const nlohmann::json &keyList) {
    nlohmann::json payload;
    payload["keyList"] = keyList;
    nlohmann::json jsonObject;
    jsonObject["event"] = "triggerKey";
    jsonObject["context"] = inContext;
    jsonObject["payload"] = payload;
    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}
void HSDConnectionManager::OpenUrl(const std::string &url) {
    nlohmann::json payload;
    payload["url"] = url;
    nlohmann::json jsonObject;
    jsonObject["event"] = "openUrl";
    jsonObject["payload"] = payload;
    websocketpp::lib::error_code ec;
    mWebsocket.send(mConnectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
}
