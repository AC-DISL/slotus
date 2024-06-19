#ifndef SLOTUS_SRC_SERVER_GRPC_SERVER_SUBMIT_SERVER_IMPL_H
#define SLOTUS_SRC_SERVER_GRPC_SERVER_SUBMIT_SERVER_IMPL_H

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include "google/protobuf/stubs/stringpiece.h"
#include "google/protobuf/util/json_util.h"
#include <nlohmann/json.hpp>

#include "job.pb.h"
#include "job.grpc.pb.h"


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using slotus::Job;
using slotus::JobStatus;
using slotus::SubmitService;

using Json = nlohmann::json;

class SubmitServiceImpl final {
public:
    ~SubmitServiceImpl();

    void Run(std::string nodeId);

private:
    class CallData{
    public:
        CallData(SubmitService::AsyncService* service, ServerCompletionQueue* cq)
            :service_(service), cq_(cq), status_(CREATE){
            Proceed("");
        }
        virtual ~CallData(){}
        virtual void Proceed(std::string containerServerIp = NULL) {
            return;
        }

        SubmitService::AsyncService* service_;
        ServerCompletionQueue *cq_;
        ServerContext ctx_;
        enum CallStatus{CREATE, PROCESS, FINISH};
        CallStatus status_;

    };

    class AsyncCallData : public CallData {
    public:
        AsyncCallData(SubmitService::AsyncService* service, ServerCompletionQueue* cq);

        ~AsyncCallData(){}

        void ProtoToJson(Job& job, Json& json_data);

        void Proceed(std::string containerServerIp = NULL) override;

    private:
        Job job_;
        JobStatus job_status_;
        ServerAsyncResponseWriter<JobStatus> responder_;
    };

    void HandleRpcs(std::string containerServerIp);

    std::unique_ptr<ServerCompletionQueue> cq_;
    SubmitService::AsyncService service_;
    std::unique_ptr<Server> server_;
};

#endif