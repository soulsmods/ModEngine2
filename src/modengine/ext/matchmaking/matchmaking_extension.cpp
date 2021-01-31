#pragma once

#include "Frpg2RequestMessage.pb.h"
#include "matchmaking_extension.h"

namespace modengine::ext {

std::shared_ptr<Hook<fpProtobufDeserialize>> hooked_ProtobufDeserialize;

void* protobuf_deserialize(void* msg, unsigned char *buffer, size_t size)
{
    return hooked_ProtobufDeserialize->original(msg, buffer, size);
}

void MatchmakingExtension::on_attach()
{
    hooked_ProtobufDeserialize = register_hook(DS3, 0x141b5ea10, protobuf_deserialize);
}

void MatchmakingExtension::on_detach()
{
}

}