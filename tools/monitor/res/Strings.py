#!/usr/bin/python3
# -*- coding: utf-8 -*-

strings = {
        'quit' : {
            'en' : 'Quit',
            'cn' : '退出'
            },
        'back' : {
            'en' : 'Back',
            'cn' : '返回'
            },
        'title' : {
            'en' : 'MonitorTool',
            'cn' : '监控工具'
            },
        'newruletitle' : {
            'en' : 'New Rule File',
            'cn' : '新建规则'
            },
        'delruletitle' : {
            'en' : 'Delete Rule File',
            'cn' : '删除规则'
            },
        'serverAddr' : {
            'en' : 'Server Address:',
            'cn' : '服务器地址:'
            },
        'serverPort' : {
            'en' : 'Server Port:',
            'cn' : '服务器端口:'
            },
        'host' : {
            'en' : 'Host Address:',
            'cn' : '主机地址:'
            },
        'port' : {
            'en' : 'Host Port:',
            'cn' : '主机端口:'
            },
        'output' : {
            'en' : 'Output',
            'cn' : '输出'
            },
        'terminate' : {
            'en' : 'Terminate',
            'cn' : '终止'
            },
        'connect' : {
            'en' : 'Connect',
            'cn' : '连接'
            },
        'connErr' : {
            'en' : 'Connect Error',
            'cn' : '连接失败'
            },
        'basicInfo' : {
            'en' : 'Basic Infomation',
            'cn' : '基本信息'
            },
        'versionInfo' : {
            'en' : 'Version Info',
            'cn' : '版本信息'
            },
        'hbVer' : {
            'en' : 'HomeBrain: ',
            'cn' : 'HomeBrain: '
            },
        'reVer' : {
                'en' : 'RuleEngine: ',
            'cn' : 'RuleEngine: '
            },
        'logSet' : {
            'en' : 'Log Setting',
            'cn' : '日志设置'
            },
        'clear' : {
            'en' : 'Clear',
            'cn' : '清除'
            },
        'watch' : {
            'en' : 'Watch Item',
            'cn' : '脚本监控'
            },
        'logOutput' : {
            'en' : 'Log Output',
            'cn' : '日志输出'
            },
        'ruleCtrl' : {
            'en' : 'Rule Control',
            'cn' : '规则控制'
            },
        'set' : {
            'en' : 'Set',
            'cn' : '设置'
            },
        'lanSwitch' : {
            'cn' : 'Language Switch ',
            'en' : '中英文切换'
            },
        'about' : {
            'en' : 'About',
            'cn' : '关于'
            },
        'loglevel' : {
            'en' : 'Log Level',
            'cn' : '日志级别'
            },
        'logFilter' : {
            'en' : 'Log Filter',
            'cn' : '日志过滤'
            },
        'logModule' : {
            'en' : 'ModuleName',
            'cn' : '模块名'
            },
        'logError' : {
            'en' : 'Error',
            'cn' : '错误'
            },
        'logWarn' : {
            'en' : 'Warning',
            'cn' : '警告'
            },
        'logNormal' : {
            'en' : 'Normal',
            'cn' : '正常'
            },
        'logInfo' : {
            'en' : 'Infomation',
            'cn' : '信息'
            },
        'logTrace' : {
            'en' : 'Trace',
            'cn' : '跟踪'
            },
        'deviceSel' : {
            'en' : 'Device:',
            'cn' : '设备:'
            },
        'deviceID' : {
            'en' : 'UUID:',
            'cn' : 'UUID:'
            },
        'setVal' : {
            'en' : 'Set',
            'cn' : '设置'
            },
        'getVal' : {
            'en' : 'Get',
            'cn' : '获取'
            },
        'refresh' : {
            'en' : 'Refresh',
            'cn' : '刷新'
            },
        'manualRefresh' : {
            'en' : 'Manual Refresh',
            'cn' : '手动刷新'
            },
        'autoRefresh' : {
            'en' : 'Auto Refresh',
            'cn' : '自动刷新'
            },
        'stopRefresh' : {
            'en' : 'Stop Refresh',
            'cn' : '停止刷新'
            },
        'filter' : {
            'en' : 'Filter:',
            'cn' : '过滤:'
            },
        'ruleSel' : {
            'en' : 'Rule:',
            'cn' : '规则:'
            },
        'commit' : {
            'en' : 'Commit',
            'cn' : '提交'
            },
        'new' : {
            'en' : 'New',
            'cn' : '新建'
            },
        'save' : {
            'en' : 'Save',
            'cn' : '保存'
            },
        'cancel' : {
            'en' : 'Cancel',
            'cn' : '取消'
            },
        'switch' : {
            'en' : 'Switch',
            'cn' : '开关'
            },
        'start' : {
            'en' : 'Start',
            'cn' : '启动'
            },
        'stop' : {
            'en' : 'Stop',
            'cn' : '停止'
            },
        'confirm' : {
            'en' : 'Confirm',
            'cn' : '确定'
            },
        'filename' : {
            'en' : 'File Name:',
            'cn' : '文件名:'
            },
        'delete' : {
            'en' : 'Delete',
            'cn' : '删除'
            },
        'rulePrompt' : {
            'en' : '* Begin with rul-',
            'cn' : '以前缀rul-开头'
            },
        'sceneAuto' : {
            'en' : 'SceneAuto',
            'cn' : '场景联动'
            },
        'modeTrigger' : {
            'en' : 'Mode Trigger',
            'cn' : '模式触发'
            },
        'factTrigger' : {
            'en' : 'Fact Trigger',
            'cn' : '事实触发'
            },
        'fact' : {
            'en' : 'Fact',
            'cn' : '事实'
            },
        'class' : {
            'en' : 'Class',
            'cn' : '类'
            },
        'agenda' : {
            'en' : 'agenda',
            'cn' : '议程'
            },
        'global' : {
            'en' : 'Global',
            'cn' : '全局变量'
            },
        'printItem' : {
            'en' : 'Select Print',
            'cn' : '打印调试项'
            },
        'selectItem' : {
            'en' : 'Select Item',
            'cn' : '输出选择'
            },
        'instance' : {
            'en' : 'Instance',
            'cn' : '实例'
            },
        'rule' : {
            'en' : 'Rule',
            'cn' : '规则'
            },
        'mem' : {
            'en' : 'Memory',
            'cn' : '内存'
            },
        'assert' : {
            'en' : 'Assert',
            'cn' : '断言'
            },
        'comehome' : {
            'en' : 'Come Home',
            'cn' : '回家模式'
            },
        'leavehome' : {
            'en' : 'Leave Home',
            'cn' : '离家模式'
            },
        'entermovie' : {
            'en' : 'Enter Movie',
            'cn' : '进入观影'
            },
        'exitmovie' : {
            'en' : 'Exit Movie',
            'cn' : '退出观影'
            },
        'sleepmode' : {
            'en' : 'Sleep Mode',
            'cn' : '睡眠模式'
            },
        'getupmode' : {
            'en' : 'Getup Mode',
            'cn' : '起床模式'
            },
        }
