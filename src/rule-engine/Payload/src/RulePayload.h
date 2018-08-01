/***************************************************************************
 *  RulePayload.h - Rule Payload Header
 *
 *  Created: 2018-06-21 13:56:36
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RulePayload_H__
#define __RulePayload_H__

#include "Payload.h"

#include <memory>
#include <string>
#include <vector>

#ifdef __cplusplus

namespace HB {

typedef enum {
    CT_INSTANCE,
    CT_FACT,
    CT_TEMPLATE,
} ConditionType;

typedef enum {
    AT_NOTIFY,
    AT_SCENE,
    AT_CONTROL,
    AT_ASSERT,
} ActionType;

class Condition;

class SlotPoint {
public:
    SlotPoint(Condition &cond, std::string name)
        : mSlotName(name), mCellLogic("none"), mCond(cond) {}
    SlotPoint(Condition &cond, std::string name, std::string flag);
    virtual ~SlotPoint();
    std::string mSlotName;
    std::string mCellLogic; /* connective symbol: &, |, ~, none*/

    std::string toString(std::string fmt = "\n      ");

    struct _Cell_ {
        _Cell_(std::string s1, std::string s2)
            : nSymbol(s1), nValue(s2) {}
        std::string nSymbol; /* compare symbol: >, <, =, >=, <=, <>, none */
        std::string nValue;
    };
    size_t cellCount() { return mCells.size(); }
    SlotPoint& append(std::string s1, std::string s2);
    std::string getSymbol(size_t index);
    std::string getValue(size_t index);
private:
    Condition& mCond;
    std::vector<_Cell_> mCells;
}; /* class SlotPoint */

class Condition {
public:
    Condition(ConditionType type, std::string cls, std::string id, std::string logic = "none");
    virtual ~Condition();
    ConditionType mType;
    std::string mCls;
    std::string mID;
    std::string mSlotLogic; /* not used (only support "and"): logic symbol between all slots */
    bool mCheckOldValue;

    std::string toString(std::string fmt = "\n    ");

    SlotPoint& makeSlot(std::string name, std::string logic = "none");
    SlotPoint* get(size_t index) const;
    size_t slotCount() { return mSlots.size(); }
private:
    std::vector<SlotPoint *> mSlots;
}; /* class Condition */

class RulePayload;
class LHSNode {
public:
    LHSNode(RulePayload& rule, std::string logic = "and");
    virtual ~LHSNode();

    std::string toString(std::string fmt = "\n  ");

    Condition& makeCond(ConditionType type, std::string cls, std::string id);
    Condition* getCond(size_t index);
    size_t condCount() { return mConditions.size(); }

    LHSNode& makeNode(std::string logic = "and");
    LHSNode* getChild(size_t index);
    size_t childCount() { return mChildren.size(); }

    std::string& condLogic() { return mCondLogic; }

private:
    RulePayload& mRule;
    std::string mCondLogic; /* conditions logic: and, or, not */
    std::vector<Condition *> mConditions;
    std::vector<LHSNode *> mChildren;
}; /* class LHSNode */

class Action {
public:
    Action(ActionType type, std::string call, std::string name, std::string value);
    Action(ActionType type, std::string call, std::string id, std::string name, std::string value);
    virtual ~Action() { }

    ActionType mType;
    bool mCheckResult;
    std::string mCall;
    std::string mID;
    std::string mSlotName;
    std::string mSlotValue;

    std::string toString(std::string fmt="\n  ");
}; /* class Action */

class RHSNode {
public:
    RHSNode(RulePayload& rule);
    virtual ~RHSNode();

    std::string toString(std::string fmt="\n  ");

    Action& makeAction(ActionType type, std::string value);
    Action& makeAction(ActionType type, std::string name, std::string value);
    Action& makeAction(ActionType type, std::string id, std::string name, std::string value);
    Action* getAction(size_t index);
    size_t actionCount() { return mActions.size(); }
private:
    std::vector<Action *> mActions;
    RulePayload& mRule;
}; /* class RHSNode */

class RulePayload : public Payload {
public:
    RulePayload() {}
    RulePayload(std::string name, std::string id, std::string ver);
    ~RulePayload();
    PayloadType type() { return PT_RULE_PAYLOAD; }
    std::string mRuleName;
    std::string mRuleID;
    std::string mVersion;
    std::string mRawData;
    int mTimeoutMS;
    int mRetryCount;
    bool mEnable;
    bool mAuto;

    std::string toString(std::string fmt = "");

    std::shared_ptr<LHSNode> mLHS;
    std::shared_ptr<RHSNode> mRHS;
    std::vector<std::shared_ptr<::UTILS::Object>> mTimerEvents;
}; /* class RulePayload */

std::string innerOfRulename(std::string name);
std::string outerOfRulename(std::string name);

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __RulePayload_H__ */
