/***************************************************************************
 *  MonitorTool.h - Monitor tool impl
 *
 *  Created: 2018-07-18 16:29:53
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __MonitorTool_H__
#define __MonitorTool_H__

#include "Thread.h"

#include <vector>
#include <string>
#include <map>
#include <functional>

#ifdef __cplusplus

namespace HB {

class MonitorTool : public ::UTILS::Thread {
public:
    typedef std::function<std::string(const char *prams)> CommandCall_t;
    typedef std::map<std::string, CommandCall_t>::iterator CommandIter_t;
    MonitorTool();
    ~MonitorTool();

    int init(int port = 8192);
    void registCommands();
    void run();
    void finalize();

    bool doRequest(int sockfd);

    int addClient(int sockfd);
    int delClient(int sockfd);

private:
    void _Insert(const std::string &name, CommandCall_t cmd);

    /* Commands */
    static std::string _SetModuleLogLevel(const char *params);
    static std::string _GetModuleLogLevel(const char *params);
    static std::string _GetModulesName(const char *params);
    static std::string _StartUDPLog(const char *params);
    static std::string _StopUDPLog(const char *params);
    static std::string _GetDevices(const char *params);
    static std::string _GetSlots(const char *params);
    static std::string _GetInstaces(const char *params);
    static std::string _GetInstanceValue(const char *params);
    static std::string _GetDeviceValue(const char *params);
    static std::string _UpdateInstanceValue(const char *params);
    static std::string _UpdateDeviceValue(const char *params);
    static std::string _GetRules(const char *params);
    static std::string _SwitchRule(const char *params);
    static std::string _GetRuleScript(const char *params);
    static std::string _GetRuleSwitch(const char *params);
    static std::string _CommitRuleScript(const char *params);
    static std::string _DeleteRuleScript(const char *params);
    static std::string _AssertFact(const char *params);
    static std::string _PrintItem(const char *params);
    static std::string _WatchItem(const char *params);

private:
    int mServerPort;
    int mRefreshFds[2];
    bool mQuitFlag;
    std::vector<int> mClientSockets;
    std::map<std::string, CommandCall_t> mCommands;
}; /* class MonitorTool */

inline void MonitorTool::_Insert(const std::string &name, CommandCall_t cmd)
{
    mCommands.insert(std::pair<std::string, CommandCall_t>(name, cmd));
}

MonitorTool& monitor();

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __MonitorTool_H__ */
