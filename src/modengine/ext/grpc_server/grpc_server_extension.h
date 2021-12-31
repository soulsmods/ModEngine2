#pragma once

#include "modengine/mod_engine.h"
#include "modengine/extension.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifndef MODENGINE_GRPC_SERVER_ADDRESS
#define MODENGINE_GRPC_SERVER_ADDRESS "localhost:30051"
#endif

namespace modengine {
namespace ext {

class GRPCServerExtension : public ModEngineExtension {
public:
    GRPCServerExtension(const std::shared_ptr<ModEngine>& instance)
        : ModEngineExtension(instance)
    {
    }

    void worker_thread_entry();

private:
    std::unique_ptr<grpc::Server> m_server;

    void on_attach() override;
    void on_detach() override;

    std::string id() override {
        return "grpc_server";
    }
};

}
}