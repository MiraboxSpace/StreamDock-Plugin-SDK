#include "Plugin.h"
#include "Action.h"
#include "ConnectionManager.h"
#include "Logger.h"

Plugin::Plugin()
    : BasePlugin()
{
}

Plugin::~Plugin()
{
}

void Plugin::KeyDownForAction(const QString &inAction,
                              const QString &inContext,
                              const QJsonObject &inPayload,
                              const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for keydown - %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->KeyDown(inPayload);
}

void Plugin::KeyUpForAction(const QString &inAction,
                            const QString &inContext,
                            const QJsonObject &inPayload,
                            const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for keyup - %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->KeyUp(inPayload);
}

void Plugin::DialDownForAction(const QString &inAction,
                               const QString &inContext,
                               const QJsonObject &inPayload,
                               const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for dialPress - %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->DialDown(inPayload);
}

void Plugin::DialUpForAction(const QString &inAction,
                             const QString &inContext,
                             const QJsonObject &inPayload,
                             const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for dialPress - %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->DialUp(inPayload);
}

void Plugin::DialRotateForAction(const QString &inAction,
                                 const QString &inContext,
                                 const QJsonObject &inPayload,
                                 const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for dialRotate - %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    int ticks = inPayload.value("ticks").toInt();
    bool pressed = inPayload.value("pressed").toBool();
    if (ticks < 0)
    {
        action->RotateCounterClockwise(inPayload, static_cast<const unsigned int>(-ticks), pressed);
    }
    else
    {
        action->RotateClockwise(inPayload, static_cast<const unsigned int>(ticks), pressed);
    }
}

void Plugin::WillAppearForAction(const QString &inAction,
                                 const QString &inContext,
                                 const QJsonObject &inPayload,
                                 const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for WillAppear - %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->WillAppear(inPayload);
}

void Plugin::WillDisappearForAction(const QString &inAction,
                                    const QString &inContext,
                                    const QJsonObject &inPayload,
                                    const QString &inDeviceID)
{
    auto action = FindAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for WillDisappear - %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->WillDisappear(inPayload);
    if (!action->KeepAliveWhenHidden()) {
        mEventActions.erase(inContext);
        RemoveAction(inAction, inContext);
    }
}

void Plugin::SendToPlugin(const QString &inAction,
                          const QString &inContext,
                          const QJsonObject &inPayload,
                          const QString &inDevice)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("Received plugin request for unknown action %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->SendToPlugin(inPayload);
}

void Plugin::DidReceiveSettings(const QString &inAction,
                                const QString &inContext,
                                const QJsonObject &inPayload,
                                const QString &inDevice)
{
    auto action = EventAction(inAction, inContext);
    if (!action)
    {
        Logger::LogToFile(QString("No action for DidReceiveSettings: %1 %2")
                              .arg(inAction)
                              .arg(inContext));
        return;
    }
    action->DidReceiveSettings(inPayload);
}

void Plugin::TouchTapForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->TouchTap(inPayload);
}
void Plugin::TouchEventForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->TouchEvent(inPayload);
}
void Plugin::DeleteActionForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID)
{
    auto action = FindAction(inAction, inContext);
    if (action) action->DeleteAction(inPayload);
    mEventActions.erase(inContext);
    RemoveAction(inAction, inContext);
}
void Plugin::TitleParametersDidChangeForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->TitleParametersDidChange(inPayload);
}
void Plugin::PropertyInspectorDidAppearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->PropertyInspectorDidAppear(inPayload);
}
void Plugin::PropertyInspectorDidDisappearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->PropertyInspectorDidDisappear(inPayload);
}

Action *Plugin::EventAction(const QString &action, const QString &context) {
    auto instance = GetOrCreateAction(action, context);
    if (instance) mEventActions[context] = instance;
    return instance;
}
