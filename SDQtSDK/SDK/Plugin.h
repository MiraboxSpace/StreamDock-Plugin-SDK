#include <map>
#ifndef PLUGIN_H
#define PLUGIN_H

#include "BasePlugin.h"
#include <QJsonObject>
#include <QString>

class Action;

class Plugin : public BasePlugin
{
protected:
    /**
     * Create or retrieve an Action instance for the given action/context.
     *
     * Return a null/empty ptr if the action is unrecognized.
     */
    virtual Action *GetOrCreateAction(const QString &action, const QString &context) = 0;
    /**
     * Remove an Action instance for the given action/context.
     */
    virtual bool RemoveAction(const QString &action, const QString &context) = 0;

    // Lookup without construction; override together with GetOrCreateAction.
    virtual Action *FindAction(const QString &action, const QString &context) {
        auto it = mEventActions.find(context);
        return it == mEventActions.end() ? nullptr : it->second;
    }
    Action *EventAction(const QString &action, const QString &context);
    std::map<QString, Action *> mEventActions;

public:
    Plugin();
    virtual ~Plugin();

    void TouchTapForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) override;
    void TouchEventForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) override;
    void DeleteActionForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) override;
    void TitleParametersDidChangeForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) override;
    void PropertyInspectorDidAppearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) override;
    void PropertyInspectorDidDisappearForAction(const QString &inAction, const QString &inContext, const QJsonObject &inPayload, const QString &inDeviceID) override;

    virtual void KeyDownForAction(const QString &inAction,
                                  const QString &inContext,
                                  const QJsonObject &inPayload,
                                  const QString &inDeviceID) override;

    virtual void KeyUpForAction(const QString &inAction,
                                const QString &inContext,
                                const QJsonObject &inPayload,
                                const QString &inDeviceID) override;

    virtual void DialDownForAction(const QString &inAction,
                                   const QString &inContext,
                                   const QJsonObject &inPayload,
                                   const QString &inDeviceID) override;
                                   
    virtual void DialUpForAction(const QString &inAction,
                                   const QString &inContext,
                                   const QJsonObject &inPayload,
                                   const QString &inDeviceID) override;

    virtual void DialRotateForAction(const QString &inAction,
                                     const QString &inContext,
                                     const QJsonObject &inPayload,
                                     const QString &inDeviceID) override;

    virtual void WillAppearForAction(const QString &inAction,
                                     const QString &inContext,
                                     const QJsonObject &inPayload,
                                     const QString &inDeviceID) override;

    virtual void WillDisappearForAction(const QString &inAction,
                                        const QString &inContext,
                                        const QJsonObject &inPayload,
                                        const QString &inDeviceID) override;

    virtual void SendToPlugin(const QString &inAction,
                              const QString &inContext,
                              const QJsonObject &inPayload,
                              const QString &inDevice) override;

    virtual void DidReceiveSettings(const QString &inAction,
                                    const QString &inContext,
                                    const QJsonObject &inPayload,
                                    const QString &inDevice) override;
};

#endif // PLUGIN_H
