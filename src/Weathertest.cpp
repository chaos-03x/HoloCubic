// #include <WiFi.h> //wifi库
// #include <ArduinoJson.h>  //Json库
// #include <HTTPClient.h>  //HTTP库
// const char* ssid    = "DESKTOP-GJ8S4SV 0371";  //wifi账号
// const char* password = "07b03>7G";  //wifi密码
// const char* host = "api.seniverse.com";  //心知天气服务器地址
// String now_address="",now_time="",now_temperature="";//用来存储报文得到的字符串
// void setup()
// {
//   Serial.begin(115200);
//   // 连接网络
//   WiFi.begin(ssid, password);
//   //等待wifi连接
//   while (WiFi.status() != WL_CONNECTED)
//  {
//     delay(500);
//     Serial.print(".");
//  }
//   Serial.println("");
//   Serial.println("WiFi connected"); //连接成功
//   Serial.print("IP address: ");    //打印IP地址
//   Serial.println(WiFi.localIP());
// }
// void loop()
// {
//     //创建TCP连接
//     WiFiClient client;
//     const int httpPort = 80;
//     if (!client.connect(host, httpPort))
//    {
//       Serial.println("connection failed");  //网络请求无响应打印连接失败
//       return;
//    }
//     //URL请求地址
//     String url ="/v3/weather/now.json?key=SFIdN0wDbUHnhNI3i&location=fujian%20fuzhou&language=zh-Hans&unit=c";
//     //发送网络请求
//     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" +
//               "Connection: close\r\n\r\n");
//     delay(5000);
//     //定义answer变量用来存放请求网络服务器后返回的数据
//     String answer;
//     while(client.available())
//    {
//       String line = client.readStringUntil('\r');
//       answer += line;
//    }
//     //断开服务器连接
//   client.stop();
//   Serial.println();
//   Serial.println("closing connection");

//     //获得json格式的数据
//   String jsonAnswer;
//   int jsonIndex;
//   //找到有用的返回数据位置i 返回头不要
//   for (int i = 0; i < answer.length(); i++) {
//     if (answer[i] == '{') {
//       jsonIndex = i;
//       break;
//    }
//  }
//   jsonAnswer = answer.substring(jsonIndex);
//   Serial.println();
//   Serial.println("JSON answer: ");
//   Serial.println(jsonAnswer);

//   // Stream& input;
//   // Json数据处理

// JsonDocument doc;

// DeserializationError error = deserializeJson(doc, jsonAnswer);

// JsonObject results_0 = doc["results"][0];

// JsonObject results_0_location = results_0["location"];
// // const char* results_0_location_id = results_0_location["id"]; // "WSSU6EXX52RE"
// const char* results_0_location_name = results_0_location["name"]; // "福州"
// // const char* results_0_location_country = results_0_location["country"]; // "CN"
// // const char* results_0_location_path = results_0_location["path"]; // "福州,福州,福建,中国"
// // const char* results_0_location_timezone = results_0_location["timezone"]; // "Asia/Shanghai"
// // const char* results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"

// JsonObject results_0_now = results_0["now"];
// const char* results_0_now_text = results_0_now["text"]; // "多云"
// // const char* results_0_now_code = results_0_now["code"]; // "4"
// const char* results_0_now_temperature = results_0_now["temperature"]; // "25"

// // const char* results_0_last_update = results_0["last_update"]; // "2024-09-28T21:27:46+08:00"

// Serial.println("Address:");
// Serial.println(results_0_location_name);
// Serial.println("Weather:");
// Serial.println(results_0_now_text);
// Serial.println("Temperature:");
// Serial.println(results_0_now_temperature);
// }
