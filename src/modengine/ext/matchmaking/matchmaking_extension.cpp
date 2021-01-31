#pragma once

#include "proto/Frpg2RequestMessage.pb.h"
#include "matchmaking_extension.h"

#include <map>
#include <cerrno>

namespace modengine::ext {

#include "matchmaking_protobuf_vtables.h"

std::atomic_long counter;
std::shared_ptr<Hook<fpProtobufDeserialize>> hooked_ProtobufDeserialize;

bool protobuf_deserialize(void* msg, unsigned char* buffer, size_t size)
{
    uintptr_t vtable_addr = *(uintptr_t*)msg;
    std::string kind = vtable_map[vtable_addr];

    std::ofstream out;
    std::string name = "proto/" + std::to_string(counter.fetch_add(1)) + "_" + kind + ".raw";
    out.open(name, std::ofstream::out | std::ofstream::binary);

    if (out.fail()) {
        error("Error trying to write {}: {}", name, std::strerror(errno));
    }

    out.write((const char*)buffer, size);
    out.flush();
    out.close();

    if (kind == "Frpg2RequestMessage__RequestGetAnnounceMessageListResponse") {
        auto announcement_list = Frpg2RequestMessage::RequestGetAnnounceMessageListResponse();

        announcement_list.ParseFromArray(buffer, (int)size);
        announcement_list.clear_data_list();

        auto data_ser = announcement_list.SerializeAsString();

        return hooked_ProtobufDeserialize->original(msg, (unsigned char*)data_ser.c_str(), data_ser.size());
    }

    return hooked_ProtobufDeserialize->original(msg, buffer, (int)size);
}

void MatchmakingExtension::on_attach()
{
    hooked_ProtobufDeserialize = register_hook(DS3, 0x141b5ea10, protobuf_deserialize);
}

void MatchmakingExtension::on_detach()
{
}

}