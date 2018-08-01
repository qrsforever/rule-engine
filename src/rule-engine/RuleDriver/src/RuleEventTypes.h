/***************************************************************************
 *  RuleEventTypes.h - RuleEventTypes Header
 *
 *  Created: 2018-06-13 18:14:49
 *
 *  Copyright QRS
 ****************************************************************************/

/****************
*  Rule Event  *
****************/
#define RET_DEBUG               0           /* Rule Event Type: debug */
#define RET_REFRESH_TIMER       1           /* Rule Event Type: refresh timer */
#define RET_STORE_CLOSE         2           /* Rule Event Type: close store database */
#define RET_TIMER_EVENT         3           /* Rule Event Type: timer event */
#define RET_LOG_LEVEL           4           /* Rule Event Type: clips log level */
#define RET_LOG_ERROR           5           /* Rule Event Type: check clips halt */
#define RET_WATCH_ITEM          6

#define RET_RULE_SYNC           11          /* Rule Event Type: rule sync from cloud */
#define RET_CLASS_SYNC          12          /* Rule Event Type: class profile sync */
#define RET_RULE_ADD            13
#define RET_RULE_DELETE         14

#define RET_INSTANCE_ADD        21          /* Rule Event Type: device online */
#define RET_INSTANCE_DEL        22          /* Rule Event Type: device offline */
#define RET_INSTANCE_PUT        23          /* Rule Event Type: device put attr */

#define RET_TRIGGER_SCENE       31          /* Rule Event Type: trigger scene rule by manually*/
#define RET_SWITCH_RULE         32          /* Rule Event Type: enable or disable rule */
#define RET_ASSERT_FACT         33          /* Rule Event Type: assert fact to clip */

/****************
*  Rule Debug   *
****************/
#define DEBUG_SHOW_ALL          0
#define DEBUG_SHOW_CLASSES      1
#define DEBUG_SHOW_RULES        2
#define DEBUG_SHOW_INSTANCES    3
#define DEBUG_SHOW_FACTS        4
#define DEBUG_SHOW_AGENDA       5
#define DEBUG_SHOW_MEMORY       6
#define DEBUG_SHOW_GLOBALS      7

/*****************
*  Timer Event   *
*****************/
#define TIMER_TOPLAY        1     /* Timer Event subevent: timer to play */
#define TIMER_DURATION      2     /* Timer Event subevent: timer duration */

/*****************
*  Rule Script   *
*****************/
#define TYPE_TEM_FILE       1   /* Rule Load Rule File Type: Template */
#define TYPE_CLS_FILE       2   /* Rule Load Rule File Type: Class */
#define TYPE_RUL_FILE       3   /* Rule Load Rule File Type: Rule */

#define MSG_RULE_RESPONSE   1   /* Rule Message What: must be consistent with clp script */
#define MSG_RULE_RHS        2   /* Rule Message What: occur on RHS */

#define RUL_SUCCESS     1
#define RUL_FAIL        2
#define RUL_TIMEOUT     3

#define RHS_INS_NOT_FOUND   1   /* Rule RHS SUB Arg1: not found instance id */
#define RHS_NTF_WRONG_TYPE  2   /* Rule RHS SUB Arg1: parameters is invalid */
#define RHS_SEE_NOT_FOUND   3   /* Rule RHS SUB Arg1: scene not found */

#define TEMLS_SEARCH_DIR "templates/"   /* specify directory of templates file */
#define CLSES_SEARCH_DIR "classes/"     /* specify directory of classes file */
#define RULES_SEARCH_DIR "rules/"       /* specify directory of rules file */


/*************
*  Others    *
**************/
#define RES_SUCCESS         0
#define RES_FAIL           -1

#define ENUM_ENABLE         1
#define ENUM_DISABLE        2

