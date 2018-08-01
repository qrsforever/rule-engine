---

title: Rule Engine Intro
date: 2018-06-14 15:48:14
tags: [ IOT, DrawIt ]
categories: [ Local ]

---

Elink
=====

Rule Schema
-----------

```:json
        {
            "ruleName":"example",
            "ruleId":"123456",
            "description":"this is a example for rule definition",
            "trigger":{
                "triggerType":"auto|manual",
                "switch":{
                    "enabled":"on",
                    "timeCondition":"on",
                    "deviceCondition ":"on",
                    "notify":"on",
                    "manual":"on"
                }
            },
            "conditions":{
                "conditionLogic":"and",
                "timeCondition":[
                {
                    "year":"2018",
                    "month":"10",
                    "day":"10|13|17",
                    "hour":"every",
                    "minute":"every",
                    "second":"1"
                }
                ],
                "deviceCondition":{
                    "deviceLogic":"or",
                    "deviceStatus":[
                    {
                        "deviceId":"0007A895C8A7",
                        "propName":"CurrentTemperature",
                        "propValue":"v>50"
                    },
                    {
                        "deviceId":"DC330D799327",
                        "propName":"onOffLight",
                        "propValue":"v==true"
                    }
                    ]
                }
            },
            "actions":{
                "notify":{
                    "title": "xxx",
                    "message":"Girlfriend Birthday!"
                },
                "deviceControl":[
                {
                    "deviceId":"0007A895C7C7",
                    "propName":"CurrentTemperature",
                    "propValue":"50"
                },
                {
                    "deviceId":"DC330D79932A",
                    "propName":"onOffLight",
                    "propValue":"true"
                }
                ],
                "manualRuleId":[
                    "1528374365.417.48775",
                    "1528424679.929.67961"
                ]
            }
        }
```

Device Schema
-------------

```
         {
            "result": {
                "description": "烟雾报警器",
                "id": 12,
                "profile": {
                    "OnlineState": {
                        "tag": "在线状态",
                        "write": "F",
                        "type": "enum",
                        "read": "T",
                        "range": {
                            "2": "离线",
                            "1": "在线"
                        }
                    },
                    "PowerOnOff": {
                        "tag": "开关状态",
                        "write": "F",
                        "type": "enum",
                        "read": "T",
                        "range": {
                            "2": "关闭",
                            "1": "开启"
                        }
                    },
                    "SecurityControl": {
                        "tag": "布防开关",
                        "write": "T",
                        "type": "enum",
                        "read": "T",
                        "range": {
                            "2": "关闭",
                            "1": "开启"
                        }
                    },
                    "Battery": {
                        "tag": "剩余电量",
                        "write": "F",
                        "type": "enum",
                        "read": "T",
                        "range": {
                            "2": "正常",
                            "1": "低电量"
                        }
                    }
                }
            },
            "status": 1,
            "request": "/api/device/profile"
        }
```

*规则引擎的设计参考了Elink的json schema, 但设计上并不会受限该schema, 如果不加新的特殊需求, 设计模型不会大改.*


Rule Model Design
=================

LHS Tree
--------

```
LHSNode Tree

                                              *********          "and": condition logic
                                           *** LHSNode  **
                                           *             *
                                           ***  "and"  ***
                                         /    *********    \
                                        /         |         \
                             child     /          |          \   child
                                      /      cond |           \
                                     /            |            \
                                    /             |             \
                                   /              |              \
                                  /               |               \
                                 /                |                \
                      *********            +-------------+            *********
                   *** LHSNode ***         |  Condition  |         *** LHSNode ***
                   *             *         |             |         *             *
                   ***  "or"   ***         |    "and"    |         ***   "or"  ***
                   /  *********  \         +-------------+            *********
                  /               \
                 /                 \            "and": slot logic, only support "and"
                / cond         cond \
               /                     \                                                +---------------------------+
              /                       \                                               |          +------+         |
      +-------------+           +-------------+                                       |          |      |         |
      |  Condition  |           |  Condition  |                                     Fact    Instance    |         |
      |             |           |             |                                       \        /        |         |
      |   "and"     |           |    "and"    |                                        \ type /         |         |
      +-------------+           +-------------+                                         \    /          v         v
                                       |                                               Condition <---> Device | TimeEvent
                                       |                                               SlotPoint <---> Property
                                       |
                -----------+-----------+-----------+---------------
                           |                       |
                           |                       |
                           v                       v
                    +-------------+         +-------------+     "&": cell logic
                    |  SlotPoint  |         |  SlotPoint  |     "|": cell logic
                    |             |         |             |     "~"
                    |    "&"      |         |     "|"     |
                    +-------------+         +-------------+
                           |                       |
                           |                       |
                           v                       v
                 +-------------------+    +-------------------+
                 |       Cell        |    |       Cell        |    Compare Symbol: "=, >, <, >=, <=, <>"
                 |                   |    |                   |    Connective Symbol: "&, |, ~"
                 |(v > 10) & (v < 20)|    |(v = 10) | (v > 20)|
                 +-------------------+    +-------------------+

```

