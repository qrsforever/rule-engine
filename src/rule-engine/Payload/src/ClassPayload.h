/***************************************************************************
 *  ClassPayload.h - Class Payload Header
 *
 *  Created: 2018-06-21 13:55:02
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __ClassPayload_H__
#define __ClassPayload_H__

#include "Payload.h"

#include <memory>
#include <string>
#include <vector>

#ifdef __cplusplus

namespace HB {

typedef enum {
    ST_INTEGER,
    ST_FLOAT,
    ST_NUMBER, /* INTEGER + FLOAT */
    ST_STRING,
    ST_SYMBOL,
}SlotType;

class Slot {
public:
    ~Slot();
    SlotType mType;
    std::string mName;
    bool mIsMulti;
    std::string mMin; /* (range min max) */
    std::string mStep;
    std::string mMax;
    std::string mAllowList; /* (allowed-xxx list) */
    std::string toString(std::string fmt = "\n  ");
private:
    friend class ClassPayload;
    Slot(SlotType type, std::string name);
    Slot(std::string name);
}; /* class */

class ClassPayload : public Payload {
public:
    ClassPayload(std::string clsname, std::string supercls, std::string ver, bool abst = false, bool react = true);
    virtual ~ClassPayload();
    PayloadType type() { return PT_CLASS_PAYLOAD; }

    std::string mClsName;
    std::string mSuperCls; /* (is-a superclass) */
    std::string mVersion;
    std::string mRawData;

    bool mIsAbstract; /* (role concrete | abstract) */
    bool mIsReactive;  /* (pattern-match reactive | non-reactive) */

    Slot& makeSlot(std::string name);
    Slot& makeSlot(SlotType type, std::string name, bool multi);
    Slot& makeSlot(SlotType type, std::string name, std::string min, std::string max, bool multi);
    Slot& makeSlot(SlotType type, std::string name, std::string allow, bool multi);

    std::string toString(std::string fmt = "");

private:
    std::vector<Slot*> mSlots;

}; /* class ClassPayload  */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __ClassPayload_H__ */
