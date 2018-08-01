/***************************************************************************
 *  DefTemplateTable.cpp - Define Template Table Impl
 *
 *  Created: 2018-06-26 15:53:14
 *
 *  Copyright QRS
 ****************************************************************************/

#include "DefTemplateTable.h"
#include "SQLiteLog.h"

#define DEFTEMPLATE_TABLE_NAME     "deftemplate"

namespace HB {

DefTemplateTable::DefTemplateTable(SQLiteDatabase &db)
    : DefTable(db, DEFTEMPLATE_TABLE_NAME)
{
    init();
}

DefTemplateTable::~DefTemplateTable()
{

}

} /* namespace HB */
