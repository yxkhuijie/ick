#include "src/grpc_remote_service.h"

#include <unistd.h>
#include "src/logger.h"

GrpcRemoteService::GrpcRemoteService() {}

GrpcRemoteService::~GrpcRemoteService() {}

Status GrpcRemoteService::RequestService(
    ServerContext* context,
    const ServiceRequest* request,
    ServiceResponse* response) {
  return Status::OK;
}

Status GrpcRemoteService::RequestLogger(
    ServerContext* context,
    const LoggerRequest* request,
    ServerWriter<LoggerResponse>* writer) {
  int count = request->count();
  Logger::getInstance()->Info("receive logger request: count: " + std::to_string(count));
  std::list<std::string> logger_list;
  GrpcLoggerSubscriber subscriber;
  Logger::getInstance()->getHistoryLog(&logger_list, count, &subscriber);
  for (const auto& it : logger_list) {
    LoggerResponse logger_response;
    logger_response.set_message(it);
    writer->Write(logger_response);
  }
  
  while(true) {
    if (context->IsCancelled()) {
      Logger::getInstance()->Info("client is cancelled");
      break;
    }
    std::list<LoggerMessage> logger_message_list;
    subscriber.getLoggerMessage(&logger_message_list);
    if (!logger_message_list.empty()) {
      for (auto& it : logger_message_list) {
        LoggerResponse logger_response;
        logger_response.set_message(it.toString(false));
        writer->Write(logger_response);
      }
    }
    usleep(100000);
  }
  Logger::getInstance()->cancelLoggerSubscriber(&subscriber);
  return Status::OK;
}

GrpcRemoteServiceThread::GrpcRemoteServiceThread()
    : listening_port_("0.0.0.0:8090") {}

GrpcRemoteServiceThread::~GrpcRemoteServiceThread() {}

void GrpcRemoteServiceThread::setListeningPort(const std::string& listening_port) {
  this->listening_port_ = listening_port;
}

void GrpcRemoteServiceThread::execute() {
  GrpcRemoteService service;
  std::string server_address(this->listening_port_);
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  builder.SetMaxMessageSize(1024 * 1024 * 1024);  // 1024 MB maximum payload
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  Logger::getInstance()->Info("grpc remote service server listening on: " + this->listening_port_);
  server->Wait();
}

void GrpcLoggerSubscriber::update(const std::list<LoggerMessage>& logger_message_list) {
  mutex_.lock();
  logger_message_list_.insert(logger_message_list_.end(), logger_message_list.begin(), logger_message_list.end());
  mutex_.unlock();
}

void GrpcLoggerSubscriber::getLoggerMessage(std::list<LoggerMessage>* logger_message_list) {
  if (logger_message_list == nullptr) return;
  mutex_.lock();
  logger_message_list->assign(logger_message_list_.begin(), logger_message_list_.end());
  logger_message_list_.clear();
  mutex_.unlock();
}
