/***************************************************************************
 *  UnitTest.cpp - Unit Test
 *
 *  Created: 2018-05-31 18:20:13
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Thread.h"
#include "SysTime.h"
#include "RingBuffer.h"
#include "StringArray.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace UTILS;

static RingBuffer *gRingBuffer = 0;

class MyThread : public Thread {
public:
    MyThread() { }
    MyThread(Runnable *r) : Thread(r) { }
    ~MyThread() { }
    void run();
};

void MyThread::run()
{
    std::cout << "MyThread run" << std::endl;
    sleep(2);
    std::cout << "MyThread end" << std::endl;
}

class MyRunnable : public Runnable {
public:
    MyRunnable() {}
    ~MyRunnable(){}
    void run();
};

void MyRunnable::run()
{
    std::cout << "MyRunnable run" << std::endl;
    uint32_t len = 0;
    char payload[64] = { 0 };
    uint8_t *bufPointer;
    uint32_t bufLength;
    for (int i = 0; i < 99; ++i) {
        len = sprintf(payload, "123456789A123456789A123456789A123456789A-%02d\n", i);
        int offset = 0;
        while(1) {
            gRingBuffer->getWriteHead(&bufPointer, &bufLength);
            if (bufLength >= len) {
                memcpy(bufPointer, payload+offset, len);
                printf("submitWrite[%d]\n", len);
                gRingBuffer->submitWrite(bufPointer, len);
                break;
            } else if (bufLength > 0) {
                memcpy(bufPointer, payload+offset, bufLength);
                printf("submitWrite[%d]\n", bufLength);
                gRingBuffer->submitWrite(bufPointer, bufLength);
                offset += bufLength;
                len -= bufLength;
            } else {
                printf("***Warning***\n");
                usleep(1000);
            }
        }
    }
    std::cout << "MyRunnable end" << std::endl;
}

void test_Thread()
{
    std::cout << "test_Thread: " << std::endl;
    MyThread *myThread = new MyThread();
    std::cout << "test_Thread: start thread" << std::endl;
    sleep(1);
    myThread->start();
    std::cout << "test_Thread: sleep 1" << std::endl;
    sleep(1);
    myThread->join();
    std::cout << "test_Thread: end!" << std::endl;
}

void test_SysTime()
{
    SysTime::DateTime dt;
    SysTime::GetDateTime(&dt);
    printf("%04d/%02d/%02d %02d:%02d:%02d\n",
        dt.mYear, dt.mMonth, dt.mDay,
        dt.mHour, dt.mMinute, dt.mSecond);
    std::cout << "Clock ms time: " << SysTime::GetMSecs() << std::endl;
}

void test_RingBuffer()
{
    uint8_t *buf = (uint8_t*)malloc(128);
    gRingBuffer = new RingBuffer(buf, 128);
    MyRunnable *myRunnable = new MyRunnable();
    MyThread *myThread = new MyThread(myRunnable);
    myThread->start();

    uint8_t *bufPointer;
    uint32_t bufLength;
    char payload[12800] = { 0 };
    int offset = 0;
    for (int i = 0; i < 99; ++i) {
        while (1) {
            usleep(1000);
            gRingBuffer->getReadHead(&bufPointer, &bufLength);
            printf("getReadLength[%d]\n", bufLength);
            if (bufLength == 0)
                break;
            if ((sizeof(payload) - offset) > bufLength) {
                memcpy(payload+offset, bufPointer, bufLength);
                offset += bufLength;
            }
            gRingBuffer->submitRead(bufPointer, bufLength);
        }
    }
    printf("\npayload = \n%s\n", payload);
    std::cout << "test_RingBuffer end" << std::endl;
}

void test_StringArray()
{
    StringArray data;
    data.put(0, "aa");
    data.put(1, "bb");
    data.put(10, "cc");

    printf("(0:%s)\n", data[0]);
    printf("(10:%s)\n", data.get(10));
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    test_StringArray();
    test_SysTime();
    test_Thread();
    test_RingBuffer();

    return sleep(1000);
}
