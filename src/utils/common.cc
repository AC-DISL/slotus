#include <iostream>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "common.h"

using Json = nlohmann::json;

void ConvertJsonToProto(const Json& json_data, Job& job) {

    job.set_name(static_cast<const std::string&>(json_data["name"]));
    job.set_files(static_cast<const std::string&>(json_data["files"]));
    job.set_attemptmaxnum(static_cast<const std::int32_t&>(json_data["attemptMaxNum"]));
    job.mutable_task()->mutable_meta_data()->set_name(static_cast<const std::string&>(json_data["task"]["MetaData"]["name"]));
    job.mutable_task()->mutable_meta_data()->set_submitter(static_cast<const std::string&>(json_data["task"]["MetaData"]["submitter"]));
    job.mutable_task()->mutable_meta_data()->set_address(static_cast<const std::string&>(json_data["task"]["MetaData"]["address"]));
    job.mutable_task()->mutable_meta_data()->set_now_time(static_cast<const std::string&>(json_data["task"]["MetaData"]["now_time"]));
    job.mutable_task()->mutable_meta_data()->set_start_time(static_cast<const std::string&>(json_data["task"]["MetaData"]["start_time"]));
    job.mutable_task()->mutable_meta_data()->set_extire_time(static_cast<const std::string&>(json_data["task"]["MetaData"]["extire_time"]));
    job.mutable_task()->mutable_meta_data()->set_parent_task(static_cast<const std::string&>(json_data["task"]["MetaData"]["parent_task"]));
    job.mutable_task()->mutable_resource_conf()->set_vcores(static_cast<const std::float_t&>(json_data["task"]["ResourceConf"]["vcores"]));
    job.mutable_task()->mutable_resource_conf()->set_memorymb(static_cast<const std::int32_t&>(json_data["task"]["ResourceConf"]["memoryMb"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->set_namespace_(static_cast<const std::string&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["namespace"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_name(static_cast<const std::string&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["name"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_kind(static_cast<const std::string&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["kind"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_imagename(static_cast<std::string>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["imageName"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_imagepullpolicy(static_cast<const std::string&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["imagePullPolicy"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_port(static_cast<const std::int32_t&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["port"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_containerport(static_cast<const std::int32_t&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["containerPort"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_targetport(static_cast<const std::int32_t&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["targetPort"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_nodeport(static_cast<const std::int32_t&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["nodePort"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->set_servicetype(static_cast<const std::string&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["serviceType"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->mutable_mounts()->set_hostpath(static_cast<const std::string&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["Mounts"]["hostPath"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->mutable_mounts()->set_mountpath(static_cast<const std::string&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["Mounts"]["mountPath"]));
    job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->set_replicas(static_cast<const std::int32_t&>(json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["replicas"]));
  
    const auto& command_array = json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["command"];  
    for (const auto& command : command_array) {
        std::string command_str = command;
        job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->add_command(command_str);
    }

    const auto& args_array = json_data["task"]["RuntimeConf"]["KubernetesNativeConf"]["ExecutorPodConf"]["MainContainerConf"]["args"];
    for (const auto& arg : args_array) {
        std::string arg_str = arg;
        job.mutable_task()->mutable_runtime_conf()->mutable_kubernetesnativeconf()->mutable_executorpodconf()->mutable_maincontainerconf()->add_args(arg_str);
    }

    const auto& container_env = json_data["task"]["RuntimeConf"]["ContainerEnv"];
    auto* container_env_map = job.mutable_task()->mutable_runtime_conf()->mutable_containerenv()->mutable_env();
    for (auto it = container_env.begin(); it != container_env.end(); ++it) {
        const std::string& key = it.key();
        const std::string& value = it.value();
        (*container_env_map)[key] = value;
    }
    
    job.mutable_task()->mutable_input_data()->set_name(static_cast<const std::string&>(json_data["task"]["InputData"]["name"]));
    job.mutable_task()->mutable_input_data()->mutable_spec()->set_inputpath(static_cast<const std::string&>(json_data["task"]["InputData"]["spec"]["inputPath"]));
    job.mutable_task()->mutable_input_data()->mutable_spec()->set_inputname(static_cast<const std::string&>(json_data["task"]["InputData"]["spec"]["inputName"]));
    job.mutable_task()->mutable_input_data()->mutable_spec()->set_textinput(static_cast<const std::string&>(json_data["task"]["InputData"]["spec"]["textInput"]));
    job.mutable_task()->mutable_output_data()->set_name(static_cast<const std::string&>(json_data["task"]["OutputData"]["name"]));
    job.mutable_task()->mutable_output_data()->mutable_spec()->set_outputpath(static_cast<const std::string&>(json_data["task"]["OutputData"]["spec"]["outputPath"]));
    job.mutable_task()->mutable_output_data()->mutable_spec()->set_outputname(static_cast<const std::string&>(json_data["task"]["OutputData"]["spec"]["outputName"]));
    job.mutable_task()->mutable_output_data()->mutable_spec()->set_textoutput(static_cast<const std::string&>(json_data["task"]["OutputData"]["spec"]["textOutput"]));
    
}

