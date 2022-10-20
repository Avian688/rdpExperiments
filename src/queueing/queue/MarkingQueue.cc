//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <inet/queueing/marker/EcnMarker.h>

#include "MarkingQueue.h"

namespace inet {
namespace queueing {

Define_Module(MarkingQueue);

void MarkingQueue::initialize(int stage)
{
    PacketQueue::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        kthresh = par("kthresh");
    }
}

void MarkingQueue::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");
    take(packet);
    cNamedObject packetPushStartedDetails("atomicOperationStarted");
    emit(packetPushStartedSignal, packet, &packetPushStartedDetails);
    EV_INFO << "Pushing packet" << EV_FIELD(packet) << EV_ENDL;
    queue.insert(packet);
    if (buffer != nullptr)
       buffer->addPacket(packet);
    else if (packetDropperFunction != nullptr) {
       while (isOverloaded()) {
           auto packet = packetDropperFunction->selectPacket(this);
           EV_INFO << "Dropping packet" << EV_FIELD(packet) << EV_ENDL;
           queue.remove(packet);
           dropPacket(packet, QUEUE_OVERFLOW);
       }
    }

    if(packetCapacity != -1 && getNumPackets() > kthresh){
        IpEcnCode ecn = EcnMarker::getEcn(packet);
        if (ecn != IP_ECN_NOT_ECT) {
            EcnMarker::setEcn(packet, IP_ECN_CE);
        }
    }

    ASSERT(!isOverloaded());
    if (collector != nullptr && getNumPackets() != 0)
       collector->handleCanPullPacketChanged(outputGate->getPathEndGate());
    cNamedObject packetPushEndedDetails("atomicOperationEnded");
    emit(packetPushEndedSignal, nullptr, &packetPushEndedDetails);
    updateDisplayString();
}

} // namespace queueing
} // namespace inet

