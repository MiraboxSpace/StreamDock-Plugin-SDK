#ifndef SDKDEFINES_H
#define SDKDEFINES_H

//
// Common base-interface
//
#define mSDKCommonAction "action"
#define mSDKCommonEvent "event"
#define mSDKCommonContext "context"
#define mSDKCommonPayload "payload"
#define mSDKCommonDevice "device"
#define mSDKCommonDeviceInfo "deviceInfo"

//
// Events
//
#define mSDKEventKeyDown "keyDown"
#define mSDKEventKeyUp "keyUp"
#define mSDKEventWillAppear "willAppear"
#define mSDKEventWillDisappear "willDisappear"
#define mSDKEventDeviceDidConnect "deviceDidConnect"
#define mSDKEventDeviceDidDisconnect "deviceDidDisconnect"
#define mSDKEventApplicationDidLaunch "applicationDidLaunch"
#define mSDKEventApplicationDidTerminate "applicationDidTerminate"
#define mSDKEventSystemDidWakeUp "systemDidWakeUp"
#define mSDKEventTitleParametersDidChange "titleParametersDidChange"
#define mSDKEventDidReceiveSettings "didReceiveSettings"
#define mSDKEventDidReceiveGlobalSettings "didReceiveGlobalSettings"
#define mSDKEventPropertyInspectorDidAppear "propertyInspectorDidAppear"
#define mSDKEventPropertyInspectorDidDisappear "propertyInspectorDidDisappear"
#define mSDKEventDialRotate "dialRotate"
#define mSDKEventDialDown "dialDown"
#define mSDKEventDialUp "dialUp"
#define mSDKEventTouchTap "touchTap"
#define mSDKEventTouchEvent "touchEvent"
#define mSDKEventDeleteAction "deleteAction"
#define mSDKEventTriggerKey "triggerKey"

//
// Functions
//
#define mSDKEventSetTitle "setTitle"
#define mSDKEventSetImage "setImage"
#define mSDKEventShowAlert "showAlert"
#define mSDKEventShowOK "showOk"
#define mSDKEventGetSettings "getSettings"
#define mSDKEventSetSettings "setSettings"
#define mSDKEventGetGlobalSettings "getGlobalSettings"
#define mSDKEventSetGlobalSettings "setGlobalSettings"
#define mSDKEventSetState "setState"
#define mSDKEventSwitchToProfile "switchToProfile"
#define mSDKEventSendToPropertyInspector "sendToPropertyInspector"
#define mSDKEventSendToPlugin "sendToPlugin"
#define mSDKEventOpenURL "openUrl"
#define mSDKEventLogMessage "logMessage"
#define mSDKEventSetFeedback "setFeedback"
#define mSDKEventSetText "setText"

//
// Payloads
//
#define mSDKPayloadSettings "settings"
#define mSDKPayloadCoordinates "coordinates"
#define mSDKPayloadState "state"
#define mSDKPayloadUserDesiredState "userDesiredState"
#define mSDKPayloadTitle "title"
#define mSDKPayloadTitleParameters "titleParameters"
#define mSDKPayloadImage "image"
#define mSDKPayloadURL "url"
#define mSDKPayloadTarget "target"
#define mSDKPayloadProfile "profile"
#define mSDKPayloadApplication "application"
#define mSDKPayloadIsInMultiAction "isInMultiAction"
#define mSDKPayloadMessage "message"

#define mSDKPayloadCoordinatesColumn "column"
#define mSDKPayloadCoordinatesRow "row"

//
// Device Info
//
#define mSDKDeviceInfoID "id"
#define mSDKDeviceInfoType "type"
#define mSDKDeviceInfoSize "size"
#define mSDKDeviceInfoName "name"

#define mSDKDeviceInfoSizeColumns "columns"
#define mSDKDeviceInfoSizeRows "rows"

//
// Title Parameters
//
#define mSDKTitleParametersShowTitle "showTitle"
#define mSDKTitleParametersTitleColor "titleColor"
#define mSDKTitleParametersTitleAlignment "titleAlignment"
#define mSDKTitleParametersFontFamily "fontFamily"
#define mSDKTitleParametersFontSize "fontSize"
#define mSDKTitleParametersCustomFontSize "customFontSize"
#define mSDKTitleParametersFontStyle "fontStyle"
#define mSDKTitleParametersFontUnderline "fontUnderline"

//
// Connection
//
#define mSDKConnectSocketFunction "connectElgatoStreamDeckSocket"
#define mSDKRegisterPlugin "registerPlugin"
#define mSDKRegisterPropertyInspector "registerPropertyInspector"
#define mSDKPortParameter "-port"
#define mSDKPluginUUIDParameter "-pluginUUID"
#define mSDKRegisterEventParameter "-registerEvent"
#define mSDKInfoParameter "-info"
#define mSDKRegisterUUID "uuid"

#define mSDKApplicationInfo "application"
#define mSDKPluginInfo "plugin"
#define mSDKDevicesInfo "devices"
#define mSDKColorsInfo "colors"
#define mSDKDevicePixelRatio "devicePixelRatio"

#define mSDKApplicationInfoSoftwareUUID "softwareUUID"
#define mSDKApplicationInfoVersion "version"
#define mSDKApplicationInfoLanguage "language"
#define mSDKApplicationInfoPlatform "platform"
#define mSDKApplicationInfoPlatformVersion "platformVersion"
#define mSDKApplicationInfoPlatformMac "mac"
#define mSDKApplicationInfoPlatformWindows "windows"

#define mSDKColorsInfoHighlightColor "highlightColor"
#define mSDKColorsInfoMouseDownColor "mouseDownColor"
#define mSDKColorsInfoDisabledColor "disabledColor"
#define mSDKColorsInfoButtonPressedTextColor "buttonPressedTextColor"
#define mSDKColorsInfoButtonPressedBackgroundColor "buttonPressedBackgroundColor"
#define mSDKColorsInfoButtonMouseOverBackgroundColor "buttonMouseOverBackgroundColor"
#define mSDKColorsInfoButtonPressedBorderColor "buttonPressedBorderColor"

typedef int mSDKTarget;
enum {
  mSDKTarget_HardwareAndSoftware = 0,
  mSDKTarget_HardwareOnly = 1,
  mSDKTarget_SoftwareOnly = 2
};

#endif // SDKDEFINES_H
