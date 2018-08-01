/***************************************************************************
 *  DefRuleTable.h - Define Rule Table Header
 *
 *  Created: 2018-06-26 15:51:56
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __DefRuleTable_H__
#define __DefRuleTable_H__

#include "DefTable.h"

#ifdef __cplusplus

namespace HB {

struct DefRuleInfo : public DefInfo {
    int mEnable;
    std::string mTimers;
}; /* DefRuleInfo */

class DefRuleTable : public DefTable {
public:
    DefRuleTable(SQLiteDatabase &db);
    ~DefRuleTable();

    TableType type() { return TT_DEFRULE; }

    bool updateOrInsert(const DefRuleInfo &info);
    std::vector<DefRuleInfo> getDefRuleInfos();

    std::vector<std::string> getFilePaths();
    bool updateForEnable(const std::string &ruleId, bool value);
    bool updateForTimers(const std::string &ruleId, const std::string &value);
    std::string getTimersByKey(const std::string &ruleId);

private:

}; /* class DefRuleTable */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __DefRuleTable_H__ */
