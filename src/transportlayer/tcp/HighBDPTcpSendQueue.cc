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

#include "HighBDPTcpSendQueue.h"

namespace inet {

namespace tcp {

Register_Class(HighBDPTcpSendQueue);

void HighBDPTcpSendQueue::enqueueAppData(Packet *msg)
{
    dataBuffer.push(msg->peekDataAt(B(0), msg->getDataLength()));
    end += msg->getByteLength();
    std::cout << "\n beginValue: " << begin << " endValue: " << end << endl;
    if (seqLess(end, begin)){
        throw cRuntimeError("Send queue is full");
    }
    delete msg;
}

}
}