Clp Struct
----------

```
                                ruleID
                                  ^                   -----> comment, here we want is rule name.
                                  |                  /
                                  |                 /                    -----> MultiSlot
                    (defrule rul-0001 "this is an example"              /
          +--------   (and                                             /
          |             (and                   -------------------------------------------------------
          |               ?fct_t1 <- (datetime ?clock ?year ?month ?day ?hour ?minute ?second $?others) ---+
          |      1        (test (= ?year 2018))                                                            |  Condition: Fact
          |    layer      (test (= ?month 06))                                                             | (not use Template)
          |               (test (or (= ?day 20) (= ?day 21) (= ?day 22) ))                              ---+
          |             )                                    -----------\
          |             (or                                              \------> Cell
  LHSNode |               (object (is-a TempSensor)                                                     ---+
          |                 (ID ?id &:(eq ?id ins-0007A895C8A7))                                           |
          |      2          (CurrentTemperature ?CurrentTemperature &:(> ?CurrentTemperature 50))          |
          |    layer      )                                                                                |
          |               (object (is-a Light)                                                             | Condition: Instance
          |                 (ID ?id &:(eq ?id ins-DC330D799327))          /-----> Cell                     |
          |                 (onOffLight ?onOffLight &:(= ?onOffLight 1)) /                                 |
          |               )       \                   -------------------                               ---+
          |             )          \                           +--> timeout-ms
          +--------   )             --------> SlotPoint        |  +--> retry-count
                     =>                                        |  |
          +--------   (bind ?c (create-rule-context rul-0001 5000 5))   ------> create the rule context
          |           (if (eq ?c FALSE) then (return))
          |           (send ?c act-control ins-0007A895C7C7 CurrentTemperature 50)   ---> action: device control
  RHSNode |           (send ?c act-control ins-DC330D79932A onOffLight 1)
          |           (send ?c act-notify 10000001 "tellYou" "Girlfriend Birthday")  ---> action: message notify
          +---------  (send ?c act-scene rul-1000)                                   ---> action: active scene
                    )

----------------------------------------------------------------------------------------------------------------------------------------

                                                     /---> DEVICE is superclass
          + --------------  (defclass SmogAlarm     /
          |                   (is-a DEVICE) -------/
          |             /---- (role concrete) (pattern-match reactive) ----> can triggered by rule
  Class   |            v      (slot OnlineState (type NUMBER) (allowed-numbers 2 1))
          | (not abstract)    (slot PowerOnOff (type INTEGER) (allowed-numbers 2 1))
          |                   (slot SecurityLevel (type SYMBOL) (allowed-symbols low normal high))
          |                   (slot Battery (type NUMBER) (allowed-numbers 2 1))
          +---------------- )                        /
                                                    /
                                                   v
                                          (INTEGER or FLOAT)

```

1. 定义Rule; (defrule rule-name ${LHSNode} => ${RHSNode})
2. LHSNode := [Condition+], Condition可以由Fact和Object(instance)组成
3. 一个Fact或者Object由一个或多个单槽(SlotPoint)/多槽组成: Fact := [SingleSlot+], Object := [(SingleSlot|MultiSlot)+]
4. 一个槽点比如Fact中的"年|月|日|时|分|秒", Object中的属性, 槽点(属性)值可进行逻辑比较, 构成触发点, 取名为Cell
5. 时间Fact不采用Template原因在程序实现更方便
6. 一旦RHSNode被执行, 首先为该Rule创建Context, Context销毁之时就是Rule的执行得到结束之日(异步).

