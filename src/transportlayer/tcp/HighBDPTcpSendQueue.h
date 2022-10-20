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

#ifndef TRANSPORTLAYER_TCP_HIGHBDPTCPSENDQUEUE_H_
#define TRANSPORTLAYER_TCP_HIGHBDPTCPSENDQUEUE_H_

#include <inet/transportlayer/tcp/TcpSendQueue.h>

namespace inet {
namespace tcp {

/**
 * High BDP Send queue that manages Chunks.
 *
 * @see TcpSendQueue
 */
class INET_API HighBDPTcpSendQueue : public TcpSendQueue
{
public:
    virtual void enqueueAppData(Packet *msg) override;
};

}
}

#endif /* TRANSPORTLAYER_TCP_HIGHBDPTCPSENDQUEUE_H_ */
