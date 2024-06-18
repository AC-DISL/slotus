#ifndef SLOTUS_SRC_UTILS_COMMON_H
#define SLOTUS_SRC_UTILS_COMMON_H

#include <nlohmann/json.hpp>
#include "protobuf/job.pb.h"
#include "protobuf/job.grpc.pb.h"

using slotus::Job;
using Json = nlohmann::json;

void ConvertJsonToProto(const Json& json_data, Job& job);

#endif  // SUBMIT_CLIENT_H



