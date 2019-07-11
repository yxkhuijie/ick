#ifndef SRC_GRPC_REMOTE_SERVICE_H_
#define SRC_GRPC_REMOTE_SERVICE_H_

#include "grpc++/grpc++.h"

#include "src/proto/remote_service.grpc.pb.h"
#include "src/thread.h"
#include "src/mutex.h"
#include "src/logger.h"

using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using com::ick::RemoteService;
using com::ick::ServiceRequest;
using com::ick::ServiceResponse;
using com::ick::LoggerRequest;
using com::ick::LoggerResponse;

class GrpcRemoteService final : public RemoteService::Service {
public:
  GrpcRemoteService();
  virtual ~GrpcRemoteService();

  Status RequestService(
      ServerContext* context,
      const ServiceRequest* request,
      ServiceResponse* response) override;

  Status RequestLogger(
      ServerContext* context,
      const LoggerRequest* request,
      ServerWriter<LoggerResponse>* writer) override;
};

class GrpcRemoteServiceThread : public IThread {
 public:
  GrpcRemoteServiceThread();
  ~GrpcRemoteServiceThread();

  void setListeningPort(const std::string& address);
  virtual void execute();

 private:
  std::string listening_port_;
};

class GrpcLoggerSubscriber : public LoggerSubscriber {
 public:
  virtual void update(const std::list<LoggerMessage>& logger_message_list);
  void getLoggerMessage(std::list<LoggerMessage>* logger_message_list);

 private:
  std::list<LoggerMessage> logger_message_list_;
  IMutex mutex_;
};

#endif  // SRC_GRPC_REMOTE_SERVICE_H_
