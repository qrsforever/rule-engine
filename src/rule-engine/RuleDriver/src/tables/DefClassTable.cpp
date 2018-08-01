/***************************************************************************
 *  DefClassTable.cpp - Define Class Table Impl
 *
 *  Created: 2018-06-26 14:09:21
 *
 *  Copyright QRS
 ****************************************************************************/

#include "DefClassTable.h"
#include "SQLiteLog.h"

#define DEFCLASS_TABLE_NAME     "defclass"

namespace HB {

DefClassTable::DefClassTable(SQLiteDatabase &db)
    : DefTable(db, DEFCLASS_TABLE_NAME)
{
    init();
}

DefClassTable::~DefClassTable()
{

}

} /* namespace HB */