**TODO**
1. 一个Rule在Context没有销毁之前多次触发, 并无响应, 只有等到Context得到success结果或者超时结束,下次触发Rule方有效.

Rule Class UML
--------------

```
                                          +---------------+
 +--------------+                         |   Condition   |                 +------------+               +----------+
 |    Action    |                         |---------------|                 | SlotPoint  |               |  _Cell_  |
 |--------------|                         |   mSlotLogic  |          -----> |------------|        ------>|----------|
 |  mType       |                         |    mType      |         /       | mCellLogic |       /       | nSymbol  |
 |  mCall       | ^                       |    mCls       |        /        | mSlotName  |      /        | nValue   |
 |  mID         |  \                      |    mID        |       /         | mCells     |◆ ---/mCells   +----------+
 |  mSlotName   |   \                     |   mSlots      |◆ ----/mSlots    +------------+
 |  mSlotValue  |    -------+             +---------------+
 +--------------+    1:n    | mActions       ^                  +-----------+     mCondLogic: and/or/not
                            |                |                  |           |     mSlotLogic: and               +------------+
                          +------------+     |          +--------------+    |     mCellLogic: &,|,~             |  SlotInfo  |
                          |  RHSNode   |     |          |   LHSNode    |    |     nSymbol: =,>,<,>=,<=,<>       |------------|
                          | -----------|     |          |--------------|    |                                   |   nName    |
                          |  mActions  |     |          |  mCondLogic  |   /mChildren                           |   nValue   |
                          +------------+     +--------◇ |  mConditions |  /                                     +------------+
                                   ^         mConditions|  mChildren   |-/  +---------------+                           ^
                                   |                    +--------------+    |  RulePayload  |                    mSlots |  1:n
                                   +----------\                ^            |---------------|                           |
                                               \               |            |  mRuleName    |                           ◆
                              +-------------+   \              |            |  mRuleID      |              +-------------------+
                              | DataChannel |    \             |     mLHS   |  mVersion     |              |  InstancePayload  |
                              |-------------|     \            +----------◇ |  mLHS         |          /---|-------------------|
                              |             |      -----------------------◇ |  mRHS         |         /    |      mInsName     |
                              |send(payload)|                        mLHS   |---------------|        /     |      mClsName     |
                              +-------------+                               |  toString()   |       /      |      mSlots       |
                                  △ ^ ^ △                                   +---------------+      /       +-------------------+
                                  | | | |                                             |           /
                                  | | | |                                             |          |
                  +---------------+ | | +----------------+                            |          |
                  |                 | |                  |                            |          |
                  |                 | |                  |                            ▽          |
         +------------------+       | |     +--------------------------+        +----------+ ◁ --+             +-----------------+
         | RuleDataChannel  |       | |     |    DeviceDataChannel     |        | Payload  |                   |  ClassPayload   |
mCloudMgr|------------------|       | |     | ------------------------ |        |----------| ◁ ----------------|-----------------|
  +----◆ |    mCloudMgr     |       | |     |       mDeviceMgr         |        |  type()  |                   |   classname     |
  |      |    mHandler      |       | |     |       mHandler           |        +----------+                   |   superclass    |
  |      +------------------+       | |     |--------------------------|             |                mSlots   |   version       |
  |               △                 | |     |    onStateChanged()      |             |                   +---◇ |    mSlots       |
  |               |                 | |     |    onPropertyChanged()   |-------------+               1:n |     |-----------------|
  |               |                 | |     |         send()           |                                 |     |   toString()    |
  |    +------------------------+   | |     +--------------------------+◆ ---------------------+         |     +-----------------+
  |    |  ElinkRuleDataChannel  |   | |                △          ◆        mHandler            |         |
  |    |------------------------|   | |                |          |                            |         |
  |    |                        |   | |                |          +----------+                 |         |
  |    |      onRuleSync()      |   | |   +-------------------------+        |                 |         |          +---------------+
  |    |        send()          |   | |   | ElinkDeviceDataChannel  |        |                 |         |          |     Slot      |
  |    +------------------------+   | |   |-------------------------|        |                 |         +--------> |---------------|
  |                                 | |   |      onProfileSync()    |        |                 |                    |    mType      |
  |                                 | |   +-------------------------+        |                 |                    |    mName      |
  |                                 | |                                      |                 |                    |   mMin/mMax   |
  |    +-------------------------+  | |                           mDeviceMgr |                 |                    |   mAllowList  |
  |    |     CloudManager        |  | |                                      v                 |                    |---------------|
  |    |-------------------------|  | |   +----------------------------------------+           |                    |   toString()  |
  +--->|                         |  | |   |             DeviceManger               |           |                    +---------------+
       | registRuleSyncCallback  |  | |   | -------------------------------------- |           |
       |                         |  | |   |                                        |           |
       +-------------------------+  | |   |   registDeviceStateChangedCallback     |           |
                                    | |   |   registDevicePropertyChangedCallback  |           |
                                    | |   |   registDeviceProfileSyncCallback      |           |
+-----------------------------------+ |   |        setProperty                     |           |
| +-----------------------------------+   +----------------------------------------+           |
| |                                                                                            |
| |                    +----------------------------+        +----------------+                |
| |             +----▷ |  MessageHandler::Callback  | <----◇ | MessageHandler | ◁ ---------+   |
| |             |      +----------------------------+        |----------------|            |   |
| |             |                                            |   mCallback    |            |   |
| |             |                                            +----------------+            |   |
| |  +----------------------+                                                              |   |
| |  |  RuleEngineService   |                                                              |   v
| |  |----------------------|   mUrgentHandler                                    +--------------------+
| |  |     mUrgentHandler   |◇ -------------------------------------------------->|  RuleEventHandler  |
| |  |                      |    mCore         +----------------------+    ------>|--------------------|
| |  |  mCore/mCoreUrgent   | ◆ -------------> |    RuleEngineCore    |   /       |    handleMessage   |
| |  |     mServerRoot      |                  |----------------------|  /        +--------------------+
| +--|     mDeviceChannel   |          mEnv    |    mHandler          | /mHandler     ^     |
+----|     mRuleChannel     |        +-------◆ |    mEnv              |◆              |     |
     | mOfflineInsesCalled  |        |         |----------------------|         ------+     |
     |----------------------|        |         |    driver()          |        /            |
     |    callInstancePush  |        |         |                      |       /    +------------------+
     |    callMessagePush   |        |         |   handleTimer        |      /     | RuleEventThread  |
     |    setDeviceChannel  |        |         |   handleClassSync    |      |     |------------------|
     |    setRuleChannel    |        |         |   handleRuleSync     |      |     |   mMessageQueue  |
     |     handleMessage    |        |         |   handleInstanceAdd  |      |     +------------------+
     +----------------------+        |         |   handleInstanceDel  |      |              △
             ◇                       v         |   handleInstancePut  |      |              |
      mStore |             +---------------+   +----------------------+      |              |
             |             |  Environment  |                                 |              |
             |             |               |--\                              |    +---------------------+
             |             +---------------+   \  CLP                        |    |   RuleUrgentThread  |
             |              Clipscpp            ------> clips6.30            |    |---------------------|
             \                                                               |    |     mService        |
              \                                                              |    +---------------------+
               \            +----------------------+                         |
                \           |   RuleEngineStore    |                         |
                 ---------> |----------------------|                         |
           /--------------◆ |     mDB              |                         |
          /        mDB      |     mHandler         |◆ -----------------------+
         /                  |     mDefTable        |    mHandler
        |                   |----------------------|\
        |                   |    open/close        | \           +---------------------+
        v                   |   updateClassTable   |  \ mTables  |      DefTable       |
+--------------+            |   updateRuleTable    |   --------> |---------------------|
|SQLiteDatabase|            |  queryClassFilePaths |             |     mDB             |
|              |            |  queryRuleFilePaths  |             | mUpdateHistoryList  |
|              |            |                      |             |---------------------|
+--------------+            +----------------------+             |   updateOrInsert    |
                                                                 |   getFilePaths      |
                                                                 |   getDefInfos       |
                                                                 |     _Update         |
                                                                 +---------------------+
                                                                     △            △
                                                                    /              \
                                                                   /                \
                                                                  |                  |
                                                                  |                  |
                                                       +------------------+    +-----------------+
                                                       |   DefClassTable  |    |   DefRuleTable  |
                                                       |                  |    |                 |
                                                       +------------------+    +-----------------+

```

