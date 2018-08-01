/***************************************************************************
 *  RuleEngineLog.h - Rule Engine Log
 *
 *  Created: 2018-07-18 15:36:53
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RuleEngineLog_H__
#define __RuleEngineLog_H__

#include "Log.h"
#include "Router.h"

#ifdef __cplusplus

extern int gRuleEngineModuleLevel;
extern int gRuleScriptModuleLevel;

#define RE_LOGE(args...)  _LOGE(gRuleEngineModuleLevel, args)
#define RE_LOGW(args...)  _LOGW(gRuleEngineModuleLevel, args)
#define RE_LOGD(args...)  _LOGD(gRuleEngineModuleLevel, args)
#define RE_LOGI(args...)  _LOGI(gRuleEngineModuleLevel, args)
#define RE_LOGT(args...)  _LOGT(gRuleEngineModuleLevel, args)
#define RE_LOGTT()        _LOGT(gRuleEngineModuleLevel, "run here!\n")

#define RS_LOGE(args...)  _LOGE(gRuleScriptModuleLevel, args)
#define RS_LOGW(args...)  _LOGW(gRuleScriptModuleLevel, args)
#define RS_LOGD(args...)  _LOGD(gRuleScriptModuleLevel, args)
#define RS_LOGI(args...)  _LOGI(gRuleScriptModuleLevel, args)
#define RS_LOGT(args...)  _LOGT(gRuleScriptModuleLevel, args)

namespace HB {

class RuleEventHandler;
class CLIPSLogger : public CLIPS::ILogger {
public:
    CLIPSLogger(RuleEventHandler &handler) : mH(handler) {}
    ~CLIPSLogger(){}

    void clips_loge(const char *str);
    void clips_logw(const char *str) { RS_LOGW("%s", str); }
    void clips_logd(const char *str) { RS_LOGD("%s", str); }
    void clips_logi(const char *str) { RS_LOGI("%s", str); }
    void clips_logt(const char *str) { RS_LOGT("%s", str); }

private:
    RuleEventHandler &mH;

}; /* class RuleEngineLogger */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __RuleEngineLog_H__ */
