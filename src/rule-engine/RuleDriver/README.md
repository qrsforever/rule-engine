---

title: RuleDriver
date: 2018-06-12 16:19:58
tags: [ C++ ]
categories: [ Local ]

---

依赖
====

1. Utils: `https://github.com/qrsforever/CppUtils`
>> see:
    Misc/README.md
    Message/README.md
    Log/README.md

2. Clipscpp: `https://github.com/qrsforever/Clipscpp`
>> see:
    Clipscpp/README.md

3. Clips630:


本地调试编译
============

1. 修改`Makefile`指定`CLIPS_DIR`路径

2. make; make test; make clean


HomeBrain工程编译调试
=====================

1. `cd $TOP_DIR`

2. `make linux DIR=homebrain/src/rule-engine`

TODO:

```
select * from sqlite_master where type = "table"
.schema
```
