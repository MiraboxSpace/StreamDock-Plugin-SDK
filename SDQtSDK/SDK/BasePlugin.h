#ifndef BASEPLUGIN_H
#define BASEPLUGIN_H

#include <QJsonObject>
#include <QString>

class ConnectionManager;

class BasePlugin
{
public:
    BasePlugin();
    virtual ~BasePlugin();

    void SetConnectionManager(ConnectionManager *inConnectionManager);

    virtual void TouchTapForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID);
    virtual void TouchEventForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID);
    virtual void DeleteActionForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID);
    virtual void TitleParametersDidChangeForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID);
    virtual void PropertyInspectorDidAppearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID);
    virtual void PropertyInspectorDidDisappearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID);
    virtual void ApplicationDidLaunch(const QJsonObject &inPayload);
    virtual void ApplicationDidTerminate(const QJsonObject &inPayload);

    virtual void DidReceiveGlobalSettings(const QJsonObject &inPayload);

    virtual void DidReceiveSettings(const QString &inAction,
                                    const QString &inContext,
                                    const QJsonObject &inPayload,
                                    const QString &inDeviceID);

    virtual void KeyDownForAction(const QString &inAction,
                                  const QString &inContext,
                                  const QJsonObject &inPayload,
                                  const QString &inDeviceID);

    virtual void KeyUpForAction(const QString &inAction,
                                const QString &inContext,
                                const QJsonObject &inPayload,
                                const QString &inDeviceID);

    virtual void DialDownForAction(const QString &inAction,
                                    const QString &inContext,
                                    const QJsonObject &inPayload,
                                    const QString &inDeviceID);

    virtual void DialUpForAction(const QString &inAction,
                                    const QString &inContext,
                                    const QJsonObject &inPayload,
                                    const QString &inDeviceID);

    virtual void DialRotateForAction(const QString &inAction,
                                     const QString &inContext,
                                     const QJsonObject &inPayload,
                                     const QString &inDeviceID);

    virtual void WillAppearForAction(const QString &inAction,
                                     const QString &inContext,
                                     const QJsonObject &inPayload,
                                     const QString &inDeviceID);

    virtual void WillDisappearForAction(const QString &inAction,
                                        const QString &inContext,
                                        const QJsonObject &inPayload,
                                        const QString &inDeviceID);

    virtual void DeviceDidConnect(const QString &inDeviceID,
                                  const QJsonObject &inDeviceInfo);

    virtual void DeviceDidDisconnect(const QString &inDeviceID);

    virtual void SystemDidWakeUp();

    virtual void SendToPlugin(const QString &inAction,
                              const QString &inContext,
                              const QJsonObject &inPayload,
                              const QString &inDeviceID);

protected:
    ConnectionManager *mConnectionManager = nullptr;
};

#endif // BASEPLUGIN_H
