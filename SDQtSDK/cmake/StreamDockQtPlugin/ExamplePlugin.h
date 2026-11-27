#ifndef EXAMPLEPLUGIN_H
#define EXAMPLEPLUGIN_H

#include <Plugin.h>
#include <ExampleAction.h>
#include <QHash>
#include <QMutex>

class ExamplePlugin : public Plugin
{
public:
    Action *FindAction(const QString &action, const QString &context) override;
    ExamplePlugin();
    ~ExamplePlugin();
    virtual Action *GetOrCreateAction(const QString &action, const QString &context) override;
    virtual bool RemoveAction(const QString &action, const QString &context) override;

private:
    QMutex mVisibleContextsMutex;
    QHash<QString, ExampleAction *> mActions;
};

#endif // EXAMPLEPLUGIN_H
