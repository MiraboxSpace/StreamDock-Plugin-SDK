#include <map>
/* Copyright (c) 2018, Corsair Memory, Inc.
 * Copyright (c) 2020-present, Fred Emmott
 *
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file.
 */

#include "HSDBasePlugin.h"

#include <memory>

class HSDAction;

/** Coordinator class for `HSDAction`-based plugins.
 *
 * Plugins should:
 * - create one or more subclasses of `HSDAction`, one for each action type.
 * - create exactly one subclass of `HSDPlugin`, which implements
 *   `GetOrCreateAction()` to retrieve instances of an `HSDAction`.
 */
class HSDPlugin : public HSDBasePlugin
{
protected:
  /** Create or retrieve an HSDAction instance for the given action/context.
   *
   * Return a null/empty shared_ptr if the action is unrecognized.
   */
  virtual std::shared_ptr<HSDAction>
  GetOrCreateAction(const std::string &action, const std::string &context) = 0;

    // Lookup without construction; override together with GetOrCreateAction.
    virtual std::shared_ptr<HSDAction> FindAction(const std::string &action, const std::string &context) {
        auto it = mEventActions.find(context);
        return it == mEventActions.end() ? nullptr : it->second;
    }
    std::shared_ptr<HSDAction> EventAction(const std::string &action, const std::string &context);
    std::map<std::string, std::shared_ptr<HSDAction>> mEventActions;
    virtual bool RemoveAction(const std::string &action, const std::string &context) { return false; }

public:
  HSDPlugin();
  virtual ~HSDPlugin();

    void TouchTapForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID) override;
    void TouchEventForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID) override;
    void DeleteActionForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID) override;
    void TitleParametersDidChangeForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID) override;
    void PropertyInspectorDidAppearForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID) override;
    void PropertyInspectorDidDisappearForAction(const std::string &inAction, const std::string &inContext, const nlohmann::json &inPayload, const std::string &inDeviceID) override;

  virtual void KeyDownForAction(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDeviceID) override;

  virtual void KeyUpForAction(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDeviceID) override;

  virtual void DialDownForAction(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDeviceID) override;

  virtual void DialUpForAction(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDeviceID) override;

  virtual void DialRotateForAction(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDeviceID) override;

  virtual void WillAppearForAction(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDeviceID) override;

  virtual void WillDisappearForAction(
      const std::string& inAction,
      const std::string& inContext,
      const nlohmann::json& inPayload,
      const std::string& inDeviceID) override;

  virtual void SendToPlugin(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDevice) override;

  virtual void DidReceiveSettings(
      const std::string &inAction,
      const std::string &inContext,
      const nlohmann::json &inPayload,
      const std::string &inDevice) override;
};
