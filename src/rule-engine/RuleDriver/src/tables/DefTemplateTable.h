/***************************************************************************
 *  DefTemplateTable.h - Define Template Table Header
 *
 *  Created: 2018-06-26 15:49:04
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __DefTemplateTable_H__
#define __DefTemplateTable_H__

#include "DefTable.h"

#ifdef __cplusplus

namespace HB {

class DefTemplateTable : public DefTable {
public:
    DefTemplateTable(SQLiteDatabase &db);
    ~DefTemplateTable();

    TableType type() { return TT_DEFTEMPLATE; }

private:

}; /* class DefTemplateTable */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __DefTemplateTable_H__ */
