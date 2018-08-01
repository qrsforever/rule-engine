/***************************************************************************
 *  MonitorTool.cpp - Monitor tool impl only for debug
 *
 *  Created: 2018-07-18 16:30:15
 *
 *  Copyright QRS
 ****************************************************************************/

#include "MonitorTool.h"
#include "MainPublicHandler.h"
#include "MessageTypes.h"
#include "InstancePayload.h"
#include "RulePayload.h"
#include "RuleEventHandler.h"
#include "RuleEngineService.h"
#include "DeviceDataChannel.h"
#include "StringData.h"
#include "StringArray.h"
#include "Log.h"
#include "LogUDP.h"
#include "LogPool.h"
#include "Common.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

#ifndef SIM_SUITE
extern "C" HBDeviceManager& deviceManager();
#endif

using namespace UTILS;

namespace HB {

static MonitorTool *gMonitor = 0;
static LogUDP *gLogUDP = 0;

MonitorTool::MonitorTool()
    : mServerPort(8192)
    , mQuitFlag(false)
{

}

MonitorTool::~MonitorTool()
{
}

int MonitorTool::init(int port)
{/*{{{*/
    LOGD("MonitorTool port[%d]\n", port);
    mServerPort = port;
    mRefreshFds[0] = -1;
    mRefreshFds[1] = -1;
    return 0;
}/*}}}*/

void MonitorTool::finalize()
{
    mQuitFlag = true;
    int nouse = 1;
    if (mRefreshFds[1] > 0)
        write(mRefreshFds[1], &nouse, sizeof(nouse));
}

int MonitorTool::addClient(int sockfd)
{/*{{{*/
    LOGI("add Client: %d\n", sockfd);
    char ok[8] = "success";
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    send(sockfd, ok, 7, 0);
    mClientSockets.push_back(sockfd);
    /* for auto close socket after 300s */
    /* mainHandler().removeMessages(MT_MONITOR, MONITOR_CLOSE_CLIENT, sockfd);
     * mainHandler().sendMessageDelayed(
     *     mainHandler().obtainMessage(MT_MONITOR, MONITOR_CLOSE_CLIENT, sockfd), 300000);  */
    return 0;
}/*}}}*/

int MonitorTool::delClient(int sockfd)
{/*{{{*/
    LOGI("del Client: %d\n", sockfd);
    /* mainHandler().removeMessages(MT_MONITOR, MONITOR_CLOSE_CLIENT, sockfd); */
    for (size_t i = 0; i < mClientSockets.size(); ++i) {
        if (mClientSockets[i] == sockfd)
            mClientSockets.erase(mClientSockets.begin() + i);
    }
    close(sockfd);
    return 0;
}/*}}}*/

void MonitorTool::run()
{/*{{{*/
    LOGD("MonitorTool listen thread:[%u]\n", id());

    fd_set rset;
    socklen_t sock_size;
    struct sockaddr_in cli_addr;
    struct sockaddr_in svr_addr;
    int cli_sockfd = -1;
    int svr_sockfd = -1;
    int maxFd = -1;
    int retnum = 0;
    int opt = 1;

    memset(&svr_addr, 0, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(mServerPort);

	if ((svr_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        LOGE("socket error port[%d]\n", mServerPort);
		return;
    }
    setsockopt(svr_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(svr_sockfd, (struct sockaddr *)&svr_addr, sizeof(struct sockaddr)) < 0) {
        LOGE("bind error [%s]\n", strerror(errno));
        return;
    }
    fcntl(svr_sockfd, F_SETFL, O_NONBLOCK);

    registCommands();

    pipe(mRefreshFds);
    while (!mQuitFlag) {
        FD_ZERO(&rset);
        FD_SET(mRefreshFds[0], &rset);
        FD_SET(svr_sockfd, &rset);
        maxFd = svr_sockfd > mRefreshFds[0] ? svr_sockfd : mRefreshFds[0];
        for (size_t i = 0; i < mClientSockets.size(); ++i) {
            if (mClientSockets[i] > maxFd)
                maxFd = mClientSockets[i];
            FD_SET(mClientSockets[i], &rset);
        }
        LOGI("maxFd = %d cliens[%d]\n", maxFd, mClientSockets.size());
        retnum = select(maxFd + 1, &rset, 0, 0, 0);
        if (-1 == retnum) {
            if (EINTR == errno)
                continue;
            return;
        }
        if (FD_ISSET(mRefreshFds[0], &rset)) {
            read(mRefreshFds[0], &opt, sizeof(opt));
            continue;
        }
        if (FD_ISSET(svr_sockfd, &rset)) {
            if (listen(svr_sockfd, 5) < 0) {
                LOGE("listen error[%s]!\n", strerror(errno));
                continue;
            }
            cli_sockfd = accept(svr_sockfd, (struct sockaddr *)&cli_addr, &sock_size);
            if (cli_sockfd > 0)
                addClient(cli_sockfd);
        } else {
            for (size_t i = 0; i < mClientSockets.size(); ++i) {
                if (FD_ISSET(mClientSockets[i], &rset))
                    if (!doRequest(mClientSockets[i]))
                        send(mClientSockets[i], "-1", 2, 0);
            }
        }
    }
    for (size_t i = 0; i < mClientSockets.size(); ++i) {
        send(mClientSockets[i], "-2", 2, 0);
        delClient(mClientSockets[i]);
    }
    close(svr_sockfd);
    LOGD("Monitor Thread Quit.\n");
}/*}}}*/

void MonitorTool::registCommands()
{/*{{{*/
    _Insert("setModuleLogLevel",    &MonitorTool::_SetModuleLogLevel);
    _Insert("getModuleLogLevel",    &MonitorTool::_GetModuleLogLevel);
    _Insert("getModulesName",       &MonitorTool::_GetModulesName);
    _Insert("startUDPLog",          &MonitorTool::_StartUDPLog);
    _Insert("stopUDPLog",           &MonitorTool::_StopUDPLog);
    _Insert("getDevices",           &MonitorTool::_GetDevices);
    _Insert("getSlots",             &MonitorTool::_GetSlots);
    _Insert("getInstaces",          &MonitorTool::_GetInstaces);
    _Insert("getInstanceValue",     &MonitorTool::_GetInstanceValue);
    _Insert("getDeviceValue",       &MonitorTool::_GetDeviceValue);
    _Insert("updateInstanceValue",  &MonitorTool::_UpdateInstanceValue);
    _Insert("updateDeviceValue",    &MonitorTool::_UpdateDeviceValue);
    _Insert("getRules",             &MonitorTool::_GetRules);
    _Insert("switchRule",           &MonitorTool::_SwitchRule);
    _Insert("getRuleScript",        &MonitorTool::_GetRuleScript);
    _Insert("getRuleSwitch",        &MonitorTool::_GetRuleSwitch);
    _Insert("commitRuleScript",     &MonitorTool::_CommitRuleScript);
    _Insert("deleteRuleScript",     &MonitorTool::_DeleteRuleScript);
    _Insert("assertFact",           &MonitorTool::_AssertFact);
    _Insert("printItem",            &MonitorTool::_PrintItem);
    _Insert("watchItem",            &MonitorTool::_WatchItem);

}/*}}}*/

std::string MonitorTool::_SetModuleLogLevel(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 2) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    int l = atoi(ps[1].c_str());
    if (ps[0] == "rule-engine") {
        ruleHandler().sendMessage(ruleHandler().obtainMessage(
                RET_LOG_LEVEL, l, 0));
    }
    return int2String(setModuleLogLevel(ps[0].c_str(), l));
}/*}}}*/

std::string MonitorTool::_GetModuleLogLevel(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    return int2String(getModuleLogLevel(ps[0].c_str()));
}/*}}}*/

std::string MonitorTool::_GetModulesName(const char *params)
{/*{{{*/
    (void)params;
    char names[128];
    return getModuleLogNames(names, 127);
}/*}}}*/

std::string MonitorTool::_StartUDPLog(const char *params)
{/*{{{*/
    if (gLogUDP)
        return "-1";
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 2) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    LOGD("startUDPLog(%s, %d)\n", ps[0].c_str(), atoi(ps[1].c_str()));
    gLogUDP = new LogUDP(ps[0].c_str(), atoi(ps[1].c_str()));
    LogPool::getInstance().attachFilter(gLogUDP);
    return "0";
}/*}}}*/

std::string MonitorTool::_StopUDPLog(const char *params)
{/*{{{*/
    (void)params;
    if (gLogUDP) {
        LogPool::getInstance().detachFilter(gLogUDP);
        delete (gLogUDP);
        gLogUDP = 0;
    }
    return "0";
}/*}}}*/

std::string MonitorTool::_GetDevices(const char *params)
{/*{{{*/
    (void)params;
    bool mulflg = false;
    std::string res;
    std::vector<std::string> items = ruleEngine().getDevices();
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] == "DEVICE")
            continue;
        if (mulflg)
            res.append(";");
        else
            mulflg = true;
        res.append(items[i]);
    }
    return res;
}/*}}}*/

