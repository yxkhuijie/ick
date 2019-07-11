#include "src/grpc_remote_client.h"
#include "gflags/gflags.h"

DEFINE_string(server, "127.0.0.1:8081", "remote service server (host:port)");
DEFINE_int32(logger_count, 0, "latest logger count");

int main(int argc, char** argv) {
  GrpcRemoteClient client(FLAGS_server);
  client.RequestLogger(FLAGS_logger_count);
}