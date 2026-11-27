#pragma once

#include "StreamDockCPPSDK/StreamDockSDK/HSDPlugin.h"
#include "HSDExampleAction.h"
#include <set>
#include <mutex>


class HSDExamplePlugin : public HSDPlugin
{
public:
    bool RemoveAction(const std::string &action, const std::string &context) override;
    std::shared_ptr<HSDAction> FindAction(const std::string &action, const std::string &context) override;
    using HSDPlugin::HSDPlugin;

    virtual std::shared_ptr<HSDAction> GetOrCreateAction(const std::string& action, const std::string& context) override;

    // Overriding from ESDBasePlugin
    virtual void DidReceiveGlobalSettings(const nlohmann::json& payload) override;
    
private:
    std::mutex mVisibleContextsMutex;
    std::set<std::string> mVisibleContexts;
    std::map<std::string, std::shared_ptr<HSDAction>> mActions;
};

