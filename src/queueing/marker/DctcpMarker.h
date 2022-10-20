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

#ifndef QUEUEING_DCTCPMARKER_H_
#define QUEUEING_DCTCPMARKER_H_

#include "inet/common/ModuleRef.h"
#include "inet/common/packet/Packet.h"
#include "inet/queueing/base/PacketFilterBase.h"
#include "inet/queueing/contract/IPacketCollection.h"

namespace inet {
namespace queueing {

/**
 * Implementation of DCTCP Marking.
 */
class INET_API DctcpMarker : public PacketFilterBase, public cListener
{
protected:
    enum DctcpMarkerResult { QUEUE_FULL, MARK, IGNORE};

  protected:
    int kthresh = 0;
    int packetCapacity = -1;
    bool useEcn = false;
    mutable DctcpMarkerResult lastResult;
    ModuleRef<IPacketCollection> collection;

  protected:
    virtual void initialize(int stage) override;

    virtual DctcpMarkerResult inspectQueue(const Packet *packet);

    virtual void processPacket(Packet *packet) override;

    // cListener:
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;

  public:
    virtual bool matchesPacket(const Packet *packet) const override;
};

} // namespace queueing
} // namespace inet

#endif

