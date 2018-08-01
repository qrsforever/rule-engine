/***************************************************************************
 *  RuleDataChannel.h - Rule Data Channel
 *
 *  Created: 2018-06-14 15:02:39
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RuleDataChannel_H__
#define __RuleDataChannel_H__

#include "DataChannel.h"
#include "RulePayload.h"
#include "TimerEvent.h"
#include "rapidjson/document.h"

#ifdef SIM_SUITE
#include "TempSimulateSuite.h"
#endif

#ifdef __cplusplus

namespace HB {

class RuleEventHandler;

class RuleDataChannel : public DataChannel {
public:
    RuleDataChannel();
    virtual ~RuleDataChannel();

    int init();

    virtual bool send(int action, std::shared_ptr<Payload> payload);

protected:
    RuleEventHandler &mH;
}; /* class RuleDataChannel */

class ElinkRuleDataChannel : public RuleDataChannel {
public:
    ElinkRuleDataChannel();
    ~ElinkRuleDataChannel();

    int init();
    void onSyncRuleProfile(const std::string jsonDoc);

    bool send(int action, std::shared_ptr<Payload> payload);

private:
    bool _ParseTrigger(rapidjson::Value &trigger, std::shared_ptr<RulePayload> payload);
    bool _ParseConditions(rapidjson::Value &conditions, std::shared_ptr<RulePayload> payload);
    bool _ParseActions(rapidjson::Value &actions, std::shared_ptr<RulePayload> payload);

    bool _ParseTimeString(const char *timestr, SlotPoint &slotpoint);
    bool _ParseTimeString(const char *timestr, TimeNode &node);
    bool _ParsePropValue(const char *propval, SlotPoint &slotpoint);

}; /* class ElinkRuleDataChannel */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __RuleDataChannel_H__ */
