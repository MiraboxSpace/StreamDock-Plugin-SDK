#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include <QString>
#include <QWebSocket>
#include "BasePlugin.h"
#include "SDKDefines.h"

class ConnectionManager : public QObject
{
    Q_OBJECT

public:
    ConnectionManager(int inPort,
                      const QString &inPluginUUID,
                      const QString &inRegisterEvent,
                      const QString &inInfo,
                      BasePlugin *inPlugin,
                      QObject *parent = nullptr);
    ~ConnectionManager();

    // API to communicate with the Stream Dock application
    void SetTitle(const QString &inTitle,
                  const QString &inContext,
                  mSDKTarget inTarget,
                  int state = -1);
    void SetImage(const QString &inBase64ImageString,
                  const QString &inContext,
                  mSDKTarget inTarget,
                  int state = -1);
    void ShowAlertForContext(const QString &inContext);
    void ShowOKForContext(const QString &inContext);
    void SetSettings(const QJsonObject &inPayload,
                     const QString &inContext);
    void GetGlobalSettings();
    void SetGlobalSettings(const QJsonObject &inPayload);
    void SetState(int inState, const QString &inContext);
    void SendToPropertyInspector(const QString &inAction,
                                 const QString &inContext,
                                 const QJsonObject &inPayload);
    void SwitchToProfile(const QString &inDeviceID,
                         const QString &inProfileName, int page = -1);
    void TriggerKey(const QString &inContext, const QJsonArray &keyList);
    void OpenUrl(const QString &url);
    void LogMessage(const QString &inMessage);

    // Run
    void Run();

private:
    // Websocket callbacks
    void OnConnected();
    void OnDisonnected();
    void OnError(QAbstractSocket::SocketError error);
    void OnMessage(const QString &message);
    // Send websocket message
    void SendMessage(const QString &message);

    // Member variables
    int mPort = 0;
    QString mPluginUUID;
    QString mRegisterEvent;
    BasePlugin *mPlugin = nullptr;
    QWebSocket *mWebsocketClient = nullptr;
};

#endif // CONNECTIONMANAGER_H
