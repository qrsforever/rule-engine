/***************************************************************************
 *  DefClassTable.h - Def Class Table Header
 *
 *  Created: 2018-06-26 14:06:57
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __DefClassTable_H__
#define __DefClassTable_H__

#include "DefTable.h"

#ifdef __cplusplus

namespace HB {

class DefClassTable : public DefTable {
public:
    DefClassTable(SQLiteDatabase &db);
    ~DefClassTable();

    TableType type() { return TT_DEFCLASS; }

private:

}; /* class DefClassTable */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __DefClassTable_H__ */
