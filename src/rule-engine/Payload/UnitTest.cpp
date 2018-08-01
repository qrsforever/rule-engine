/***************************************************************************
 *  UnitTest.cpp - UnitTest for DataPayload
 *
 *  Created: 2018-06-21 14:05:23
 *
 *  Copyright QRS
 ****************************************************************************/

#include "ClassPayload.h"
#include "RulePayload.h"
#include "InstancePayload.h"

using namespace HB;

void test_class_payload()
{
    printf("\n >>> test_class_payload <<< \n");
    std::shared_ptr<ClassPayload> payload = std::make_shared<ClassPayload>("Light", "DEVICE", "1.0.0");
    payload->makeSlot(ST_INTEGER, "switch", "0 1", false);
    payload->makeSlot(ST_FLOAT, "temprature", "-15.0", "95.0", false);
    payload->makeSlot(ST_STRING, "color", "r g b", false);

    printf("%s\n", payload->toString().c_str());
}

void test_rule_payload()
{
    printf("\n >>> test_rule_payload <<< \n");
    std::shared_ptr<RulePayload> payload = std::make_shared<RulePayload>("example", innerOfRulename("0001"), "1.0.0");

    /* Condition */
    payload->mLHS->condLogic() = "and"; // TOP

#if 1
#if 0
    Condition &timeCond = payload->mLHS->makeCond(CT_FACT, "datetime", "fct_t1");
#else
    Condition &timeCond = payload->mLHS->makeCond(CT_TEMPLATE, "datetime", "fct_t2");
#endif
    timeCond.makeSlot("clock");
    timeCond.makeSlot("year").append("=", "2018");
    timeCond.makeSlot("month").append("=", "06");
    timeCond.makeSlot("day", "|").append("=", "20").append("=", "21").append("=", "22");
    timeCond.makeSlot("hour", "none"); /* hour: every hour */
    timeCond.makeSlot("minute"); /* default: none */
    timeCond.makeSlot("second");
    /* timeCond.makeSlot("hour", "&").append(">=", "0").append("<", "24"); */
    /* timeCond.makeSlot("minute").append("=", "0"); */
    /* timeCond.makeSlot("second").append("=", "0"); */
    /* timeCond.makeSlot("wday"); */
#else
    Condition &timeCond = payload->mLHS->makeCond(CT_TEMPLATE, "timer-event", "fct_t1");
    timeCond.makeSlot("id").append("eq", "1001");
#endif

    LHSNode &or_node = payload->mLHS->makeNode("or");

    Condition &cond1 = or_node.makeCond(CT_INSTANCE, "TempSensor", innerOfInsname("0007A895C8A7"));
    cond1.makeSlot("CurrentTemperature").append(">", "50");

    Condition &cond2 = or_node.makeCond(CT_INSTANCE, "Light", innerOfInsname("DC330D799327"));
    cond2.makeSlot("onOffLight").append("=", "1");

    /* Action */
    payload->mRHS->makeAction(AT_CONTROL, innerOfInsname("0007A895C7C7"), "CurrentTemperature", "50");
    payload->mRHS->makeAction(AT_CONTROL, innerOfInsname("DC330D79932A"), "onOffLight", "1");
    payload->mRHS->makeAction(AT_NOTIFY, "10000001", "tellYou", "Girlfriend Birthday");
    payload->mRHS->makeAction(AT_SCENE, "rul-100");

    printf("%s\n", payload->toString().c_str());
}

void test_instance_payload()
{
    printf("\n >>> test_rule_payload <<< \n");
    std::string id("123456");
    std::string innerID = innerOfInsname(id);
    printf("innerID = %s\n", innerID.c_str());
    std::string outerID = outerOfInsname(innerID);
    printf("outerID = %s\n", outerID.c_str());
}

int main(int argc, char *argv[])
{
    /*-----------------------------------------------------------------
    *	Test Class Payload
    *-----------------------------------------------------------------*/
    test_class_payload();

    /*-----------------------------------------------------------------
    *	Test Rule Payload
    *-----------------------------------------------------------------*/
    test_rule_payload();

    /*-----------------------------------------------------------------
    *	Test Instance Payload
    *-----------------------------------------------------------------*/
    test_instance_payload();

    return 0;
}
