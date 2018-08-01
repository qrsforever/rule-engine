#!/usr/bin/python3
# -*- coding: utf-8 -*-

import json
from urllib.request import Request, urlopen
from urllib.error import URLError

device_list = []
rule_list = []

class DeviceInfo(object):

    """Docstring for DeviceInfo. """

    def __init__(self, deviceId, productName, aliasName):
        self._deviceId = deviceId
        self._productName = productName
        self._aliasName = aliasName

    def getId(self):
        return self._deviceId

    def getName(self):
        return self._productName

    def getAlias(self):
        return self._aliasName

class RuleInfo(object):

    """Docstring for RuleInfo. """

    def __init__(self, ruleid, rulename, enabled):
        self._ruleid = ruleid
        self._rulename = rulename
        self._enabled = enabled

    def getId(self):
        return self._ruleid

    def getName(self):
        return self._rulename


class ElinkRequest(object):

    """Docstring for RequestHeader. """

    def __init__(self, uri):
        self._ip = "10.185.30.96"
        self._host = "smarthome.lecloud.com"
        self._token = "238XXXvKm3oZNGEm1Sm2T6Udm2Wnr4nikdxi6fm1kfU4Pj17RxSDAv5B2n3m5RMitfRw0b6VGLjoVsJejE4z3B9Z0Bt1cm3ewDuBBuOwLkRrUxinrb3zGkwUm4"
        self._uri = uri

    def headers(self):
        return { 'Host' : self._host, 'Token' : self._token, 'Content-Type': 'application/json'}

    def url(self):
        return "http://" + self._ip + "/" + self._uri

    def curl(self):
        """
        for test
        """
        return "curl -v \"" + self.url() + "\" -H \"Host:" + self._host + " \" -H \"token:" + self._token + "\""

def request_devices_list(uri):
    print("request devices list info uri:", uri)
    elink = ElinkRequest(uri)
    # import os
    # os.system(elink.curl())
    # print(elink.curl())
    try:
        req = Request(elink.url(), headers=elink.headers(), method="POST")
        res = urlopen(req)
        data = json.loads(res.read().decode(encoding='UTF-8', errors='ignore'))
        if data['status'] != 1 :
            print("Sync fail status: ", data['status'])
            return

        with open('profiles/devices_list.json', 'wt') as f:
            #  print("response: ", data)
            f.write(json.dumps(data, indent=4, ensure_ascii=False))

        infos = data['result']['deviceInfos']
        for info in infos:
            device_list.append(
                    DeviceInfo(info['deviceId'], info['productName'], info['aliasName']))

    except URLError as e:
        if hasattr(e, "reason"):
            print("Reason: ", e.reason)
        elif hasattr(e, "code"):
            print("ErrCode: ", e.code)
        else:
            print("Unkown error!")

def request_device_profile(uri):
    print("request device profile uri:", uri)
    elink = ElinkRequest(uri)
    for device in device_list:
        try:
            data = json.dumps({'device_id':device.getId()}).encode('utf-8')
            req = Request(elink.url(), data = data, headers=elink.headers(), method="POST")
            res = urlopen(req)
            data = json.loads(res.read().decode(encoding='UTF-8', errors='ignore'))
            print("data = ", data)
            if data['status'] != 1 :
                print("Sync fail status: ", data['status'])
                return

            with open("profiles/"+device.getId()+".json", 'wt') as f:
                #  print("response: ", data)
                f.write(json.dumps(data, indent=4, ensure_ascii=False))

        except URLError as e:
            if hasattr(e, "reason"):
                print("Reason: ", e.reason)
            elif hasattr(e, "code"):
                print("ErrCode: ", e.code)
            else:
                print("Unkown error!")

def request_rules_list(uri):
    print("request rules list uri:", uri)
    elink = ElinkRequest(uri)
    try:
        req = Request(elink.url(), headers=elink.headers(), method="POST")
        res = urlopen(req)
        data = json.loads(res.read().decode(encoding='UTF-8', errors='ignore'))
        if data['status'] != 1 :
            print("Sync fail status: ", data['status'])
            return

        with open('rules/rules_list.json', 'wt') as f:
            #  print("response: ", data)
            f.write(json.dumps(data, indent=4, ensure_ascii=False))

        infos = data['result']['data']
        for info in infos:
            rule_list.append(
                    RuleInfo(info['ruleid'], info['rulename'], info['triggerenabled']))

    except URLError as e:
        if hasattr(e, "reason"):
            print("Reason: ", e.reason)
        elif hasattr(e, "code"):
            print("ErrCode: ", e.code)
        else:
            print("Unkown error!")

def request_rule_query(uri):
    print("request rule query uri:", uri)
    elink = ElinkRequest(uri)
    for rule in rule_list:
        try:
            data = json.dumps({'rule_id':rule.getId()}).encode('utf-8')
            req = Request(elink.url(), data = data, headers=elink.headers(), method="POST")
            res = urlopen(req)
            data = json.loads(res.read().decode(encoding='UTF-8', errors='ignore'))
            print("data = ", data)
            if data['status'] != 1 :
                print("Sync fail status: ", data['status'])
                return

            with open("rules/"+rule.getName()+".json", 'wt') as f:
                #  print("response: ", data)
                f.write(json.dumps(data, indent=4, ensure_ascii=False))

        except URLError as e:
            if hasattr(e, "reason"):
                print("Reason: ", e.reason)
            elif hasattr(e, "code"):
                print("ErrCode: ", e.code)
            else:
                print("Unkown error!")


def main():
    print("Sync resources from elink cloud.")

    request_devices_list('api/devices/list')

    request_device_profile('api/device/profile')

    request_rules_list('api/rule/list')

    request_rule_query('api/rule/query')


if __name__ == "__main__":
    main()
