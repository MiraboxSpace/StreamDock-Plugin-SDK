#include "ExamplePlugin.h"
#include <Logger.h>
#include <QMutexLocker>

ExamplePlugin::ExamplePlugin()
    : Plugin()
{
}

ExamplePlugin::~ExamplePlugin()
{
    QMutexLocker mMutexLocker(&mVisibleContextsMutex);

    QStringList keys = mActions.keys();
    for (qint32 i = 0; i < keys.size(); ++i) {
        delete mActions.value(keys[i]);
    }
}

Action *ExamplePlugin::GetOrCreateAction(const QString &action, const QString &context)
{
    QMutexLocker mMutexLocker(&mVisibleContextsMutex);

    if (action == "com.hotspot.streamdockcppsdk.demo.myaction1" ||
        action == "com.hotspot.streamdockcppsdk.demo.myaction2") {
        if (!mActions.contains(context)) {
            ExampleAction *tmpAction = new ExampleAction(mConnectionManager, action, context);
            mActions.insert(context, tmpAction);
        }
        return mActions.value(context);
    }

    Logger::LogToServer(QString("Asked to get or create unknown action: ").append(action));
    return nullptr;
}

bool ExamplePlugin::RemoveAction(const QString &action, const QString &context)
{
    QMutexLocker mMutexLocker(&mVisibleContextsMutex);

    if (action == "com.hotspot.streamdockcppsdk.demo.myaction1" ||
        action == "com.hotspot.streamdockcppsdk.demo.myaction2") {
        if (mActions.contains(context)) {
            delete mActions.take(context);
            return true;
        } else {
            return false;
        }
    }

    Logger::LogToServer(QString("Asked to get or create unknown action: ").append(action));
    return false;
}

Action *ExamplePlugin::FindAction(const QString &action, const QString &context) {
    QMutexLocker locker(&mVisibleContextsMutex);
    auto instance = mActions.value(context, nullptr);
    return instance && instance->GetAction() == action ? instance : nullptr;
}