1. 核心驱动: RuleEngineService RuleEngineCore RuleEngineStore
2. 数据通道: RuleDataChannel DeviceDataChannel
3. 载体转换: ClassPayload InstancePayload RulePayload
4. 辅助工具: MessageHandler SQLiteDatabase Log

Clp Script Design
-----------------

```
                                                                  (auto)     (manual)
                                          device                 property
                      profile  rule   online offline             changed      scene
               TestCase --------------------------------------------------------------------------------------------------->
                         |       |       |      |        ║          |           |
                         |       |       |      |    mainHandler    |           |
            mainHander() |       |       |      |        ║          |           |
                v        v       v       v      v        ▼          v           v
   MainThread ------------------------------------------------------------------------------------------------------------->
                 \    profile  rule      \      \          ║        \           \
                  \      json    json     \      \     ruleHandler   \           \
                   \        \      \       |      |        ║          |           |
                    \        \      \      |      |        ▼          |           |
                     \        \      \     v  T   v     T         T   v     T     v   T         T         T         T
      RuleThread     --------------+----------+---------+---------+---+-----+---------+---------+---------+---------+----->
                      ^            T      add |  del    |         |   \     |     /   |         |         |         |
                  ruleHander()     |          |         |         |    \    |    /    |         |         |         |
                                   |          |         |         |     |   |   |     |         |         |         |
                                   +--\       +--\      +--\      +--\  v   v   v  /--+      /--+      /--+      /--+
                                       \          \         \         \+---------+/         /         /         /
 T: timer (default 1s)                  \          \         \         | refresh |         /         /         /
                                         ----------------------------->|         |<----------------------------
                                                                       |   run   |
                                                                       +---------+
                                                                            |               +--------+
             (LHS)                                                          |         ----▷ |  USER  |
             +--------------------------------------------------------------+        /      +--------+
   salience  |                                                                      /            △
             |                        +-------------------+                        /             |
             v                        |   RuleContext     |                       /              |
        +--------+                    |-------------------|                      /              /
    +---| Rule-1 |                    |   rule-id         |                     /              /
    |   +--------+                    |   timeout-ms      |           +---------+       +-----------+        +----------------+
    |                                 |   retry-count     |---------▷ | Context |       |  DEVICE   |        |  SmogAlarm     |
    +-> +--------+                    |   current-try     |           |         |       |-----------|        |----------------|
    +---| Rule-2 |                    |   start-time      |           +---------+       |   ID      |◁ ------|   OnlineState  |
    |   +--------+                    |   act-error       |                             |   UUID    |        |   PowerOnOff   |
    |                                 |   unanswer-list   |                             |   Class   |        |   Battery      |
    +-> +--------+                    |   response-rules  |                             +-----------+        +----------------+
    +---| Rule-3 |                    |-------------------|
    |   +--------+                    |   try-again       |
    |                                 |   unanswer-count  |
    +-> +--------+                    |   act-control     |
        | Rule-4 |                    |   act-notify      |
        +--------+                    |   act-scene       |
            |                         +-------------------+
            |                                      +-------------------------+----------------------+------------+
            |                                      |                         |                      |            |
            |          Action                      |                         |                      |            v
            +------------------------- >  act-control ------------> act-notify -----------> act-scene   |   send-message
            (RHS)                             |  ^                      |   ^                   /       |        |
                            act-step-control  |  | true                 |   | true             /        |        | success
                                              |  |                      |   |                 v                  |   or
                                              |  +------>  make-rule    |   +------>  make-rule                  |  fail
                                              |  | false  (response)    |   | false  (response)                  |
                                              v  |                      v   |                                    v
          RuleEngineService ---------------------------------------------------------------------------------------------->
                                              ins-push                  txt-push                              msg-push



```

