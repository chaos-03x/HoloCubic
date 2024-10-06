// WeatherManager.cpp
#include "WeatherAPI.h"


WeatherAPI::WeatherAPI(const char* host, const char* apiKey)
    : host(host), apiKey(apiKey), weatherCode(-1) {}


bool WeatherAPI::fetchWeatherData() {
    const int httpPort = 80;
    WiFiClient client;

    Serial.println("\nConnecting to API....");
    if (!client.connect(host, httpPort)) {
        Serial.println("Failed to connect to API.");
        return false;
    }
    Serial.println("Connected to API."); // 连接成功

    // 构建请求URL
    String url = 
    "/v3/weather/daily.json?key=SFIdN0wDbUHnhNI3i&location=fujian%20fuzhou&language=zh-Hans&unit=c";
    Serial.print("Requesting URL: ");
    Serial.println(url); // 打印请求的URL

    // 发送HTTP请求
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    // 等待响应
    delay(3000); // 等待响应
    Serial.println("Waiting for response...");

    // 调试用：读取响应状态行（第一行应包含HTTP状态码）
    String statusLine = client.readStringUntil('\n');
    Serial.print("Status line: ");
    Serial.println(statusLine); // 打印状态行

    // 检查响应状态码，确保是200 OK
    if (statusLine.indexOf("200 OK") == -1) {
        Serial.println("Server returned an error, status line does not contain '200 OK'.");
        return false; // 如果状态码不是200，返回false
    }

    // 调试用：读取并打印响应头，直到遇到空行（响应头的结束标志）
    String header;
    while (client.available()) {
        String line = client.readStringUntil('\n'); // 读取一行响应
        header += line + "\n"; // 将读取的行添加到header中
        Serial.println(line); // 打印每一行响应头
        if (line == "\r") { // 响应头以空行结束 (\r\n\r\n)，检测到 \r 后退出
            break;
        }
    }
    Serial.println("Response header received:");
    Serial.println(header); // 打印完整的响应头

    // 读取响应主体内容
    String answer;
    while (client.available()) {
        String line = client.readStringUntil('\r'); // 读取一行响应
        answer += line; // 将读取的行添加到answer中
    }

    client.stop(); // 关闭连接
    Serial.println("Closing connection");

    // 打印完整的响应内容
    Serial.println("Full response: ");
    Serial.println(answer); // 打印完整的响应内容

    // 提取JSON数据
    int jsonIndex = answer.indexOf('{'); // 查找JSON对象的开始位置
    Serial.print("jsonIndex: ");
    Serial.println(jsonIndex); // 打印jsonIndex的值

    // 检查jsonIndex是否有效
    if (jsonIndex != -1) {
        String jsonAnswer = answer.substring(jsonIndex); // 截取JSON字符串
        // 尝试解析JSON数据
        if (parseWeatherData(jsonAnswer)) {
            Serial.println("JSON data parsed successfully."); // 解析成功
            return true; // 如果解析成功，返回true
        } else {
            Serial.println("Failed to parse JSON data."); // 解析失败
        }
    } else {
        Serial.println("No valid JSON data received."); // 没有找到有效的JSON数据
    }

    return false; // 返回false，表示获取天气数据失败
}



// 解析JSON数据
bool WeatherAPI::parseWeatherData(const String& jsonAnswer) {

JsonDocument doc;

DeserializationError error = deserializeJson(doc, jsonAnswer);

JsonObject results_0 = doc["results"][0];
// 提取位置相关信息
JsonObject results_0_location = results_0["location"];
now_address = results_0_location["name"].as<String>(); // "福州"

// 提取天气相关信息
JsonObject first_daily_item = results_0["daily"][0]; // 获取第一天的天气数据

now_weather = first_daily_item["text_day"].as<String>(); // "小雨"
now_high_tem = first_daily_item["high"].as<String>(); // "28"
now_low_tem = first_daily_item["low"].as<String>(); // "22"
now_rainfall = first_daily_item["rainfall"].as<String>(); // "5.40"
now_wind_direction = first_daily_item["wind_direction"].as<String>(); // "无持续风向"
now_wind_scale = first_daily_item["wind_scale"].as<String>(); // "2"
now_hum = first_daily_item["humidity"].as<String>(); // "85"

// 根据天气描述设置天气代码（优化）
if (strstr(now_weather.c_str(), "晴")) {
    weatherCode = 0;
} else if (strstr(now_weather.c_str(), "多云")) {
    weatherCode = 1;
} else if (strstr(now_weather.c_str(), "阴")) {
    weatherCode = 2;
} else if (strstr(now_weather.c_str(), "阵雨")) {
    weatherCode = 2;
} else if (strstr(now_weather.c_str(), "小雨")) {
    weatherCode = 3;
} else if (strstr(now_weather.c_str(), "中雨")) {
    weatherCode = 4;
} else if (strstr(now_weather.c_str(), "大雨")) {
    weatherCode = 5;
} else if (strstr(now_weather.c_str(), "暴雨")) {
    weatherCode = 5;
}

// 处理最后更新时间
const char* results_0_last_update = results_0["last_update"]; // "2024-10-06T08:00:00+08:00"

// const char* results_0_last_update = results_0["last_update"]; // "2024-10-06T08:00:00+08:00"

    
//     DynamicJsonDocument doc(1536);
//     DeserializationError error = deserializeJson(doc, jsonAnswer);
//     if (error) {
//         Serial.print("Failed to parse JSON: ");
//         Serial.println(error.f_str()); // 输出解析错误信息
//         return false; // 返回false表示解析失败
//     }

//     JsonObject results_0 = doc["results"][0];
//     JsonObject results_0_location = results_0["location"];
//     now_address = results_0_location["name"].as<String>();

//     const char* text_day = results_0["now"]["text"].as<const char*>();
//     now_weather = text_day;

//     now_high_tem = results_0["now"]["high"].as<String>();
//     now_low_tem = results_0["now"]["low"].as<String>();
//     now_rainfall = results_0["now"]["rainfall"].as<String>();
//     now_wind_direction = results_0["now"]["wind_direction"].as<String>();
//     now_wind_scale = results_0["now"]["wind_scale"].as<String>();
//     now_hum = results_0["now"]["humidity"].as<String>();

//     // 根据天气描述设置天气代码
//     if (strstr(now_weather.c_str(), "晴")) {
//         weatherCode = 0;
//     } else if (strstr(now_weather.c_str(), "多云")) {
//         weatherCode = 1;
//     } else if (strstr(now_weather.c_str(), "阴")) {
//         weatherCode = 2;
//     } else if (strstr(now_weather.c_str(), "雨")) {
//         weatherCode = 4;
//     }
    
    return true; // 返回true表示解析成功
}


String WeatherAPI::getAddress() const { return now_address; }
String WeatherAPI::getWeather() const { return now_weather; }
String WeatherAPI::getTemperature() const { return now_high_tem + "°C"; } // Assuming you want to return high temperature
String WeatherAPI::getHighTemperature() const { return now_high_tem; }
String WeatherAPI::getLowTemperature() const { return now_low_tem; }
String WeatherAPI::getRainfall() const { return now_rainfall; }
String WeatherAPI::getWindDirection() const { return now_wind_direction; }
String WeatherAPI::getWindScale() const { return now_wind_scale; }
String WeatherAPI::getHumidity() const { return now_hum; }
int WeatherAPI::getWeatherCode() const { return weatherCode; }
