/***************************************************************************
 *  RuleEventHandler.h - Rule Engine Service Header
 *
 *  Created: 2018-06-12 18:07:36
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RuleEventHandler_H__
#define __RuleEventHandler_H__

#include "MessageHandler.h"
#include "RuleEventTypes.h"

#ifdef __cplusplus

namespace HB {

class RuleEventHandler : public ::UTILS::MessageHandler {
public:
    typedef std::shared_ptr<RuleEventHandler> pointer;

    RuleEventHandler(::UTILS::MessageQueue *queue, pthread_t id);
    ~RuleEventHandler();

    pthread_t id() { return mID; }

protected:
    void handleMessage(::UTILS::Message *msg);
    pthread_t mID;

}; /* class RuleEventHandler */

RuleEventHandler& ruleHandler();
RuleEventHandler& urgentHandler();

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __RuleEventHandler_H__ */

