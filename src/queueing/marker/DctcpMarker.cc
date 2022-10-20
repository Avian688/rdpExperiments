//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "DctcpMarker.h"
#include "inet/common/ModuleAccess.h"
#include "inet/queueing/marker/EcnMarker.h"

namespace inet {
namespace queueing {

Define_Module(DctcpMarker);

void DctcpMarker::initialize(int stage)
{
    PacketFilterBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        kthresh = par("k");
        if (kthresh < 0.0)
            throw cRuntimeError("k parameter must not be negative");
        useEcn = par("useEcn");
        packetCapacity = par("packetCapacity");
        auto outputGate = gate("out");
        collection.reference(outputGate, false);
        if (!collection)
            collection.reference(this, "collectionModule", true);
        cModule * cm = check_and_cast<cModule *>(collection.get());
        cm->subscribe(packetPushedSignal, this);
        cm->subscribe(packetPulledSignal, this);
        cm->subscribe(packetRemovedSignal, this);
        cm->subscribe(packetDroppedSignal, this);
    }
}

DctcpMarker::DctcpMarkerResult DctcpMarker::inspectQueue(const Packet *packet)
{
    int queueLength = collection->getNumPackets();


    if (queueLength >= packetCapacity) {
        EV_DEBUG << "Queue length >= capacity" << EV_FIELD(queueLength) << EV_FIELD(packetCapacity) << EV_ENDL;
        return QUEUE_FULL;
    }
    else if(queueLength > kthresh){
        return MARK;
    }

    return IGNORE;
}

bool DctcpMarker::matchesPacket(const Packet *packet) const
{
    lastResult = const_cast<DctcpMarker *>(this)->inspectQueue(packet);
    switch (lastResult) {
        case  IGNORE:
            return true;
        case MARK:
            return useEcn;
        case QUEUE_FULL:
            return false;
        default:
            throw cRuntimeError("Unknown DctcpMarker result");
    }
}

void DctcpMarker::processPacket(Packet *packet)
{
    switch (lastResult) {
        case MARK:
            if(useEcn) {
                EcnMarker::setEcn(packet, IP_ECN_CE);
            }
        case IGNORE:
        case QUEUE_FULL:
            break;
        default:
            throw cRuntimeError("Unknown DctcpMarker result");
    }
}

void DctcpMarker::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details)
{
}

} // namespace queueing
} // namespace inet

