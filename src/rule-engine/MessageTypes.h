/***************************************************************************
 *  MessageTypes.h - MessageTypes Header
 *
 *  Created: 2018-06-13 14:26:35
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __MessageTypes_H__
#define __MessageTypes_H__

enum MessageTypes {
    MT_SYSTEM = 1,      /* system message */
    MT_TIMER,           /* timer message */
    MT_NETWORK,         /* network message */
    MT_DEVICE,          /* devices message */
    MT_RULE,            /* rule engine message */
    MT_MONITOR = 99,    /* debug monitor tool */
    MT_SIMULATE = 100,  /* only for simulate */
};

/******************
 *  System Event  *
 ******************/
#define SYSTEM_EVENT_START   1   /* system start event */
#define SYSTEM_EVENT_RESTART 2   /* system restart event */
#define SYSTEM_EVENT_READY   3   /* system ready event */

#define SYSTEM_START_SUCCESS   1    /* system sub event: start ok */
#define SYSTEM_START_ERROR    -1    /* system sub event: start error */

/*****************
 *  Timer Event  *
 *****************/
#define TIMER_EVENT_CYCLE   1   /* Timer sub event: cycle timer */


/*******************
 *  Network Event  *
 *******************/
#define NETWORK_EVENT_CONNECT  1   /* network connect event */

#define NETWORK_CONNECT_SUCCESS  1   /* network sub event: connect ok */
#define NETWORK_CONNECT_ERROR   -1   /* network sub event: connect error */

/****************
 *  Rule Event  *
 ****************/
#define RULE_EVENT_START    1  /* rule start event */
#define RULE_EVENT_RESTART  2  /* rule stop event */
#define RULE_EVENT_SYNC     3  /* rule sync from cloud event */
#define RULE_EVENT_LOAD     4  /* rule load from local event */
#define RULE_EVENT_PARSE    5  /* rule parse */

#define RULE_SYNC_NONE          1    /* rule sync sub event: none new version */
#define RULE_SYNC_NEW_VERSION   2    /* rule sync sub event: new version */
#define RULE_SYNC_CHECK_TIMER   3    /* rule sync sub event: check timer */

/******************
 *  Device Event  *
 ******************/
#define DEVICE_EVENT_STATUS     1   /* device status event */
#define DEVICE_EVENT_PROPERTY   2   /* device property event */

#define DEVICE_STATUS_ONLINE    1   /* device sub event: online */
#define DEVICE_STATUS_OFFLINE   2   /* device sub event: offline */

#define DEVICE_PROPERTY_REPORT  1   /* device sub event: report property */
#define DEVICE_PROPERTY_CONTROL 2   /* device sub event: control property */

/*******************
 *  Monitor Event  *
 *******************/
#define MONITOR_CLOSE_CLIENT 1


#endif /* __MessageTypes_H__ */
