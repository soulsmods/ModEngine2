#include "grpc_server_extension.h"
#include "modengine/version.h"

#include "mapserver.grpc.pb.h"

#include <memory>
#include <Windows.h>

namespace modengine::ext {

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class ModEngine2ServiceImpl final : public ModEngine2Service::Service 
{
    Status GetRuntimeInformation(ServerContext* , const RuntimeInformationRequest* /*request*/, RuntimeInformationReply* reply) override 
    {
        info("Got initial connection from new client");
        reply->set_version(std::string(g_version));
        reply->set_gametype(modengine::RuntimeInformationReply_GameType::RuntimeInformationReply_GameType_DS3);
        return Status::OK;
    }
};

DWORD WINAPI GRPCWorkerThread(GRPCServerExtension *extension)
{
    extension->worker_thread_entry();
    return 0;
}

void GRPCServerExtension::worker_thread_entry()
{
    std::string server_address(MODENGINE_GRPC_SERVER_ADDRESS);
    ModEngine2ServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    auto server = builder.BuildAndStart();
    info("Server listening on {}", server_address);
    server->Wait();
}

void GRPCServerExtension::on_attach()
{
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GRPCWorkerThread, NULL, NULL, NULL);
}

void GRPCServerExtension::on_detach()
{
}

}
