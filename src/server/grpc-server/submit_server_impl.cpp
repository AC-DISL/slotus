#include "submit_server_impl.h"

#include <errno.h>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <malloc.h>
#include <stdio.h>
#include <xlog/xlog.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

#include "google/protobuf/stubs/stringpiece.h"
#include "google/protobuf/util/json_util.h"
#include "job.grpc.pb.h"
#include "job.pb.h"

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

SubmitServiceImpl::~SubmitServiceImpl() {
    server_->Shutdown();
    cq_->Shutdown();
}

void SubmitServiceImpl::Run(std::string nodeId) {
    ServerBuilder builder;
    std::string server_address(nodeId.c_str());
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    cq_     = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    LOG_INFO("Server listening on: %s ", server_address.c_str());
    std::string container_server_ip = "127.0.0.1:1999";
    HandleRpcs(container_server_ip);
}

void SubmitServiceImpl::HandleRpcs(std::string containerServerIp) {
    new AsyncCallData(&service_, cq_.get());
    void* tag = nullptr;
    bool ok   = false;

    while (true) {
        GPR_ASSERT(cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);

        static_cast<CallData*>(tag)->Proceed(containerServerIp);
    }
}

SubmitServiceImpl::AsyncCallData::AsyncCallData(
    SubmitService::AsyncService* service, ServerCompletionQueue* cq)
    : CallData(service, cq), responder_(&ctx_) {
    Proceed("");
}

void SubmitServiceImpl::AsyncCallData::ProtoToJson(Job& job, Json& json_data) {
    json_data["name"]          = job.name();
    json_data["files"]         = job.files();
    json_data["attemptMaxNum"] = job.attemptmaxnum();
    json_data["task"]["MetaData"]["name"] =
        job.mutable_task()->mutable_meta_data()->name();
    json_data["task"]["MetaData"]["submitter"] =
        job.mutable_task()->mutable_meta_data()->submitter();
    json_data["task"]["MetaData"]["address"] =
        job.mutable_task()->mutable_meta_data()->address();
    json_data["task"]["MetaData"]["now_time"] =
        job.mutable_task()->mutable_meta_data()->now_time();
    json_data["task"]["MetaData"]["start_time"] =
        job.mutable_task()->mutable_meta_data()->start_time();
    json_data["task"]["MetaData"]["extire_time"] =
        job.mutable_task()->mutable_meta_data()->extire_time();
    json_data["task"]["MetaData"]["parent_task"] =
        job.mutable_task()->mutable_meta_data()->parent_task();
    json_data["task"]["ResourceConf"]["vcores"] =
        job.mutable_task()->mutable_resource_conf()->vcores();
    json_data["task"]["ResourceConf"]["memoryMb"] =
        job.mutable_task()->mutable_resource_conf()->memorymb();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["namespace"] =
        job.mutable_task()
            ->mutable_runtime_conf()
            ->mutable_kubernetesnativeconf()
            ->namespace_();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["name"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->name();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["kind"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->kind();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["imageName"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->imagename();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["imagePullPolicy"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->imagepullpolicy();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["port"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->port();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["containerPort"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->containerport();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["targetPort"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->targetport();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["nodePort"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->nodeport();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["serviceType"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->servicetype();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["mounts"]["hostPath"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->mutable_mounts()
                     ->hostpath();
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["MainContainerConf"]["mounts"]["mountPath"] =
                 job.mutable_task()
                     ->mutable_runtime_conf()
                     ->mutable_kubernetesnativeconf()
                     ->mutable_executorpodconf()
                     ->mutable_maincontainerconf()
                     ->mutable_mounts()
                     ->mountpath();

    for (const auto& command : job.mutable_task()
                                   ->mutable_runtime_conf()
                                   ->mutable_kubernetesnativeconf()
                                   ->mutable_executorpodconf()
                                   ->mutable_maincontainerconf()
                                   ->command()) {
        json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]
                 ["ExecutorPodConf"]["MainContainerConf"]["command"]
                     .push_back(command);
    }

    for (const auto& arg : job.mutable_task()
                               ->mutable_runtime_conf()
                               ->mutable_kubernetesnativeconf()
                               ->mutable_executorpodconf()
                               ->mutable_maincontainerconf()
                               ->args()) {
        json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]
                 ["ExecutorPodConf"]["MainContainerConf"]["args"]
                     .push_back(arg);
    }
    json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]
             ["replicas"] = job.mutable_task()
                                ->mutable_runtime_conf()
                                ->mutable_kubernetesnativeconf()
                                ->mutable_executorpodconf()
                                ->replicas();

    auto* container_env_map = job.mutable_task()
                                  ->mutable_runtime_conf()
                                  ->mutable_containerenv()
                                  ->mutable_env();
    for (const auto& entry : *container_env_map) {
        const std::string& env_name  = entry.first;
        const std::string& env_value = entry.second;
        json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]
                 ["ExecutorPodConf"]["MainContainerConf"]["ContainerEnv"]
                 [env_name] = env_value;
    }

    json_data["task"]["InputData"]["name"] =
        job.mutable_task()->mutable_input_data()->name();
    json_data["task"]["InputData"]["inputPath"] =
        job.mutable_task()->mutable_input_data()->mutable_spec()->inputpath();
    json_data["task"]["InputData"]["inputName"] =
        job.mutable_task()->mutable_input_data()->mutable_spec()->inputname();
    json_data["task"]["InputData"]["textInput"] =
        job.mutable_task()->mutable_input_data()->mutable_spec()->textinput();
    json_data["task"]["OutputData"]["name"] =
        job.mutable_task()->mutable_output_data()->name();
    json_data["task"]["OutputData"]["outputPath"] =
        job.mutable_task()->mutable_output_data()->mutable_spec()->outputpath();
    json_data["task"]["OutputData"]["outputName"] =
        job.mutable_task()->mutable_output_data()->mutable_spec()->outputpath();
    json_data["task"]["OutputData"]["textOutput"] =
        job.mutable_task()->mutable_output_data()->mutable_spec()->outputpath();
}

void SubmitServiceImpl::AsyncCallData::Proceed(std::string containerServerIp) {
    LOG_INFO("this: %p status: %d", this, status_);

    if (status_ == CREATE) {
        LOG_INFO("this: %p status: CREATE", this);
        status_ = PROCESS;
        service_->RequestJobSubmit(&ctx_, &job_, &responder_, cq_, cq_,
            this);  // handle submit of client

    } else if (status_ == PROCESS) {
        LOG_INFO("this: %p status: PROCESS", this);
        LOG_INFO("Start to create k3s pod: %s",
            job_.mutable_task()->mutable_meta_data()->name().c_str());
        new AsyncCallData(service_, cq_);  // create new asynccalldata
        status_ = FINISH;

        Json json_data;
        ProtoToJson(job_, json_data);

        // callback test
        job_status_.set_status("callback test");
        responder_.Finish(job_status_, Status::OK, this);

    } else {
        LOG_INFO("this: %p status: FINISH", this);
        GPR_ASSERT(status_ == FINISH);
        delete this;
    }
}