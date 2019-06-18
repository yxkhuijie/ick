#include "gtest/gtest.h"
#include "src/mysql_connection.h"


TEST(message_test, content)
{
  MySqlConnection connection("127.0.0.1", "admin", "admin", "online_transcriber");
  std::string sql = "select * from audio_info_online_transcriber";
  connection.open();
  connection.executeQuery(sql);
  EXPECT_EQ("Hello World!", "Hello World!");
}
