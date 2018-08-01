/***************************************************************************
 *  RuleDataChannel.cpp - Rule Data Channel
 *
 *  Created: 2018-06-14 15:08:33
 *
 *  Copyright QRS
 ****************************************************************************/

#include "RuleDataChannel.h"
#include "RuleEventHandler.h"
#include "RuleEventTypes.h"
#include "InstancePayload.h"
#include "Common.h"
#include "Log.h"
#include <stdlib.h>

/* #define USE_TIMER_EVENT 1 */

#ifndef SIM_SUITE
std::string getClassNameByDeviceId(const std::string &deviceId)
{
    return std::string("");
}
#endif

namespace HB {

RuleDataChannel::RuleDataChannel()
    : mH(ruleHandler())
{
}

RuleDataChannel::~RuleDataChannel()
{
}

int RuleDataChannel::init()
{
    return 0;
}

bool RuleDataChannel::send(int action, std::shared_ptr<Payload> payload)
{
    (void)action;
    (void)payload;
    return false;
}

ElinkRuleDataChannel::ElinkRuleDataChannel()
{

}

ElinkRuleDataChannel::~ElinkRuleDataChannel()
{

}

int ElinkRuleDataChannel::init()
{
    LOGTT();
#ifdef SIM_SUITE
    /* regist rule sync json doc from cloud */
    cloudManager().registSyncRuleProfileCallback(
        std::bind(
            &ElinkRuleDataChannel::onSyncRuleProfile,
            this,
            std::placeholders::_1)
        );
#endif
    return 0;
}


bool ElinkRuleDataChannel::_ParseTrigger(rapidjson::Value &trigger, std::shared_ptr<RulePayload> payload)
{
    rapidjson::Value &triggerType = trigger["triggerType"];
    if (!triggerType.IsString()) {
        LOGE("parse triggerType error!\n");
        return false;
    }
    if (!strncmp(triggerType.GetString(), "auto", 3))
        payload->mAuto = true;
    else
        payload->mAuto = false;

    rapidjson::Value &nswitch = trigger["switch"];
    if (!nswitch.IsObject()) {
        LOGE("parse switch error!\n");
        return false;
    }
    rapidjson::Value &enabled = nswitch["enabled"];
    if (!enabled.IsString()) {
        LOGE("parse enabled error!\n");
        return false;
    }
    if (!strncmp(enabled.GetString(), "off", 3))
        payload->mEnable = false;
    else
        payload->mEnable = true;
    return true;
}

bool ElinkRuleDataChannel::_ParseTimeString(const char *ctimestr, SlotPoint &slotpoint)
{
    char *timestr = (char*)ctimestr;
    char *t_or = strtok(timestr,"|");
    if (t_or) {
        slotpoint.mCellLogic = "|";
        do {
            slotpoint.append("=", t_or);
            t_or = strtok(NULL, "|");
        } while (t_or);
        return true;
    }

    char *t_range = strtok(timestr, "-");
    if (t_range) {
        slotpoint.mCellLogic = "&";
        slotpoint.append(">=", t_range);
        t_range = strtok(NULL, "-");
        if (!t_range) {
            LOGE("parse conditions.timeCondition.x error!\n");
            return false;
        }
        slotpoint.append("<=", t_range);
        return true;
    }

    if (!strncmp(timestr, "every", 5))
        return true;

    char *t_not = strtok(timestr, "!");
    if (t_not) {
        slotpoint.mCellLogic = "|";
        do {
            slotpoint.append("<>", t_not);
            t_not = strtok(NULL, "!");
        } while (t_not);
        return true;
    }

    /* TODO */
    slotpoint.append("=", ctimestr);
    return true;
}

bool ElinkRuleDataChannel::_ParseTimeString(const char *ctimestr, TimeNode &node)
{
    char *timestr = (char*)ctimestr;
    char *t_set = strtok(timestr,"|");
    if (t_set) {
        node.setValueType(eSet);
        do {
            node.append(atoi(t_set));
            t_set = strtok(NULL, "|");
        } while (t_set);
        return true;
    }

    char *t_min = strtok(timestr, "-");
    char *t_max = strtok(NULL, "-");
    if (t_min && t_max) {
        node.setValueType(eRange);
        node.setRange(atoi(t_min), atoi(t_max));
        return true;
    } else {
        LOGE("parse conditions.timeCondition.x error!\n");
        return false;
    }

    if (!strncmp(timestr, "every", 5)) {
        node.setValueType(eAny);
        return true;
    }

    char *t_not = strtok(timestr, "!");
    if (t_not) {
        node.setValueType(eNot);
        do {
            node.append(atoi(t_not));
            t_not = strtok(NULL, "!");
        } while (t_not);
        return true;
    }

    node.setValueType(eNull);
    return true;
}

bool ElinkRuleDataChannel::_ParsePropValue(const char *cpropval, SlotPoint &slotpoint)
{
    char *propval = (char*)cpropval;
    char *equ = strstr(propval, "==");
    if (equ) {
        slotpoint.append("=", equ + 2);
        return true;
    }
    char *egt = strstr(propval, ">=");
    if (egt) {
        slotpoint.append(">=", egt + 2);
        return true;
    }
    char *elt = strstr(propval, "<=");
    if (elt) {
        slotpoint.append("<=", elt + 2);
        return true;
    }
    char *neq = strstr(propval, "~=");
    if (neq) {
        slotpoint.append("<>", neq + 2);
        return true;
    }
    char *gt = strstr(propval, ">");
    if (gt) {
        slotpoint.append(">", gt + 1);
        return true;
    }
    char *lt = strstr(propval, "<");
    if (lt) {
        slotpoint.append("<", lt + 1);
        return true;
    }
    LOGE("unkown compare logic!\n");
    return false;
}

bool ElinkRuleDataChannel::_ParseConditions(rapidjson::Value &conditions, std::shared_ptr<RulePayload> payload)
{
    rapidjson::Value &conditionLogic = conditions["conditionLogic"];
    if (!conditionLogic.IsString()) {
        LOGE("parse conditions.conditionLogic error!\n");
        return false;
    }
    payload->mLHS->condLogic() = conditionLogic.GetString();

    if (conditions.HasMember("timeCondition")) {
        rapidjson::Value &timeCondition = conditions["timeCondition"];
        if (timeCondition.IsArray()) {
            LHSNode *node = payload->mLHS.get();
            if (timeCondition.Size() > 1)
                node = &(payload->mLHS->makeNode("or"));
            char fctID[8] = { 0 };
            for (size_t i = 0; i < timeCondition.Size(); ++i) {
                rapidjson::Value &dt = timeCondition[i];
                sprintf(fctID, "fct_f%lu", i);
#ifdef USE_TIMER_EVENT
                char eID[9] = { 0 };
                sprintf(eID, "%u", rand() % 100000000);
                std::shared_ptr<TimerEvent> te;
                if (dt.HasMember("week"))
                    te = std::make_shared<TimerEvent>(atoi(eID), true);
                else
                    te = std::make_shared<TimerEvent>(atoi(eID), false);
                Condition &timeCond = node->makeCond(CT_TEMPLATE, "timer-event", fctID);
                timeCond.makeSlot("id").append("eq", eID);
#else
                Condition &timeCond = node->makeCond(CT_FACT, "datetime", fctID);
#endif

                if (!dt.IsObject()) {
                    LOGE("parse conditions.timeCondition[%d] error!\n", i);
                    return false;
                }
                timeCond.makeSlot("clock");
                if (dt.HasMember("year") && dt["year"].IsString()) {
#ifdef USE_TIMER_EVENT
                    if (!_ParseTimeString(dt.GetString(), *(te->year()))) {
#else
                    if (!_ParseTimeString(dt.GetString(), timeCond.makeSlot("year"))) {
#endif
                        LOGE("parse conditions.timeCondition[%d].year error!\n", i);
                        return false;
                    }
                }
                if (dt.HasMember("month") && dt["month"].IsString()) {
#ifdef USE_TIMER_EVENT
                    if (!_ParseTimeString(dt.GetString(), *(te->month()))) {
#else
                    if (!_ParseTimeString(dt.GetString(), timeCond.makeSlot("month"))) {
#endif
                        LOGE("parse conditions.timeCondition[%d].month error!\n", i);
                        return false;
                    }
                }
                if (dt.HasMember("day") && dt["day"].IsString()) {
#ifdef USE_TIMER_EVENT
                    if (!_ParseTimeString(dt.GetString(), *(te->day()))) {
#else
                    if (!_ParseTimeString(dt.GetString(), timeCond.makeSlot("day"))) {
#endif
                        LOGE("parse conditions.timeCondition[%d].day error!\n", i);
                        return false;
                    }
                }
                if (dt.HasMember("hour") && dt["hour"].IsString()) {
#ifdef USE_TIMER_EVENT
                    if (!_ParseTimeString(dt.GetString(), *(te->hour()))) {
#else
                    if (!_ParseTimeString(dt.GetString(), timeCond.makeSlot("hour"))) {
#endif
                        LOGE("parse conditions.timeCondition[%d].hour error!\n", i);
                        return false;
                    }
                }
                if (dt.HasMember("minute") && dt["minute"].IsString()) {
#ifdef USE_TIMER_EVENT
                    if (!_ParseTimeString(dt.GetString(), *(te->minute()))) {
#else
                    if (!_ParseTimeString(dt.GetString(), timeCond.makeSlot("minute"))) {
#endif
                        LOGE("parse conditions.timeCondition[%d].minute error!\n", i);
                        return false;
                    }
                }
                if (dt.HasMember("second") && dt["second"].IsString()) {
#ifdef USE_TIMER_EVENT
                    if (!_ParseTimeString(dt.GetString(), *(te->second()))) {
#else
                    if (!_ParseTimeString(dt.GetString(), timeCond.makeSlot("second"))) {
#endif
                        LOGE("parse conditions.timeCondition[%d].second error!\n", i);
                        return false;
                    }
                }
                if (dt.HasMember("week") && dt["week"].IsString()) {
#ifdef USE_TIMER_EVENT
                    if (!_ParseTimeString(dt.GetString(), *(te->week()))) {
#else
                    if (!_ParseTimeString(dt.GetString(), timeCond.makeSlot("week"))) {
#endif
                        LOGE("parse conditions.timeCondition[%d].week error!\n", i);
                        return false;
                    }
                }
#ifdef USE_TIMER_EVENT
                payload->mTimerEvents.push_back(te);
#endif
            } /* end for */
        } /* end timeCondition */
    }

    if (conditions.HasMember("deviceCondition")) {
        rapidjson::Value &deviceCondition = conditions["deviceCondition"];
        if (deviceCondition.IsObject()) {
            rapidjson::Value &deviceLogic = deviceCondition["deviceLogic"];
            if (!deviceLogic.IsString()) {
                LOGE("parse conditions.deviceCondition.deviceLogic error!\n");
                return false;
            }
            rapidjson::Value &deviceStatus = deviceCondition["deviceStatus"];
            if (!deviceStatus.IsArray()) {
                LOGE("parse conditions.deviceCondition.deviceStatus error!\n");
                return false;
            }
            LHSNode *node = &(payload->mLHS->makeNode(deviceLogic.GetString()));

            for (size_t i = 0; i < deviceStatus.Size(); ++i) {
                rapidjson::Value &dev = deviceStatus[i];
                if (!dev.IsObject()) {
                    LOGE("parse conditions.deviceCondition.deviceStatus[%d] error!\n", i);
                    return false;
                }
                rapidjson::Value &did = dev["deviceId"];
                if (!did.IsString()) {
                    LOGE("parse conditions.deviceCondition.deviceStatus[%d].deviceId error!\n", i);
                    return false;
                }
                rapidjson::Value &pro = dev["propName"];
                if (!pro.IsString()) {
                    LOGE("parse conditions.deviceCondition.deviceStatus[%d].propName error!\n", i);
                    return false;
                }
                rapidjson::Value &val = dev["propValue"];
                if (!val.IsString()) {
                    LOGE("parse conditions.deviceCondition.deviceStatus[%d].proValue error!\n", i);
                    return false;
                }
                Condition &insCond = node->makeCond(CT_INSTANCE,
                    getClassNameByDeviceId(did.GetString()), innerOfInsname(did.GetString()));

                SlotPoint &slotpoint = insCond.makeSlot(pro.GetString());
                char *propval = (char*)val.GetString();
                char *t_and = strstr(propval, "and");
                char *t_or = strstr(propval, "or");

                if (!t_and && !t_or) {
                    if (!_ParsePropValue(propval, slotpoint)) {
                        LOGE("parse conditions.deviceCondition.deviceStatus[%d] error!\n", i);
                        return false;
                    }
                }

                char lside[64] = { 0 };
                char rside[64] = { 0 };
                /* and */
                if (t_and) {
                    slotpoint.mCellLogic = "&";
                    strncpy(lside, propval, t_and - propval);
                    strcpy(rside, t_and+3);
                }

                /* or */
                if (t_or) {
                    slotpoint.mCellLogic = "|";
                    strncpy(lside, propval, t_or - propval);
                    strcpy(rside, t_or+2);
                }

                if (lside[0] && rside[0]) {
                    if (!_ParsePropValue(lside, slotpoint)) {
                        LOGE("parse conditions.deviceCondition.deviceStatus[%d] error!\n", i);
                        return false;
                    }
                    if (!_ParsePropValue(rside, slotpoint)) {
                        LOGE("parse conditions.deviceCondition.deviceStatus[%d] error!\n", i);
                        return false;
                    }
                }
            } /* end for */
        } /* end if IsObject */
    } /* end if deviceCondition */

    return true;
}

bool ElinkRuleDataChannel::_ParseActions(rapidjson::Value &actions, std::shared_ptr<RulePayload> payload)
{
    if (actions.HasMember("notify")) {
        rapidjson::Value &notify = actions["notify"];
        if (notify.IsObject()) {
            rapidjson::Value &title = notify["title"];
            rapidjson::Value &message = notify["message"];
            if (!title.IsString() || !message.IsString()) {
                LOGE("parse actions.notify error!\n");
                return false;
            }
            char id[9] = { 0 };
            sprintf(id, "n-%u", rand() % 1000000000);
            payload->mRHS->makeAction(AT_NOTIFY, id, title.GetString(), message.GetString());
        }
    }

    if (actions.HasMember("deviceControl")) {
        rapidjson::Value &deviceControl = actions["deviceControl"];
        if (deviceControl.IsArray()) {
            for (size_t i = 0; i < deviceControl.Size(); ++i) {
                rapidjson::Value &dev = deviceControl[i];
                if (!dev.IsObject()) {
                    LOGE("parse actions.deviceControl[%d] error!\n", i);
                    return false;
                }
                rapidjson::Value &deviceId = dev["deviceId"];
                rapidjson::Value &propName = dev["propName"];
                rapidjson::Value &propValue = dev["propValue"];
                if (!deviceId.IsString() || !propName.IsString() || !propValue.IsString()) {\
                    LOGE("parse actions.deviceControl[%d].xxx error!\n", i);
                    return false;
                }
                payload->mRHS->makeAction(AT_CONTROL,
                    innerOfInsname(deviceId.GetString()),
                    propName.GetString(), propValue.GetString());
            }
        }
    }

    if (actions.HasMember("manualRuleId")) {
        rapidjson::Value &manualRuleId = actions["manualRuleId"];
        if (manualRuleId.IsArray()) {
            for (size_t i = 0; i < manualRuleId.Size(); ++i) {
                rapidjson::Value &rul =  manualRuleId[i];
                if (!rul.IsString()) {
                    LOGE("parse manualRuleId[%d] error!\n", i);
                    return false;
                }
                payload->mRHS->makeAction(AT_SCENE, innerOfRulename(rul.GetString()));
            }
        }
    }
    return true;
}

void ElinkRuleDataChannel::onSyncRuleProfile(const std::string jsonDoc)
{
    LOGD("jsonDoc:\n%s\n", jsonDoc.c_str());
    rapidjson::Document doc;
    doc.Parse<0>(jsonDoc.c_str());
    if (doc.HasParseError()) {
        rapidjson::ParseErrorCode code = doc.GetParseError();
        LOGE("rapidjson parse error[%d]\n", code);
        return;
    }

    if (!doc.HasMember("status") && !doc.HasMember("result")) {
        LOGE("rapidjson parse error!\n");
        return;
    }

    rapidjson::Value &result = doc["result"];
    if (!result.HasMember("rule")) {
        LOGE("rapidjson parse error, no rule key!\n");
        return;
    }
    rapidjson::Value &rule = result["rule"];

    /* elink v0.0.7 */
    std::string ruleVersion("0.0.7");
    /* parse header */
    rapidjson::Value &ruleName = rule["ruleName"];
    if (!ruleName.IsString()) {
        LOGE("parse rule name error!\n");
        return;
    }
    rapidjson::Value &ruleId = rule["ruleId"];
    if (!ruleId.IsString()) {
        LOGE("parse rule id error!\n");
        return;
    }
    rapidjson::Value &trigger = rule["trigger"];
    if (!trigger.IsObject()) {
        LOGE("parse trigger error!\n");
        return;
    }

    std::shared_ptr<RulePayload> payload = std::make_shared<RulePayload>(
        ruleName.GetString(),
        innerOfRulename(ruleId.GetString()),
        ruleVersion.c_str());

    /* parse trigger */
    if (!_ParseTrigger(trigger, payload)) {
        LOGE("parse trigger object error!\n");
        return;
    }

    /* parse conditions */
    if (rule.HasMember("conditions")) {
        rapidjson::Value &conditions = rule["conditions"];
        if (conditions.IsObject()) {
            if (!_ParseConditions(conditions, payload)) {
                LOGE("parse conditions object error!\n");
                return;
            }
        }
    }

    /* parse actions */
    if (rule.HasMember("actions")) {
        rapidjson::Value &actions = rule["actions"];
        if (!actions.IsObject()) {
            LOGE("parse actions object error!\n");
            return;
        }
        if (!_ParseActions(actions, payload)) {
            LOGE("parse actions object error!\n");
            return;
        }
    }

    mH.sendMessage(mH.obtainMessage(RET_RULE_SYNC, payload));
}

bool ElinkRuleDataChannel::send(int action, std::shared_ptr<Payload> payload)
{
    (void)action;
    (void)payload;
    return false;
}

} /* namespace HB */