std::string MonitorTool::_GetSlots(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    bool mulflg = false;
    std::string res;
    std::vector<std::string> items = ruleEngine().getSlots(ps[0]);
    for (size_t i = 0; i < items.size(); ++i) {
        if (mulflg)
            res.append(";");
        else
            mulflg = true;
        res.append(items[i]);
    }
    return res;
}/*}}}*/

std::string MonitorTool::_GetInstaces(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    bool mulflg = false;
    std::string res;
    std::vector<std::string> items = ruleEngine().getInstaces(ps[0]);
    for (size_t i = 0; i < items.size(); ++i) {
        if (mulflg)
            res.append(";");
        else
            mulflg = true;
        res.append(outerOfInsname(items[i]));
    }
    return res;
}/*}}}*/

std::string MonitorTool::_GetInstanceValue(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 2) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    return ruleEngine().getInstanceValue(innerOfInsname(ps[0]), ps[1]);
}/*}}}*/

std::string MonitorTool::_GetDeviceValue(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 2) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    std::string propertyValue;
    deviceManager().GetDevicePropertyValue(ps[0], ps[1], propertyValue, false);
    return propertyValue;
}/*}}}*/

std::string MonitorTool::_UpdateInstanceValue(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 3) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    std::shared_ptr<InstancePayload> payload = std::make_shared<InstancePayload>();
    if (payload) {
        payload->mInsName = innerOfInsname(ps[0]);
        payload->mSlots.push_back(InstancePayload::SlotInfo(ps[1], ps[2]));
        ruleHandler().sendMessage(ruleHandler().obtainMessage(RET_INSTANCE_PUT, payload));
    }
    return "0";
}/*}}}*/

