#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <nlohmann/json.hpp>
#include <grpcpp/grpcpp.h>
#include "grpc-client/submit_client.h"
#include <common.h>
#include <xlog/xlog.h>

using Json = nlohmann::json;
const int kGrpcPort = 50050;

int main(int argc, char *argv[]) {

    if (argc < 2) {
        LOG_INFO("Usage: ./client <command>, available command: *.json/stop/start/status/help/exit");
        return 1;
    }
    
    const std::string kFileSuffix = ".json";
    std::string slotus_arg = argv[1];

    if (slotus_arg.length() >= kFileSuffix.size() && slotus_arg.substr(slotus_arg.length() - kFileSuffix.size()) == kFileSuffix) {
        
        std::ifstream json_file;
        Json json_data;
        Job job;
        json_file.open(slotus_arg);
        json_data = Json::parse(json_file);
        json_file.close();

        std::string server_addr = "localhost:" + std::to_string(kGrpcPort);
        SubmitClient client(grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials()));  
        ConvertJsonToProto(json_data, job);
        client.SubmitJob(job);
        
    } else {
        LOG_INFO("Error: Unknown command, available command: *.json/stop/start/status/help/exit");
        return 1;
    }
   
    return 0;
}
