// @copyright  (c) 2022, ljj
// This source code is licensed under the MIT-style license found in the LICENSE file.

#include "HSDPlugin.h"

#include "NlohmannJSONUtils.h"
#include "HSDConnectionManager.h"
#include "HSDLogger.h"

#include <atomic>
#include <mutex>

#include "HSDAction.h"

using json = nlohmann::json;

HSDPlugin::HSDPlugin() : HSDBasePlugin()
{
}

HSDPlugin::~HSDPlugin()
{
}

void HSDPlugin::KeyDownForAction(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDeviceID)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("No action for keydown - " + inAction + " " + inContext);
    return;
  }
  action->KeyDown(inPayload);
}

void HSDPlugin::KeyUpForAction(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDeviceID)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("No action for keyup - " + inAction + " " + inContext);
    return;
  }
  action->KeyUp(inPayload);
}

void HSDPlugin::DialDownForAction(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDeviceID)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("No action for dialPress - " + inAction + " " + inContext);
    return;
  }
  action->DialDown(inPayload);
}

void HSDPlugin::DialUpForAction(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDeviceID)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("No action for dialPress - " + inAction + " " + inContext);
    return;
  }
  action->DialUp(inPayload);
}

void HSDPlugin::DialRotateForAction(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDeviceID)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("No action for dialRotate - " + inAction + " " + inContext);
    return;
  }
  int ticks(inPayload["ticks"]);
  bool pressed(inPayload["pressed"]);
  if (inPayload["ticks"] < 0)
  {
    action->RotateCounterClockwise(inPayload, static_cast<const unsigned int>(-ticks), pressed);
  }
  else
  {
    action->RotateClockwise(inPayload, static_cast<const unsigned int>(ticks), pressed);
  }
}

void HSDPlugin::WillAppearForAction(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDeviceID)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("No action for WillAppear - " + inAction + " " + inContext);
    return;
  }
  action->WillAppear(inPayload);
}

void HSDPlugin::WillDisappearForAction(
    const std::string& inAction,
    const std::string& inContext,
    const json& inPayload,
    const std::string& inDeviceID)
{
    auto action = FindAction(inAction, inContext);
    if (!action)
    {
        HSDLogger::LogMessage("No action for WillDisAppear - " + inAction + " " + inContext);
        return;
    }
    action->WillDisAppear(inPayload);
}

void HSDPlugin::SendToPlugin(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDevice)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("Received plugin request for unknown action " + inAction + " " + inContext);
    return;
  }
  action->SendToPlugin(inPayload);
}

void HSDPlugin::DidReceiveSettings(
    const std::string &inAction,
    const std::string &inContext,
    const json &inPayload,
    const std::string &inDevice)
{
  auto action = EventAction(inAction, inContext);
  if (!action)
  {
    HSDLogger::LogMessage("No action for DidReceiveSettings: " + inAction + " " + inContext);
    return;
  }
  action->DidReceiveSettings(inPayload);
}

void HSDPlugin::TouchTapForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->TouchTap(inPayload);
}
void HSDPlugin::TouchEventForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->TouchEvent(inPayload);
}
void HSDPlugin::DeleteActionForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID)
{
    auto action = FindAction(inAction, inContext);
    if (action) action->DeleteAction(inPayload);
    mEventActions.erase(inContext);
    RemoveAction(inAction, inContext);
}
void HSDPlugin::TitleParametersDidChangeForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->TitleParametersDidChange(inPayload);
}
void HSDPlugin::PropertyInspectorDidAppearForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->PropertyInspectorDidAppear(inPayload);
}
void HSDPlugin::PropertyInspectorDidDisappearForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID)
{
    auto action = EventAction(inAction, inContext);
    if (action) action->PropertyInspectorDidDisappear(inPayload);
}

std::shared_ptr<HSDAction> HSDPlugin::EventAction(const std::string &action, const std::string &context) {
    auto instance = GetOrCreateAction(action, context);
    if (instance) mEventActions[context] = instance;
    return instance;
}