1. 规则分为联动(Auto)和手动(Manual)两种, 联动: 设备属性变化触发相关规则 手动: 触发场景,比如回家模式
2. RuleThread是专门的规则线程, 作用: 延时Timer以及RefreshAgenda/Run(触发Rule,执行Action), 这样设计相对简单, 避免Rule多线程问题
3. 脚本Actions中的方法act-control/act-notify会调用RuleEngineService中的ins-push/txt-push(返回false:异步, 返回true:同步)
4. 如果ins-push/txt-push是异步(return:false)方式, 则act-xxx会自动创建等待结果的规则, 当父规则Context销毁, 该auto规则删除
5. 如果联动规则触发了场景规则被调用, act-scene也会自动创建等待(子规则)结果规则, 且它触发的规则也会创建Context.
6. act-xxx方法是RHS中Action, 每个action的执行可以是同步异步(3,4). 如果异步则会自动创建规则(autorule)用以等待结果, 并将之存入
    response-rules中, 除此之外, 把自身funcall注入unanswer-list中,如果autorule检查到action执行成功, 会将funcall从unanswer-list移除,
    Rule是判断unanswer-list的size决定自身是否执行成功的(size=0: success). RuleEngineService根据T(default:1是)定时给脚本喂时间, 这
    样Rule就可以实现超时机制, (单次)超时后, 首先检查current-try次数, 如果小于retry-count, 则从unanswer-list中取出没有完成的
    funcall,并且执行.
