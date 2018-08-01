/***************************************************************************
 *  DefTable.h - Define Table Base Class Header
 *
 *  Created: 2018-06-26 12:29:15
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __DefTable_H__
#define __DefTable_H__

#include "DBTable.h"

namespace HB {

#define DEF_FIELD_DEFNAME  "DefName"
#define DEF_FIELD_VERSION  "Version"
#define DEF_FIELD_CLPPATH  "FilePath"
#define DEF_FIELD_RAWDATA  "RawData"

struct DefInfo {
    std::string mDefName;
    std::string mVersion;
    std::string mFilePath;
    std::string mRawData;
}; /* struct DefInfo */

class DefTable : public DBTable {
public:
    virtual ~DefTable();

    bool deleteByKey(const std::string &defName);
    bool updateOrInsert(const DefInfo &info);
    std::vector<DefInfo> getDefInfos();
    virtual std::string getVersion(std::string defName);
    virtual std::string getFilePath(std::string defName);
    virtual std::vector<std::string> getFilePaths();

#ifdef TABLE_DEBUG
    void showTable();
#endif

protected:
    DefTable(SQLiteDatabase &db, const char *tabName);

}; /* class DefTable */

} /* namespace HB */

#ifdef __cplusplus


#endif /* __cplusplus */

#endif /* __DefTable_H__ */
