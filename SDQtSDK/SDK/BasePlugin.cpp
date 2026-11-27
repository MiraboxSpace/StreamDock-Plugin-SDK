#include "BasePlugin.h"

BasePlugin::BasePlugin()
{
}

BasePlugin::~BasePlugin()
{
}

void BasePlugin::SetConnectionManager(ConnectionManager *inConnectionManager)
{
    mConnectionManager = inConnectionManager;
}

void BasePlugin::DidReceiveGlobalSettings(const QJsonObject &inPayload)
{
}

void BasePlugin::DidReceiveSettings(const QString &inAction,
                                    const QString &inContext,
                                    const QJsonObject &inPayload,
                                    const QString &inDeviceID)
{
}

void BasePlugin::KeyDownForAction(const QString &inAction,
                                  const QString &inContext,
                                  const QJsonObject &inPayload,
                                  const QString &inDeviceID)
{
}

void BasePlugin::KeyUpForAction(const QString &inAction,
                                const QString &inContext,
                                const QJsonObject &inPayload,
                                const QString &inDeviceID)
{
}

void BasePlugin::DialDownForAction(const QString &inAction,
                                    const QString &inContext,
                                    const QJsonObject &inPayload,
                                    const QString &inDeviceID)
{
}


void BasePlugin::DialUpForAction(const QString &inAction,
    const QString &inContext,
    const QJsonObject &inPayload,
    const QString &inDeviceID)
{
}


void BasePlugin::DialRotateForAction(const QString &inAction,
                                     const QString &inContext,
                                     const QJsonObject &inPayload,
                                     const QString &inDeviceID)
{
}

void BasePlugin::WillAppearForAction(const QString &inAction,
                                     const QString &inContext,
                                     const QJsonObject &inPayload,
                                     const QString &inDeviceID)
{
}

void BasePlugin::WillDisappearForAction(const QString &inAction,
                                        const QString &inContext,
                                        const QJsonObject &inPayload,
                                        const QString &inDeviceID)
{
}

void BasePlugin::DeviceDidConnect(const QString &inDeviceID,
                                  const QJsonObject &inDeviceInfo)
{
}

void BasePlugin::DeviceDidDisconnect(const QString &inDeviceID)
{
}

void BasePlugin::SystemDidWakeUp()
{
}

void BasePlugin::SendToPlugin(const QString &inAction,
                              const QString &inContext,
                              const QJsonObject &inPayload,
                              const QString &inDeviceID)
{
}

void BasePlugin::TouchTapForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) { }
void BasePlugin::TouchEventForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) { }
void BasePlugin::DeleteActionForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) { }
void BasePlugin::TitleParametersDidChangeForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) { }
void BasePlugin::PropertyInspectorDidAppearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) { }
void BasePlugin::PropertyInspectorDidDisappearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) { }
void BasePlugin::ApplicationDidLaunch(const QJsonObject &inPayload) { }
void BasePlugin::ApplicationDidTerminate(const QJsonObject &inPayload) { }
