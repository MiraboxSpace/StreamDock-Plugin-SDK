#include "HSDExamplePlugin.h"

#include "StreamDockCPPSDK/StreamDockSDK/NlohmannJSONUtils.h"
#include "StreamDockCPPSDK/StreamDockSDK/HSDLogger.h"

std::shared_ptr<HSDAction> HSDExamplePlugin::GetOrCreateAction(const std::string& action, const std::string& context)
{
    auto it = mActions.find(context);
    if (it != mActions.end()) {
        return it->second;
    }

    if (action == "com.hotspot.streamdockcppsdk.demo.myaction1" ||
        action == "com.hotspot.streamdockcppsdk.demo.myaction2") {
        auto impl = std::make_shared<HSDExampleAction>(
            mConnectionManager,
            action,
            context
            );
        mActions.emplace(context, impl);
        return impl;
    }

    HSDLogger::LogMessage("Asked to get or create unknown action: " + action);
    return nullptr;
}

void HSDExamplePlugin::DidReceiveGlobalSettings(const nlohmann::json& payload)
{
    HSDLogger::LogMessage("Received global settings");
    HSDLogger::LogMessage("Global settings: " + payload.dump());

    // Do plugin-wide stuff here, e.g. reconnect to application being
    // controlled

    for (const auto& action : mActions) {
        // ... then pass it on to each action, e.g.:
        // action->setApplicationConnection(myConnectionObject);
    }
}

std::shared_ptr<HSDAction> HSDExamplePlugin::FindAction(const std::string &action, const std::string &context) {
    auto it = mActions.find(context);
    return it != mActions.end() && it->second->GetAction() == action ? it->second : nullptr;
}

bool HSDExamplePlugin::RemoveAction(const std::string &action, const std::string &context) {
    if (!FindAction(action, context)) return false;
    return mActions.erase(context) != 0;
}
