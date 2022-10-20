//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/packet/chunk/BytesChunk.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "TcpLargeSessionApp.h"

namespace inet {

Define_Module(TcpLargeSessionApp);

#define MSGKIND_CONNECT    1
#define MSGKIND_SEND       2
#define MSGKIND_CLOSE      3

TcpLargeSessionApp::~TcpLargeSessionApp()
{
    cancelAndDelete(timeoutMsg);
}

void TcpLargeSessionApp::initialize(int stage)
{
    TcpLargeAppBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        activeOpen = par("active");
        tOpen = par("tOpen");
        tSend = par("tSend");
        tClose = par("tClose");
        sendBytes = par("sendBytes");
        commandIndex = 0;
        timeoutMsg = new cMessage("timer");
        tcpMod = dynamic_cast<tcp::HighBDPTcp*>(getParentModule()->getSubmodule("tcp"));
    }
}

void TcpLargeSessionApp::handleStartOperation(LifecycleOperation *operation)
{
    if (simTime() <= tOpen) {
        timeoutMsg->setKind(MSGKIND_CONNECT);
        scheduleAt(tOpen, timeoutMsg);
    }
}

void TcpLargeSessionApp::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(timeoutMsg);
    if (socket.isOpen())
        close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void TcpLargeSessionApp::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(timeoutMsg);
    if (operation->getRootModule() != getContainingNode(this))
        socket.destroy();
}

void TcpLargeSessionApp::handleTimer(cMessage *msg)
{
    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            if (activeOpen)
                connect(); // sending will be scheduled from socketEstablished()
            else
                throw cRuntimeError("TODO");
            break;

        case MSGKIND_SEND:
            sendData();
            break;

        case MSGKIND_CLOSE:
            close();
            break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void TcpLargeSessionApp::sendData()
{
    uint32_t numBytes = 1000000000;
    std::cout << "\n size of uint32_t: " << sizeof(uint32_t) << endl;
    std::cout << "\n Bytes remaining in send queue: " << tcpMod->getSendQueueBytesAvailable(socket.getSocketId()) << endl;
    EV_INFO << "sending data with " <<  tcpMod->getSendQueueBytesAvailable(socket.getSocketId())-1 << " bytes\n";

    if(tcpMod->getSendQueueBytesAvailable(socket.getSocketId())-1 > 0){
        sendPacket(createDataPacket(tcpMod->getSendQueueBytesAvailable(socket.getSocketId())-1));
    }

    if(bytesSent < sendBytes){
        std::cout << "\nSending next data chunk" << endl;
        scheduleAt(simTime() + SimTime(200,SIMTIME_MS), timeoutMsg);
    }
    else {
        timeoutMsg->setKind(MSGKIND_CLOSE);
        scheduleAt(std::max(tClose, simTime()), timeoutMsg);
    }
}

Packet *TcpLargeSessionApp::createDataPacket(long sendBytes)
{
    const char *dataTransferMode = par("dataTransferMode");
    Ptr<Chunk> payload;
    if (!strcmp(dataTransferMode, "bytecount")) {
        payload = makeShared<ByteCountChunk>(B(sendBytes));
    }
    else if (!strcmp(dataTransferMode, "object")) {
        const auto& applicationPacket = makeShared<ApplicationPacket>();
        applicationPacket->setChunkLength(B(sendBytes));
        payload = applicationPacket;
    }
    else if (!strcmp(dataTransferMode, "bytestream")) {
        const auto& bytesChunk = makeShared<BytesChunk>();
        std::vector<uint8_t> vec;
        vec.resize(sendBytes);
        for (int i = 0; i < sendBytes; i++)
            vec[i] = (bytesSent + i) & 0xFF;
        bytesChunk->setBytes(vec);
        payload = bytesChunk;
    }
    else
        throw cRuntimeError("Invalid data transfer mode: %s", dataTransferMode);
    payload->addTag<CreationTimeTag>()->setCreationTime(simTime());
    Packet *packet = new Packet("data1");
    packet->insertAtBack(payload);
    return packet;
}

void TcpLargeSessionApp::socketEstablished(TcpSocket *socket)
{
    TcpLargeAppBase::socketEstablished(socket);

    ASSERT(commandIndex == 0);
    timeoutMsg->setKind(MSGKIND_SEND);
    scheduleAt(std::max(tSend, simTime()), timeoutMsg);
}

void TcpLargeSessionApp::socketDataArrived(TcpSocket *socket, Packet *msg, bool urgent)
{
    TcpLargeAppBase::socketDataArrived(socket, msg, urgent);
}

void TcpLargeSessionApp::socketClosed(TcpSocket *socket)
{
    TcpLargeAppBase::socketClosed(socket);
    cancelEvent(timeoutMsg);
    if (operationalState == State::STOPPING_OPERATION && !this->socket.isOpen())
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void TcpLargeSessionApp::socketFailure(TcpSocket *socket, int code)
{
    TcpLargeAppBase::socketFailure(socket, code);
    cancelEvent(timeoutMsg);
}

void TcpLargeSessionApp::finish()
{
    EV << getFullPath() << ": received " << bytesRcvd << " bytes in " << packetsRcvd << " packets\n";
    recordScalar("bytesRcvd", bytesRcvd);
    recordScalar("bytesSent", bytesSent);
}

void TcpLargeSessionApp::refreshDisplay() const
{
    TcpLargeAppBase::refreshDisplay();

    std::ostringstream os;
    os << TcpSocket::stateName(socket.getState()) << "\nsent: " << bytesSent << " bytes\nrcvd: " << bytesRcvd << " bytes";
    getDisplayString().setTagArg("t", 0, os.str().c_str());
}

} // namespace inet