7. 如果规则被触发, 但是RHS中的Action控制的设备Poweroff, RuleEngineService会得知该事件, 并在设备上线后刷新与之相关的Rules

Act Auto Gen Rule
-----------------

act-control(ins-04FA8309822A, CleaningMode, 2):
```

         +---> '_' represent inner auto generate
         |
         |          +---> parent rule name that call act-control in RHS
         |          |
         |          |                  +--> sign this rule is for reponse
         |------------------------     |
(defrule _rul-1529578016.389.86822-response-ins-04FA8309822A |---> action id (here is instance name)
    (declare (salience 1000))               -----------------+
    (object (is-a SweepingRobot) (ID ?id &:(eq ?id ins-04FA8309822A)) (CleaningMode ?v &:(eq ?v 2)))
  =>
    (send [rul-1529578016.389.86822] action-success "act-control ins-04FA8309822A CleaningMode 2")
)         -------------------------- -------------- ---------------------------------------------
          |                            |                                         |
          +--> parent rule context     +--> rule context method                  |
                                            modify unanswer-list                 v
                                                                        arguments of action-success
```
*将设置instance(设备)的属性值作为条件,被控制的设备属性值如果上报值符合条件,证明控制成功*


act-notify(n-350490027, "tellYou" "Girlfriend Birthday"):
```

                                             notify id        (assert (rule-response notifyid success))
                                            -----------       |
(defrule _rul-1529583875.818.80441-response-n-350490027       |
    (declare (salience 1000))                                 |
    (rule-response n-350490027 success)  <--------------------+
  =>
    (send [rul-1529583875.818.80441] action-success "act-notify n-350490027 \"tellYou\" \"Girlfriend Birthday\"")
)

```
*通过响应Fact(rule-response notifyid success)判断消息通知是否执行成功.*


act-scene(rul-1529578676.958.69587):
```
                                                                       (assert (rule-response ruleid success))
                                            rule name: nest call       |
                                            ------------------------   |
(defrule _rul-1529578775.206.24324-response-rul-1529578676.958.69587   |
    (declare (salience 1000))                                          |
    (rule-response rul-1529578676.958.69587 success)  <----------------+
  =>
    (send [rul-1529578775.206.24324] action-success "act-scene rul-1529578676.958.69587")
)
```
*通过响应Fact(rule-response ruleid success)判断消息通知是否执行成功.*


Others
======

Local Build
-----------

1. make