std::string MonitorTool::_UpdateDeviceValue(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 3) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    deviceManager().SetDevicePropertyValue(ps[0], ps[1], ps[2], false);
    return "0";
}/*}}}*/

std::string MonitorTool::_GetRules(const char *params)
{/*{{{*/
    bool mulflg = false;
    std::string res;
    std::vector<std::string> items = ruleEngine().getRules();
    for (size_t i = 0; i < items.size(); ++i) {
        if (mulflg)
            res.append(";");
        else
            mulflg = true;
        res.append(items[i]);
    }
    return res;
}/*}}}*/

std::string MonitorTool::_SwitchRule(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 2) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    if (ps[1] == "true")
        ruleEngine().enableRule(ps[0]);
    else
        ruleEngine().disableRule(ps[0]);
    return "0";
}/*}}}*/

std::string MonitorTool::_GetRuleScript(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    std::string dir = ruleEngine().getServerRoot() + "/" + RULES_SEARCH_DIR;
    std::stringstream doc;
    std::ifstream in;
    in.open(dir + ps[0] + ".clp", std::ifstream::in);
    if (in.is_open()) {
        doc << in.rdbuf();
        in.close();
        return doc.str();
    }
    return "-1";
}/*}}}*/

std::string MonitorTool::_GetRuleSwitch(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    if (ruleEngine().getRuleSwitch(ps[0]))
        return "1";
    return "0";
}/*}}}*/

