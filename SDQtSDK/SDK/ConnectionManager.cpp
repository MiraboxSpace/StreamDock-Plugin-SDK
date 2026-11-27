#include "ConnectionManager.h"
#include "Logger.h"
#include <QCoreApplication>
#include <QJsonDocument>

ConnectionManager::ConnectionManager(int inPort,
                                     const QString &inPluginUUID,
                                     const QString &inRegisterEvent,
                                     const QString &inInfo,
                                     BasePlugin* inPlugin,
                                     QObject *parent)
    : QObject{parent},
      mPort(inPort),
      mPluginUUID(inPluginUUID),
      mRegisterEvent(inRegisterEvent),
      mPlugin(inPlugin)
{
    if (inPlugin) {
        inPlugin->SetConnectionManager(this);
    }
}

ConnectionManager::~ConnectionManager()
{
    mWebsocketClient->abort();
    delete mWebsocketClient;
    mWebsocketClient = nullptr;
}

void ConnectionManager::Run()
{
    if (!mWebsocketClient) {
        mWebsocketClient = new QWebSocket();
        connect(mWebsocketClient, &QWebSocket::connected, this, &ConnectionManager::OnConnected);
        connect(mWebsocketClient, &QWebSocket::disconnected, this, &ConnectionManager::OnDisonnected);
        connect(mWebsocketClient, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &ConnectionManager::OnError);
        connect(mWebsocketClient, &QWebSocket::textMessageReceived, this, &ConnectionManager::OnMessage);

        QString urlString = QString("ws://127.0.0.1:%1").arg(mPort);
        mWebsocketClient->open(QUrl(urlString));
    }
}

