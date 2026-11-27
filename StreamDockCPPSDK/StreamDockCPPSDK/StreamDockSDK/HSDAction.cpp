// @copyright  (c) 2022, ljj
// This source code is licensed under the MIT-style license found in the LICENSE file.

#include "HSDAction.h"

#include "HSDConnectionManager.h"

HSDAction::HSDAction(HSDConnectionManager* hsd_connection, const std::string& action, const std::string& context)
  : mHSDConnection(hsd_connection), mAction(action), mContext(context) {
}

HSDAction::~HSDAction() {
}

std::string HSDAction::GetContext() const {
  return mContext;
}

std::string HSDAction::GetAction() const {
  return mAction;
}

void HSDAction::DidReceiveSettings(const nlohmann::json& payload) {
}

void HSDAction::KeyDown(const nlohmann::json& payload) {
}

void HSDAction::KeyUp(const nlohmann::json& payload) {
}

void HSDAction::DialUp(const nlohmann::json& payload) {
}

void HSDAction::DialDown(const nlohmann::json& payload) {
}

void HSDAction::RotateClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed) {
}

void HSDAction::RotateCounterClockwise(const nlohmann::json& payload, const unsigned int ticks, const bool pressed) {
}

void HSDAction::SendToPlugin(const nlohmann::json& payload) {
}

void HSDAction::WillAppear(const nlohmann::json& payload) {
}

void HSDAction::WillDisAppear(const nlohmann::json& payload) {
}

HSDConnectionManager* HSDAction::GetHSDConnection() const {
	return mHSDConnection;
}

void HSDAction::SetState(int state) {
  GetHSDConnection()->SetState(state, mContext);
}

void HSDAction::SetTitle(const std::string& title, ESDSDKTarget target, int state) {
  GetHSDConnection()->SetTitle(title, mContext, target, state);
}

void HSDAction::SetImage(const std::string& inBase64ImageString, ESDSDKTarget target, int state) {
  GetHSDConnection()->SetImage(inBase64ImageString, mContext, target, state);
}

void HSDAction::ShowAlert() {
  GetHSDConnection()->ShowAlertForContext(mContext);
}

void HSDAction::ShowOK() {
  GetHSDConnection()->ShowOKForContext(mContext);
}

void HSDAction::SetSettings(const nlohmann::json& inSettings) {
	GetHSDConnection()->SetSettings(inSettings, mContext);
}

void HSDAction::SendToPropertyInspector(const nlohmann::json& payload) {
  GetHSDConnection()->SendToPropertyInspector(mAction, mContext, payload);
}

void HSDAction::TouchTap(const nlohmann::json &payload) { }
void HSDAction::TouchEvent(const nlohmann::json &payload) { }
void HSDAction::DeleteAction(const nlohmann::json &payload) { }
void HSDAction::TitleParametersDidChange(const nlohmann::json &payload) { }
void HSDAction::PropertyInspectorDidAppear(const nlohmann::json &payload) { }
void HSDAction::PropertyInspectorDidDisappear(const nlohmann::json &payload) { }

void HSDAction::TriggerKey(const nlohmann::json &keyList) {
    GetHSDConnection()->TriggerKey(mContext, keyList);
}
void HSDAction::OpenUrl(const std::string &url) {
    GetHSDConnection()->OpenUrl(url);
}
