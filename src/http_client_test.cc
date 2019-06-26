#include "gtest/gtest.h"
#include "src/http_client.h"

TEST(message_test, content)
{
  HttpClient client;
  client.SetMethod("GET");
  client.FetchUrl("http://www.baidu.com");
}