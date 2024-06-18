#include "submit_client.h"
#include <iostream>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include <xlog/xlog.h>
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

SubmitClient::SubmitClient(std::shared_ptr<Channel> channel) 
    : stub_(SubmitService::NewStub(channel)) {}

void SubmitClient::SubmitJob(const Job& job) {
    
    JobStatus job_status;
    ClientContext context;

    std::unique_ptr<ClientAsyncResponseReader<JobStatus>> rpc(
        stub_->PrepareAsyncJobSubmit(&context, job, &cq_)
    );

    rpc->StartCall();//start async call
    rpc->Finish(&job_status, &status_, (void*)1);//Waiting for completion of async call

    void* client_tag = nullptr;
    bool ok = false;
    GPR_ASSERT(cq_.Next(&client_tag, &ok));//Waiting for completion event of async call, retrieve callback information from cq_
    GPR_ASSERT(client_tag == (void*)1);
    GPR_ASSERT(ok);

    if(status_.ok()) {
        LOG_INFO("Job submitted: %s", job.name().c_str());
        LOG_INFO("Job callback MSG: %s", job_status.status().c_str());
    } else {
        LOG_ERROR("status error");
    }
}

