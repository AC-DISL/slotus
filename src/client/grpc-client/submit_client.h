#ifndef SLOTUS_SRC_CLIENT_GRPC_CLIENT_SUBMIT_CLIENT_H
#define SLOTUS_SRC_CLIENT_GRPC_CLIENT_SUBMIT_CLIENT_H

#include <memory>
#include <nlohmann/json.hpp>
#include <grpcpp/grpcpp.h>
#include <protobuf/job.pb.h>
#include <protobuf/job.grpc.pb.h>

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using slotus::SubmitService;
using slotus::Job;
using slotus::JobStatus;
using Json = nlohmann::json;

class SubmitClient {
public:
    SubmitClient(std::shared_ptr<Channel> channel);

    void SubmitJob(const Job& job);

private:
    std::unique_ptr<SubmitService::Stub> stub_;
    CompletionQueue cq_;
    Status status_;
};

#endif  // SUBMIT_CLIENT_H