> 除了clips代码(编译后在Makefile中指定), 其他依赖模块会依次编译:
    @cd $(MISC_DIR); make;
    @cd $(MESSAGE_DIR);make;
    @cd $(LOG_DIR);make;
    @cd $(CLIPSCPP_DIR);make;
    @cd $(PAYLOAD_DIR);make;
    @cd $(DRIVER_DIR);make;

2. make clean

> 依赖模块会依次清除:
    @cd $(MISC_DIR); make clean;
    @cd $(MESSAGE_DIR);make clean;
    @cd $(LOG_DIR);make clean;
    @cd $(CLIPSCPP_DIR);make clean;
    @cd $(PAYLOAD_DIR);make clean;
    @cd $(DRIVER_DIR);make clean;


Sync Elink Script
-----------------

1. 自动同步云端的profile和rules: `cd test; ./sync_from_cloud.py`

2. 脚本通过请求获取信息, 其中Token可能改变, 需要定时修改

```
class ElinkRequest(object):

   """Docstring for RequestHeader. """

   def __init__(self, uri):
       self._ip = "10.185.30.96"
       self._host = "smarthome.lecloud.com"
       self._token = "238XXXvKm3oZNGEm1Sm2T6Udm2Wnr4nikdxi6fm1kfU4Pj17RxSDAv5B2n3m5RMitfRw0b6VGLjoVsJejE4z3B9Z0Bt1cm3ewDuBBuOwLkRrUxinrb3zGkwUm4"
       self._uri = uri
```

3. 脚本只供开发调试使用


Rule Categories
---------------

classified into three major categories:

- global rules which cannot be disable, such as: retract the fact of datetime which asserted per second

    ```
    (defrule retract-datetime
        (declare (salience ?*SALIENCE-LOWEST*))
        ?f <- (datetime $?)
      =>
        (retract ?f)
    )
    ```

 *一个fact被声明, 将不会消失, 但可以通过modify修改, 也可以retract将其收回.*

- custom rules configured through UI which can enable or disable. the naming conventions of the rule is:
    ```
    ; "rul-" is the prefix of the custom rule name
    (defrule rul-1529578775.206.24324 "autotest3"
      (object (is-a AirClean1)
        (ID ?id &:(eq ?id ins-0007A895C8A7))
        (WorkMode ?WorkMode &:(= ?WorkMode 5))
      )
     =>
      (bind ?c (create-rule-context rul-1529578775.206.24324))
      (if (eq ?c FALSE) then (return))
      (send ?c act-scene rul-1529578676.958.69587)
    )
    ```
*用户有权开启/关闭自己配置的规则, 设计上一个custom rule对应一个clp文件, 方便删除更新.*

- auto-gen rules which have short life cycle, disappeared when got the response of the action or rule context destroied.

    ```
    ; "_rul-" is the prefix of the auto-gen rule name
    (defrule _rul-1529578775.206.24324-response-rul-1529578676.958.69587
        (declare (salience 1000))
        (rule-response rul-1529578676.958.69587 success)
      =>
        (send [rul-1529578775.206.24324] action-success "act-scene rul-1529578676.958.69587")
    )
    ```
*用户不可见, Action方法中内部生成, 当Action有response(或者Rule超时/失败)则自动删除*

TODO
====

1. 不使用模板的MultiSlot的Fact作为条件, Slot是有顺序的, 判断Fact时, 可以省略后面的, 不可以从中间省略, 例如:

    ```:-
    正确:
      ?fct_t1 <- (time ?year ?month ?day ?hour ?minute ?second $?others)
      ?fct_t2 <- (time ?year ?month ?day ?hour ?minute $?others)
      ?fct_t3 <- (time ?year ?month ?day ?hour $?others)
    错误(语法正确, 意义错误, second其实是hour):
      ?fct_t4 <- (time ?year ?month ?day ?second $?others)
    ```

2. 使用带模板的Fact作为条件, 模板Slot没有顺序限制, 但需要谨记在使用完之后retract掉, 否则不会消失, 例如:

    ```:-1
    (defrule test
        ?fct_t1 <- (datetime (year ?y &:(= ?y 2018) (second ?s))
      =>
        (retract fct_t1)
    )

    触发rule:test
    (assert (datetime (year 2018) (second 10)))
    不触发rule, 所以这个fact没有被retract, 该fact一直存在, 浪费MEM, 可以使用Object(Instance)取代
    (assert (datetime (year 2019) (second 10)))
    ```

