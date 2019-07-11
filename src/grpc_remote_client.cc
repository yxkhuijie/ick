#include "src/grpc_remote_client.h"

#include "src/logger.h"

using com::ick::LoggerRequest;
using com::ick::LoggerResponse;

using grpc::ClientReader;

GrpcRemoteClient::GrpcRemoteClient(const std::string& server) {
  stub_ = RemoteService::NewStub(grpc::CreateChannel(
      server, grpc::InsecureChannelCredentials()));
  Logger::getInstance()->Info("set remote service server: " + server);
}

GrpcRemoteClient::~GrpcRemoteClient() {}

void GrpcRemoteClient::RequestLogger(int count /* = 0 */) {
  LoggerRequest logger_request;
  logger_request.set_count(count);
  ClientContext client_context;
  std::shared_ptr<ClientReader<LoggerResponse> >
      client_reader(stub_->RequestLogger(&client_context, logger_request));
  LoggerResponse logger_response;
  while(client_reader->Read(&logger_response)) {
  	if (logger_response.has_error()) {
  	  Logger::getInstance()->Error("request logger error: " + std::to_string(logger_response.error().code()) + ", " + logger_response.error().message());
  	  break;
  	}
  	std::string logger_message = logger_response.message();
  	Logger::getInstance()->Info(logger_message);
  }
}