std::string MonitorTool::_CommitRuleScript(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 2) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    std::shared_ptr<StringArray> ruledata = std::make_shared<StringArray>();
    if (ruledata) {
        ruledata->put(0, ps[0].c_str());
        ruledata->put(1, "0.0.1");
        ruledata->put(2, ps[1].c_str());
        ruleHandler().sendMessage(ruleHandler().obtainMessage(RET_RULE_ADD, ruledata));
        return "0";
    }
    return "-1";
}/*}}}*/

std::string MonitorTool::_DeleteRuleScript(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    std::shared_ptr<StringData> ruleId = std::make_shared<StringData>(ps[0].c_str());
    if (ruleId)
        ruleHandler().sendMessage(ruleHandler().obtainMessage(RET_RULE_DELETE, ruleId));
    return "0";
}/*}}}*/

std::string MonitorTool::_AssertFact(const char *params)
{/*{{{*/
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    std::shared_ptr<StringData> fact = std::make_shared<StringData>(ps[0].c_str());
    if (fact)
        ruleHandler().sendMessage(ruleHandler().obtainMessage(RET_ASSERT_FACT, fact));
    return "0";
}/*}}}*/

std::string MonitorTool::_PrintItem(const char *params)
{
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 1) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    int val = DEBUG_SHOW_ALL;
    if (ps[0] == "mem")
        val = DEBUG_SHOW_MEMORY;
    else if (ps[0] == "fact")
        val = DEBUG_SHOW_FACTS;
    else if (ps[0] == "instance")
        val = DEBUG_SHOW_INSTANCES;
    else if (ps[0] == "class")
        val = DEBUG_SHOW_CLASSES;
    else if (ps[0] == "rule")
        val = DEBUG_SHOW_RULES;
    else if (ps[0] == "agenda")
        val = DEBUG_SHOW_AGENDA;
    else if (ps[0] == "global")
        val = DEBUG_SHOW_GLOBALS;
    ruleEngine().debug(val);
    return "0";
}

std::string MonitorTool::_WatchItem(const char *params)
{
    std::vector<std::string> ps = stringSplit(params, ";");
    if (ps.size() != 2) {
        LOGE("params[%d] format error!\n", ps.size());
        return "-1";
    }
    std::shared_ptr<StringData> item = std::make_shared<StringData>(ps[1].c_str());
    if (item) {
        ruleHandler().sendMessage(ruleHandler().obtainMessage(
                RET_WATCH_ITEM, atoi(ps[0].c_str()), 0, item));
    }
    return "0";
}

bool MonitorTool::doRequest(int sockfd)
{/*{{{*/
    char buff[4096] = { 0 };
    if (recv(sockfd, buff, 4095, 0) <= 0) {
        LOGE("recv error[%s]!\n", strerror(errno));
        delClient(sockfd);
        return true;
    }
    std::string res("-1");
    char *cmd = strtok(buff, ";");
    if (!cmd) {
        LOGE("command[%s] format error!\n", buff);
        return false;
    }
    CommandIter_t it = mCommands.find(cmd);
    if (it == mCommands.end()) {
        if (!strncmp(cmd, "quit", 4)) {
            delClient(sockfd);
            return true;
        } else if (!strncmp(cmd, "getHomeBrainVersion", 19)
            || !strncmp(cmd, "getRuleEngineVersion", 20)) {
            res = "1.0.0";
        } else {
            LOGW("Unkown command[%s]\n", cmd);
            return -1;
        }
    } else
        res = it->second(buff + strlen(cmd) + 1);

    LOGD("command[%s]: [%s]\n", buff, res.c_str());
    if (!res.empty())
        return send(sockfd, res.c_str(), res.size(), 0);
    return false;
}/*}}}*/

MonitorTool& monitor()
{
    if (0 == gMonitor)
         gMonitor = new MonitorTool();
    return *gMonitor;
}

} /* namespace HB */
