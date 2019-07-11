#ifndef SRC_GRPC_REMOTE_CLIENT_H_
#define SRC_GRPC_REMOTE_CLIENT_H_

#include "src/object.h"
#include "grpc++/grpc++.h"
#include "src/proto/remote_service.grpc.pb.h"

using grpc::ClientContext;
using com::ick::RemoteService;

class ick_api GrpcRemoteClient : IObject {
 public:
  GrpcRemoteClient(const std::string& server);
  ~GrpcRemoteClient();

  void RequestLogger(int count = 0);

 private:
  std::unique_ptr<RemoteService::Stub> stub_;
  grpc::ClientContext client_context_;
};

#endif