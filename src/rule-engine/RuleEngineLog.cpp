/***************************************************************************
 *  RuleEngineLog.cpp - Rule Engine Log impl
 *
 *  Created: 2018-07-18 15:39:46
 *
 *  Copyright QRS
 ****************************************************************************/

#include "RuleEngineLog.h"
#include "RuleEventHandler.h"
#include "StringData.h"

int gRuleEngineModuleLevel = LOG_LEVEL_WARNING;
int gRuleScriptModuleLevel = LOG_LEVEL_WARNING;

using namespace UTILS;

namespace HB {

static LogModule RuleEngineModule("rule-engine", gRuleEngineModuleLevel);
static LogModule RuleScriptModule("rule-script", gRuleScriptModuleLevel);

void CLIPSLogger::clips_loge(const char *str)
{
    RS_LOGE("%s", str);
    std::shared_ptr<StringData> data = std::make_shared<StringData>(str);
    mH.sendMessageAtFrontOfQueue(mH.obtainMessage(RET_LOG_ERROR, data));
}

} /* namespace HB */
