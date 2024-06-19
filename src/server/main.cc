#include "grpc-server/submit_server_impl.h"
#include <xlog/xlog.h>

int main(int argc, char *argv[]) {

    xLogInit("slotus_server.log");

    SubmitServiceImpl server;
    server.Run("0.0.0.0:50050");

    return 0;
}