3. 云端下发的规则ID及为某个设备创建实例ID时, 为了避免规则ID和实例ID可能是纯数字, 比如设备实例其实就是DeviceID, 使用下面API转:

    ```:-
    加ins-前缀
    std::string innerOfInsname(std::string name);
    std::string outerOfInsname(std::string name);

    加rul-前缀
    std::string innerOfRulename(std::string name);
    std::string outerOfRulename(std::string name);
    ```

4. 暂时不支持运行时更新设备类, 比如Light, 原因是如果该类在Rule的条件LHS中引用, 该类将无法直接删除更新.

5. 运行时动态使能的设计(未设计)

6. 对中文的支持(支持)

7. 简单规则测试 (see TempSimulateSuite.cpp)  (finished)

RULE | LHS | ACTION | PASS ?
-----|-----|--------|:----:
autotest1 | instance | act-control | Yes
autotest3 | instance | act-scene | Yes
autotest4 | instance | act-notify | Yes
manualtest1 | fact | act-control | Yes
manualtest3 | fact | act-control | Yes

8. LHS带有`and`和`or`的规则测试

```clp
    (defrule rul-0000000000.000.00001 "tv-light-rule"
      (and
        (object (is-a Light)
          (ID ?ins-38D269B0EA1801010311 &:(eq ?ins-38D269B0EA1801010311 ins-38D269B0EA1801010311))
          (PowerOnOff ?PowerOnOff &:(= ?PowerOnOff 1))
        )
        (object (is-a Letv)
          (ID ?ins-00000000000000000002 &:(eq ?ins-00000000000000000002 ins-00000000000000000002))
          (PowerOnOff ?PowerOnOff &:(= ?PowerOnOff 1))
        )
        (object (is-a LightSensor)
          (ID ?ins-00000000000000000001 &:(eq ?ins-00000000000000000001 ins-00000000000000000001))
          (Quantity ?Quantity &:(>= ?Quantity 10 )&:(<= ?Quantity 20))
        )
      )
     =>
      (bind ?c (create-rule-context rul-0000000000.000.00001))
      (if (eq ?c FALSE) then (return))
      (send ?c act-control ins-38D269B0EA1801010311 Brightness 2)
    )
```
*TestCase: `TEST_RULE_AND` Pass*
*在and逻辑下, object slot (ID ?id) id变量名必须不能一样(这里使用did), 否则and条件永远不成立*

```clp
    (defrule rul-0000000000.000.00002 "emergency-alarm-rule"
      (or
        (object (is-a EmergencyButton)
          (ID ?ins-38D269B0EA1886D3E200 &:(eq ?ins-38D269B0EA1886D3E200 ins-38D269B0EA1886D3E200))
          (PowerOnOff ?PowerOnOff &:(= ?PowerOnOff 1))
        )
        (object (is-a SmogAlarm)
          (ID ?ins-00124B00146D743D00 &:(eq ?ins-00124B00146D743D00 ins-00124B00146D743D00))
          (PowerOnOff ?PowerOnOff &:(= ?PowerOnOff 1))
        )
      )
     =>
      (bind ?c (create-rule-context rul-0000000000.000.00002))
      (if (eq ?c FALSE) then (return))
      (send ?c act-notify n-714636915 "Your house is dangerous!" "Warning")
    )
```
*TestCase: `TEST_RULE_OR` Pass*

9. LHS.Condition.SlotPoint带有的`&`和`|`"规则测试

10. RHS触发的Action中设备如果不在线(poweroff), 那么需要在设备上线后重新刷新相关规则(条件重新判断)

*TestCase: `TEST_RULE_INS_ONLINE_LATER` Pass*

11. RHS触发的Action中设备的最终状态,需要按一定step,逐步达到该效果.

*TestCase: `TEST_RULE_STEP_CONTROL` Pass*
*用户只是要最终状态, 至于如何达到这个状态(begin, step)用户其实不需要关注, 所以这个功能暂时没有实际意义*
