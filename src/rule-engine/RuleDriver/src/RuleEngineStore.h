/***************************************************************************
 *  RuleEngineStore.h - Rule Engine Store Header
 *
 *  Created: 2018-06-26 11:06:44
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RuleEngineStore_H__
#define __RuleEngineStore_H__

#include "SQLiteDatabase.h"
#include "DefTemplateTable.h"
#include "DefClassTable.h"
#include "DefRuleTable.h"
#include "TimerEventTable.h"
#include "Object.h"
#include "Mutex.h"

#include <string>
#include <memory>

namespace HB {

class RuleEventHandler;

class RuleEngineStore : public ::UTILS::Object {
                        /* , public std::enable_shared_from_this<RuleEngineStore> { */
public:
    typedef std::shared_ptr<RuleEngineStore> pointer;
    RuleEngineStore(const std::string &db);
    ~RuleEngineStore();

    bool isOpen() { return mDB != 0; }
    bool open();
    bool close();

    bool updateTemplateTable(const std::string &tmplName, const std::string &version, const std::string &fileName);
    bool updateClassTable(const std::string &clsName, const std::string &version, const std::string &fileName);
    bool updateRuleTable(const std::string &ruleId, const std::string &version, const std::string &fileName);

    std::vector<std::string> queryTemplateFilePaths(bool urgent = false);
    std::vector<std::string> queryClassFilePaths(bool urgent = false);
    std::vector<std::string> queryRuleFilePaths(bool urgent = false);

    /* rule */
    bool updateRuleForEnable(const std::string &ruleId, bool value);
    bool updateRuleForTimers(const std::string &ruleId, const std::string &value);
    std::string queryRuleTimers(const std::string &ruleId);
    std::vector<DefRuleInfo> queryRuleInfos();
    bool deleteRule(const std::string &ruleId);

    /* timer event */
    bool updateTimerEvent(const TimerEventInfo &info);
    bool queryTimerEvent(int eID, TimerEventInfo &info);
    std::vector<TimerEventInfo> queryTimerEventInfos();

    inline DefTemplateTable& templateTable();
    inline DefClassTable& classTable();
    inline DefRuleTable& ruleTable();
    inline TimerEventTable& timerTable();

private:
    bool _UpdateDefTable(DefTable *table, const std::string &defName, const std::string &version, const std::string &fileName);
    std::vector<std::string> _QueryDefFilePaths(DefTable *table, bool urgent);

private:
    RuleEventHandler &mHandler;
    std::string mDBFilePath;
    UTILS::Mutex mDBMutex;
    UTILS::SQLiteDatabase *mDB;
    DefTemplateTable *mDefTmplTab;
    DefClassTable *mDefClassTab;
    DefRuleTable *mDefRuleTab;
    TimerEventTable *mTimerEventTab;

}; /* class RuleEngineStore */

inline DefTemplateTable& RuleEngineStore::templateTable()
{
    if (!mDefTmplTab)
        mDefTmplTab = new DefTemplateTable(*mDB);
    return *mDefTmplTab;
}

inline DefClassTable& RuleEngineStore::classTable()
{
    if (!mDefClassTab)
        mDefClassTab = new DefClassTable(*mDB);
    return *mDefClassTab;
}

inline DefRuleTable& RuleEngineStore::ruleTable()
{
    if (!mDefRuleTab)
        mDefRuleTab = new DefRuleTable(*mDB);
    return *mDefRuleTab;
}

inline TimerEventTable& RuleEngineStore::timerTable()
{
    if (!mTimerEventTab)
        mTimerEventTab = new TimerEventTable(*mDB);
    return *mTimerEventTab;
}

} /* namespace HB */

#ifdef __cplusplus


#endif /* __cplusplus */

#endif /* __RuleEngineStore_H__ */
