/***************************************************************************
 *  TempSimulateSuite.h - Only Test
 *
 *  Created: 2018-06-15 19:52:12
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __TempSimulateSuite_H__
#define __TempSimulateSuite_H__

#include <string>
#include <functional>
#include "Message.h"

#ifdef __cplusplus

using namespace UTILS;

namespace HB {

/* from device-manage */
typedef enum {
    HB_DEVICE_STATUS_UNINITIALIZED = 0,
    HB_DEVICE_STATUS_INITIALIZING,
    HB_DEVICE_STATUS_ONLINE,
    HB_DEVICE_STATUS_OFFLINE,
} HBDeviceStatus;

class HBDeviceCallBackHandler {
public:
    virtual ~HBDeviceCallBackHandler() {}

    virtual void onDeviceStatusChanged(const std::string deviceId, const std::string deviceType, HBDeviceStatus status) = 0;
    virtual void onDevicePropertyChanged(const std::string deviceId, const std::string propertyKey, std::string value) = 0;
};

class HBDeviceManager {
public:
    int SetDevicePropertyValue(const std::string deviceId, const std::string propertyKey, const std::string value, bool async = false);
    int GetDevicePropertyValue(const std::string deviceId, const std::string propertyKey, std::string& value, bool async = false);

    int Init();
    int SetCallback(HBDeviceCallBackHandler* callback) { mCallback = callback; return 0; }

    HBDeviceCallBackHandler& cb() { return *mCallback; }
    HBDeviceCallBackHandler *mCallback;
};

HBDeviceManager& deviceManager();

class HBCloudManager {
public:
    typedef std::function<void(std::string)> SyncRuleProfileCallback;
    typedef std::function<void(std::string, std::string)> SyncDeviceProfileCallback;
    void registSyncDeviceProfileCallback(SyncDeviceProfileCallback cb) {mDeviceProfileCB = cb;}
    void registSyncRuleProfileCallback(SyncRuleProfileCallback cb) {mRuleProfileCB = cb;}
    SyncDeviceProfileCallback mDeviceProfileCB;
    SyncRuleProfileCallback mRuleProfileCB;
};

HBCloudManager& cloudManager();

std::string getClassNameByDeviceId(const std::string &deviceId);

void tempSimulateTest(Message *msg);

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __TempSimulateSuite_H__ */
