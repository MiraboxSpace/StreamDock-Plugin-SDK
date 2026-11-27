#include "Action.h"
#include "ConnectionManager.h"

Action::Action(ConnectionManager *connection, const QString &action, const QString &context)
  : mConnectionManager(connection), mAction(action), mContext(context)
{
}

Action::~Action()
{
}

QString Action::GetContext() const
{
    return mContext;
}

QString Action::GetAction() const
{
    return mAction;
}

ConnectionManager *Action::GetConnectionManager() const
{
    return mConnectionManager;
}

void Action::DidReceiveSettings(const QJsonObject &payload)
{
}

void Action::KeyDown(const QJsonObject &payload)
{
}

void Action::KeyUp(const QJsonObject &payload)
{
}

void Action::DialUp(const QJsonObject &payload)
{
}

void Action::DialDown(const QJsonObject &payload)
{
}

void Action::RotateClockwise(const QJsonObject &payload, const unsigned int ticks, const bool pressed)
{
}

void Action::RotateCounterClockwise(const QJsonObject &payload, const unsigned int ticks, const bool pressed)
{
}

void Action::SendToPlugin(const QJsonObject &payload)
{
}

void Action::WillAppear(const QJsonObject &payload)
{
}

void Action::WillDisappear(const QJsonObject &payload)
{
}

void Action::SetState(int state)
{
    GetConnectionManager()->SetState(state, mContext);
}

void Action::SetTitle(const QString &title, mSDKTarget target, int state)
{
    GetConnectionManager()->SetTitle(title, mContext, target, state);
}

void Action::SetImage(const QString &inBase64ImageString, mSDKTarget target, int state)
{
    GetConnectionManager()->SetImage(inBase64ImageString, mContext, target, state);
}

void Action::SetSettings(const QJsonObject &inPayload)
{
    GetConnectionManager()->SetSettings(inPayload, mContext);
}

void Action::ShowAlert()
{
    GetConnectionManager()->ShowAlertForContext(mContext);
}

void Action::ShowOK()
{
    GetConnectionManager()->ShowOKForContext(mContext);
}

void Action::SendToPropertyInspector(const QJsonObject &inPayload)
{
    GetConnectionManager()->SendToPropertyInspector(mAction, mContext, inPayload);
}

void Action::TouchTap(const QJsonObject &payload) { }
void Action::TouchEvent(const QJsonObject &payload) { }
void Action::DeleteAction(const QJsonObject &payload) { }
void Action::TitleParametersDidChange(const QJsonObject &payload) { }
void Action::PropertyInspectorDidAppear(const QJsonObject &payload) { }
void Action::PropertyInspectorDidDisappear(const QJsonObject &payload) { }

void Action::TriggerKey(const QJsonArray &keyList) {
    GetConnectionManager()->TriggerKey(mContext, keyList);
}
void Action::OpenUrl(const QString &url) {
    GetConnectionManager()->OpenUrl(url);
}
