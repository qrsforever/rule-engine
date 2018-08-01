/***************************************************************************
 *  DataSink.cpp - Data Sink Impl
 *
 *  Created: 2018-06-04 10:00:42
 *
 *  Copyright QRS
 ****************************************************************************/

#include "DataSink.h"

namespace UTILS {

DataSink::DataSink()
    : mRingBuffer(0)
{

}

DataSink::~DataSink()
{

}

bool DataSink::onStart()
{
    return true;
}

bool DataSink::onDataArrive()
{
    return true;
}

bool DataSink::onError()
{
    return true;
}

bool DataSink::onEnd()
{
    return true;
}

} /* namespace UTILS */
