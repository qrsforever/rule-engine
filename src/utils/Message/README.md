---

title: Utils之Message
date: 2018-06-05 17:55:45
tags: [ C++, IOT ]
categories: [ Local ]

---

依赖
====

1. misc

本地调试编译
============

1. 编译依赖 `cd ../Misc; make`

2. 编译调试 `make; make test`

3. 清除 `make clean`

HomeBrain工程编译调试
=====================

1. `cd $TOP_DIR`

2. `make linux DIR=homebrain/src/utils`

设计框图
========

```
v0.0.1
            +--------------------------------------------------------------------------------------------------+
            |                                                                                                  |
            |                                                                                                  |
            |                                                                       +-----------------+        |
            |                                                                       |     Message     |        |
            |                                                                    -->|-----------------|        |
            |                                       +----------------+          /   |      what       |        |
            |                                       |  MessageQueue  |         /    |    arg[1|2|3]   | target |
            |                                 ----->|----------------|        /     |     target      |◇ ------+
            |                                /      |    mMessages   |-------/      |-----------------|
            |                               /       |----------------| mMessage     |     obtain      |
            |                              /        | enqueueMessage |              |    recycle      |
            v                             /         |  removeMessage |              |      next       |
 +--------------------+                  /          |   nextMessage  |              +-----------------+
 |   MessageHandler   |                 /           +----------------+
 |--------------------|  mQueue        /
 |   mMessageQueue    |---------------/
 |   mMessageHandler  |◆ -----------------+
 |--------------------|  mMessageHandler  |
 |  dispatchMessage   |                   |                +------------+
 | sendMessage[Delay] |                   |        +-----▷ |   Thread   |
 |   handleMessage    |                   |        |       |------------|
 +--------------------+                   |        |       |    PID     |
           △                              |        |       |------------|
           |                              |        |       |    run     |
           |                              |        |       +------------+
           |                              v        |
 +-------------------+              +----------------------+
 | RuleEngineHandler |              |     MessageLooper    |
 |-------------------|              |----------------------|
 |   handleMessage   |              |       mMsgQueue      |      +--------------------------------------+
 +-------------------+              |----------------------|      |while(true)                           |
                                    |        run           |----->|   msg = mMessageQueue->nextMessage() |
                                    +----------------------+      |   msg->target->dispatchMessage()     |
                                               △                  +--------------------------------------+
 +-------------------+                         |
 | RuleEngineThread  |                         |
 |-------------------|-------------------------+
 |                   |
 |-------------------|
 |       run         |
 +-------------------+
```
