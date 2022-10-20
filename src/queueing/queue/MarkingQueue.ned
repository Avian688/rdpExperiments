//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package rdpexperiments.queueing.queue;

import inet.queueing.queue.PacketQueue;

simple MarkingQueue extends PacketQueue
{
    parameters:
        @class("inet::queueing::MarkingQueue");
        int kthresh = default(40);
        packetCapacity = default(100);
        dropperClass = default("inet::queueing::PacketAtCollectionEndDropper");
}
