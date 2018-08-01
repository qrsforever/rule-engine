/***************************************************************************
 *  RuleEngineStore.cpp - Rule Engine Store Impl
 *
 *  Created: 2018-06-26 11:07:12
 *
 *  Copyright QRS
 ****************************************************************************/

#include "RuleEngineStore.h"
#include "RuleEventHandler.h"

#include "RuleEngineLog.h"

#define DELAY_TIME (5*1000)

namespace HB {

RuleEngineStore::RuleEngineStore(const std::string &db)
    : mHandler(ruleHandler())
    , mDBFilePath(db), mDB(0), mDefTmplTab(0)
    , mDefClassTab(0), mDefRuleTab(0), mTimerEventTab(0)
{
    RE_LOGTT();
}

RuleEngineStore::~RuleEngineStore()
{
    RE_LOGTT();
    close();
}

bool RuleEngineStore::open()
{
    RE_LOGTT();
    mHandler.removeMessages(RET_STORE_CLOSE);
    mHandler.sendEmptyMessageDelayed(RET_STORE_CLOSE, DELAY_TIME);
    /* mHandler.sendMessageDelayed(
     *     mHandler.obtainMessage(RET_STORE_CLOSE, shared_from_this()), DELAY_TIME); */
    if (isOpen())
        return true;
    mDB = new SQLiteDatabase();
    if (!mDB->open(mDBFilePath.c_str()))
        return false;
    return true;
}

bool RuleEngineStore::close()
{
    RE_LOGTT();
    Mutex::Autolock _l(&mDBMutex);
    if (mDefTmplTab)
        delete mDefTmplTab;
    mDefTmplTab = 0;
    if (mDefClassTab)
        delete mDefClassTab;
    mDefClassTab = 0;
    if (mDefRuleTab)
        delete mDefRuleTab;
    mDefRuleTab = 0;
    if (mDB)
        mDB->close();
    mDB = 0;
    return true;
}

/*{{{ public def table */
bool RuleEngineStore::_UpdateDefTable(DefTable *table, const std::string &defName, const std::string &version, const std::string &fileName)
{
    RE_LOGTT();
    Mutex::Autolock _l(&mDBMutex);
    DefInfo info;
    info.mDefName = defName;
    info.mVersion = version;
    info.mFilePath = fileName;
    if (!table->updateOrInsert(info))
        return false;
    return true;
}

std::vector<std::string> RuleEngineStore::_QueryDefFilePaths(DefTable *table, bool urgent)
{
    if (urgent) {
        /* not used yet */
        return std::vector<std::string>();
    }

    RE_LOGTT();
    Mutex::Autolock _l(&mDBMutex);
    return std::move(table->getFilePaths());
}

bool RuleEngineStore::updateTemplateTable(const std::string &tmplName, const std::string &version, const std::string &fileName)
{
    if (!open())
        return false;
    return _UpdateDefTable(&templateTable(), tmplName, version, fileName);
}

bool RuleEngineStore::updateClassTable(const std::string &clsName, const std::string &version, const std::string &fileName)
{
    if (!open())
        return false;
    return _UpdateDefTable(&classTable(), clsName, version, fileName);
}

bool RuleEngineStore::updateRuleTable(const std::string &ruleName, const std::string &version, const std::string &fileName)
{
    if (!open())
        return false;
    return _UpdateDefTable(&ruleTable(), ruleName, version, fileName);
}

std::vector<std::string> RuleEngineStore::queryTemplateFilePaths(bool urgent)
{
    if (!open())
        return std::vector<std::string>();
    return std::move(_QueryDefFilePaths(&templateTable(), urgent));
}

std::vector<std::string> RuleEngineStore::queryClassFilePaths(bool urgent)
{
    if (!open())
        return std::vector<std::string>();
    return std::move(_QueryDefFilePaths(&classTable(), urgent));
}

std::vector<std::string> RuleEngineStore::queryRuleFilePaths(bool urgent)
{
    if (!open())
        return std::vector<std::string>();
    return std::move(_QueryDefFilePaths(&ruleTable(), urgent));
}
/*}}}*/

/*{{{ rule table */
bool RuleEngineStore::updateRuleForEnable(const std::string &ruleId, bool value)
{
    if (!open())
        return false;
    return ruleTable().updateForEnable(ruleId, value);
}

bool RuleEngineStore::updateRuleForTimers(const std::string &ruleId, const std::string &value)
{
    if (!open() || value.empty())
        return false;
    return ruleTable().updateForTimers(ruleId, value);
}

std::vector<DefRuleInfo> RuleEngineStore::queryRuleInfos()
{
    if (!open())
        return std::vector<DefRuleInfo>();
    return std::move(ruleTable().getDefRuleInfos());
}

std::string RuleEngineStore::queryRuleTimers(const std::string &ruleId)
{
    if (!open())
        return std::string();
    return std::move(ruleTable().getTimersByKey(ruleId));
}

bool RuleEngineStore::deleteRule(const std::string &ruleId)
{
    if (!open())
        return false;
    return ruleTable().deleteByKey(ruleId);
}
/*}}}*/

/*{{{ timer event table */
bool RuleEngineStore::updateTimerEvent(const TimerEventInfo &info)
{
    if (!open())
        return false;
    return timerTable().updateOrInsert(info);
}

std::vector<TimerEventInfo> RuleEngineStore::queryTimerEventInfos()
{
    if (!open())
        return std::vector<TimerEventInfo>();
    return std::move(timerTable().getTimerEventInfos());
}

bool RuleEngineStore::queryTimerEvent(int eID, TimerEventInfo &info)
{
    if (!open())
        return false;
    return timerTable().getTimerEventInfoByKey(eID, info);
}
/*}}}*/
} /* namespace HB */
