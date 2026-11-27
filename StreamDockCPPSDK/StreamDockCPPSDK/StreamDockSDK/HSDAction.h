// @copyright  (c) 2022, ljj
// This source code is licensed under the MIT-style license found in the LICENSE file.

#pragma once

#include "HSDSDKDefines.h"

#include <nlohmann/json.hpp>
#include <string>

class HSDConnectionManager;

/** Class representing a specific action (kind of button).
 *
 * A plugin can provide multiple actions, e.g.:
 * - 'mute on'
 * - 'mute off'
 * - 'toggle mute'
 *
 * Each of these can be represented by an `HSDAction` subclass.
 *
 * This class is intended to be used in conjunction with an `HSDPlugin`
 * subclass; your `HSDPlugin` subclass should contain minimum logic beyond
 * storing and instantiating `HSDAction` subclass instances as needed.
 *
 * If your action reflects state outside of the plugin (e.g. hardware state,
 * the current time, state in another application such as OBS), you may want
 * to use `HSDActionWithExternalState`.
 */
class HSDAction {
 public:
  HSDAction(HSDConnectionManager* hsd_connection, const std::string& action, const std::string& context);
  virtual ~HSDAction();

  std::string GetAction() const;
  std::string GetContext() const;

    virtual void TouchTap(const nlohmann::json &payload);
    virtual void TouchEvent(const nlohmann::json &payload);
    virtual void DeleteAction(const nlohmann::json &payload);
    virtual void TitleParametersDidChange(const nlohmann::json &payload);
    virtual void PropertyInspectorDidAppear(const nlohmann::json &payload);
    virtual void PropertyInspectorDidDisappear(const nlohmann::json &payload);

  virtual void DidReceiveSettings(const nlohmann::json& payload);
  virtual void KeyDown(const nlohmann::json& payload);
  virtual void KeyUp(const nlohmann::json& payload);
  virtual void DialUp(const nlohmann::json& payload);
  virtual void DialDown(const nlohmann::json& payload);
  virtual void SendToPlugin(const nlohmann::json& payload);
  virtual void WillAppear(const nlohmann::json& payload);
  virtual void WillDisAppear(const nlohmann::json& payload);
  virtual void RotateClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed);
  virtual void RotateCounterClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed);

 protected:
  HSDConnectionManager* GetHSDConnection() const;

  // Convenience wrappers for GetHSDConnection()->foo()
    void TriggerKey(const nlohmann::json &keyList);
    void OpenUrl(const std::string &url);
  void SetState(int state);
  void SetTitle(const std::string& title, ESDSDKTarget = kESDSDKTarget_HardwareAndSoftware, int state = -1);
  void SetImage(const std::string& inBase64ImageString, ESDSDKTarget = kESDSDKTarget_HardwareAndSoftware, int state = -1);
  void ShowAlert();
  void ShowOK();
  void SetSettings(const nlohmann::json& inSettings);
  void SendToPropertyInspector(const nlohmann::json& payload);

 private:
  std::string mAction;
  std::string mContext;
  HSDConnectionManager* mHSDConnection = nullptr;
};