void ConnectionManager::SetTitle(const QString &inTitle,
                                 const QString &inContext,
                                 mSDKTarget inTarget,
                                 int inState)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventSetTitle;
    jsonObject[mSDKCommonContext] = inContext;
    QJsonObject payload;
    payload[mSDKPayloadTarget] = inTarget;
    payload[mSDKPayloadTitle] = inTitle;
    if (inState >= 0) {
        payload[mSDKPayloadState] = inState;
    }
    jsonObject[mSDKCommonPayload] = payload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::SetImage(const QString &inBase64ImageString,
                                 const QString &inContext,
                                 mSDKTarget inTarget,
                                 int inState)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventSetImage;
    jsonObject[mSDKCommonContext] = inContext;
    QJsonObject payload;
    payload[mSDKPayloadTarget] = inTarget;
    const QString prefix = "data:image/png;base64,";
    if (inBase64ImageString.isEmpty() || inBase64ImageString.startsWith(prefix)) {
        payload[mSDKPayloadImage] = inBase64ImageString;
    } else {
        payload[mSDKPayloadImage] = "data:image/png;base64," + inBase64ImageString;
    }
    if (inState >= 0) {
        payload[mSDKPayloadState] = inState;
    }
    jsonObject[mSDKCommonPayload] = payload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::ShowAlertForContext(const QString &inContext)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventShowAlert;
    jsonObject[mSDKCommonContext] = inContext;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::ShowOKForContext(const QString &inContext)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventShowOK;
    jsonObject[mSDKCommonContext] = inContext;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::SetSettings(const QJsonObject &inPayload,
                                    const QString &inContext)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventSetSettings;
    jsonObject[mSDKCommonContext] = inContext;
    jsonObject[mSDKCommonPayload] = inPayload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::GetGlobalSettings()
{
    QJsonObject jsonObject{{mSDKCommonEvent, mSDKEventGetGlobalSettings}, {mSDKCommonContext, mPluginUUID}};
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::SetGlobalSettings(const QJsonObject &inPayload)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventSetGlobalSettings;
    jsonObject[mSDKCommonContext] = mPluginUUID;
    jsonObject[mSDKCommonPayload] = inPayload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::SetState(int inState, const QString &inContext)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventSetState;
    jsonObject[mSDKCommonContext] = inContext;
    QJsonObject payload;
    payload[mSDKPayloadState] = inState;
    jsonObject[mSDKCommonPayload] = payload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::SendToPropertyInspector(const QString &inAction,
                                                const QString &inContext,
                                                const QJsonObject &inPayload)
{
    QJsonObject jsonObject;
    jsonObject[mSDKCommonEvent] = mSDKEventSendToPropertyInspector;
    jsonObject[mSDKCommonContext] = inContext;
    jsonObject[mSDKCommonAction] = inAction;
    jsonObject[mSDKCommonPayload] = inPayload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::SwitchToProfile(const QString &inDeviceID, const QString &inProfileName, int page)
{
    if (!inDeviceID.isEmpty()) {
        QJsonObject jsonObject;
        jsonObject[mSDKCommonEvent] = mSDKEventSwitchToProfile;
        jsonObject[mSDKCommonContext] = mPluginUUID;
        jsonObject[mSDKCommonDevice] = inDeviceID;
        QJsonObject payload;
        payload[mSDKPayloadProfile] = inProfileName;
        if (page >= 0) payload["page"] = page;
        jsonObject[mSDKCommonPayload] = payload;
        SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
    }
}

void ConnectionManager::LogMessage(const QString &inMessage)
{
    if (!inMessage.isEmpty()) {
        QJsonObject jsonObject;
        jsonObject[mSDKCommonEvent] = mSDKEventLogMessage;
        QJsonObject payload;
        payload[mSDKPayloadMessage] = inMessage;
        jsonObject[mSDKCommonPayload] = payload;
        SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
    }
}

void ConnectionManager::OnConnected()
{
    // Register plugin with Software
    QJsonObject jsonObject;
    jsonObject["event"] = mRegisterEvent;
    jsonObject["uuid"] = mPluginUUID;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void ConnectionManager::OnDisonnected()
{
    QString messageString = "Socket disconnected: " + mWebsocketClient->errorString();
    Logger::LogToFile(messageString);

    qApp->quit();
    this->deleteLater();
}

void ConnectionManager::OnError(QAbstractSocket::SocketError error)
{
    QString errorString = "Socket error: " + mWebsocketClient->errorString();
    Logger::LogToFile(errorString);
}

void ConnectionManager::OnMessage(const QString &message)
{
    Logger::LogToServer("OnMessage: " + message);

    try {
        QJsonObject receivedJson = QJsonDocument::fromJson(message.toUtf8()).object();
        QString event = receivedJson.value(mSDKCommonEvent).toString();
        QString context = receivedJson.value(mSDKCommonContext).toString();
        QString action = receivedJson.value(mSDKCommonAction).toString();
        QString deviceID = receivedJson.value(mSDKCommonDevice).toString();
        QJsonObject payload = receivedJson.value(mSDKCommonPayload).toObject();
        if (event == mSDKEventTouchTap) {
                mPlugin->TouchTapForAction(action, context, payload, deviceID);
            } else if (event == mSDKEventTouchEvent) {
                mPlugin->TouchEventForAction(action, context, payload, deviceID);
            } else if (event == mSDKEventDeleteAction) {
                mPlugin->DeleteActionForAction(action, context, payload, deviceID);
            } else if (event == mSDKEventTitleParametersDidChange) {
                mPlugin->TitleParametersDidChangeForAction(action, context, payload, deviceID);
            } else if (event == mSDKEventPropertyInspectorDidAppear) {
                mPlugin->PropertyInspectorDidAppearForAction(action, context, payload, deviceID);
            } else if (event == mSDKEventPropertyInspectorDidDisappear) {
                mPlugin->PropertyInspectorDidDisappearForAction(action, context, payload, deviceID);
            } else if (event == mSDKEventApplicationDidLaunch) {
                mPlugin->ApplicationDidLaunch(payload);
            } else if (event == mSDKEventApplicationDidTerminate) {
                mPlugin->ApplicationDidTerminate(payload);
            } else if (event == mSDKEventKeyDown) {
            mPlugin->KeyDownForAction(action, context, payload, deviceID);
        } else if (event == mSDKEventKeyUp) {
            mPlugin->KeyUpForAction(action, context, payload, deviceID);
        } else if (event == mSDKEventWillAppear) {
            mPlugin->WillAppearForAction(action, context, payload, deviceID);
        } else if (event == mSDKEventWillDisappear) {
            mPlugin->WillDisappearForAction(action, context, payload, deviceID);
        } else if (event == mSDKEventDidReceiveSettings) {
            mPlugin->DidReceiveSettings(action, context, payload, deviceID);
        } else if (event == mSDKEventDidReceiveGlobalSettings) {
            mPlugin->DidReceiveGlobalSettings(payload);
        } else if (event == mSDKEventDeviceDidConnect) {
            QJsonObject deviceInfo = receivedJson.value(mSDKCommonDeviceInfo).toObject();
            mPlugin->DeviceDidConnect(deviceID, deviceInfo);
        } else if (event == mSDKEventDeviceDidDisconnect) {
            mPlugin->DeviceDidDisconnect(deviceID);
        } else if (event == mSDKEventSendToPlugin) {
            mPlugin->SendToPlugin(action, context, payload, deviceID);
        } else if (event == mSDKEventSystemDidWakeUp) {
            mPlugin->SystemDidWakeUp();
        } else if (event == mSDKEventDialDown) {
            mPlugin->DialDownForAction(action, context, payload, deviceID);
        } else if (event == mSDKEventDialUp) {
            mPlugin->DialUpForAction(action, context, payload, deviceID);
        } else if (event == mSDKEventDialRotate) {
            mPlugin->DialRotateForAction(action, context, payload, deviceID);
        }
    } catch (...) {
    }
}

void ConnectionManager::SendMessage(const QString &message)
{
    if (mWebsocketClient) {
        mWebsocketClient->sendTextMessage(message);
    }
}

void ConnectionManager::TriggerKey(const QString &inContext, const QJsonArray &keyList) {
    QJsonObject payload;
    payload["keyList"] = keyList;
    QJsonObject jsonObject;
    jsonObject["event"] = "triggerKey";
    jsonObject["context"] = inContext;
    jsonObject["payload"] = payload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}
void ConnectionManager::OpenUrl(const QString &url) {
    QJsonObject payload;
    payload["url"] = url;
    QJsonObject jsonObject;
    jsonObject["event"] = "openUrl";
    jsonObject["payload"] = payload;
    SendMessage(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}
