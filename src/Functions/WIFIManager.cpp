#include "WiFiManager.h"
#include <Update.h> // 添加此行以支持OTA更新

const char* ssid = "CMCC-awmy-2023";  // WiFi账号
const char* password = "20232323";     // WiFi密码
WebServer server(80); // 设置端口为80

// 判断WIFI是否连接
bool isWIFIConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// 开启WIFI热点
void startWifiAP() {
    const char *ssid = "ESP32";
    const char *password = "12345678";

    // 配置WiFi热点
    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
}

void closeWifiAP() {
    WiFi.softAPdisconnect(true); // true 表示断开所有连接
    Serial.println("Hotspot closed");
}


// 显示HTML页面，选择WiFi并连接
void handleRoot() {
    String html = "<html><head>";
    html += "<style>";
    html += "body {";
    html += "    background-color: #f0f0f0;"; // 背景颜色
    html += "    display: flex;"; // 使用 flexbox
    html += "    flex-direction: column;"; // 垂直排列
    html += "    align-items: center;"; // 水平居中
    html += "    justify-content: center;"; // 垂直居中
    html += "    height: 100vh;"; // 高度占满视口
    html += "    font-family: Arial, sans-serif;"; // 字体
    html += "}";
    html += "h1 {";
    html += "    color: #333;"; // 标题颜色
    html += "}";
    html += ".form-container {";
    html += "    margin: 10px;"; // 表单间距
    html += "    padding: 20px;"; // 表单内边距
    html += "    background-color: #fff;"; // 表单背景颜色
    html += "    border-radius: 5px;"; // 圆角
    html += "    box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);"; // 阴影
    html += "    display: flex;"; // 使用 flexbox
    html += "    flex-direction: column;"; // 垂直排列
    html += "    align-items: center;"; // 水平居中
    html += "}";
    html += "select, input[type='text'], input[type='password'] {";
    html += "    width: 200px;"; // 输入框宽度
    html += "    padding: 10px;"; // 内边距
    html += "    margin: 5px 0;"; // 外边距
    html += "    border: 1px solid #ccc;"; // 边框
    html += "    border-radius: 4px;"; // 圆角
    html += "}";
    html += "input[type='submit'] {";
    html += "    background-color: #4CAF50;"; // 按钮背景颜色
    html += "    color: white;"; // 按钮文字颜色
    html += "    padding: 10px 20px;"; // 按钮内边距
    html += "    border: none;"; // 无边框
    html += "    border-radius: 4px;"; // 圆角
    html += "    cursor: pointer;"; // 鼠标指针样式
    html += "    margin-top: 10px;"; // 按钮上方间距
    html += "}";
    html += "input[type='submit']:hover {";
    html += "    background-color: #45a049;"; // 悬停时按钮颜色
    html += "}";
    html += "</style>";
    html += "</head><body>";
    html += "<h1>WiFi Configuration</h1>";
    html += "<div class='form-container'>";
    html += "<form action='/connect' method='POST'>";
    html += "WiFi Name: <select id='wifiSelect' name='ssid'></select><br>"; // 下拉列表
    html += "Password: <input type='password' name='password' required><br>"; // 必填密码
    html += "<input type='submit' value='Connect'>";
    html += "</form>";
    html += "<form action='/restart' method='POST'>";
    html += "<input type='submit' value='Restart ESP32'>";
    html += "</form>";
    html += "<form action='/serial/start' method='POST'>";
    html += "<input type='submit' value='Start Receiving Serial Data'>";
    html += "</form>";
    html += "<form action='/serial/stop' method='POST'>";
    html += "<input type='submit' value='Stop Receiving Serial Data'>";
    html += "</form>";
    html += "<form action='/serial' method='GET'>";
    html += "<input type='submit' value='View Serial Data'>";
    html += "</form>";
    
    // 添加固件上传的表单
    html += "<form action='/upload' method='POST' enctype='multipart/form-data'>";
    html += "Select Firmware File: <input type='file' name='firmware' required><br>";
    html += "<input type='submit' value='Upload Firmware'>";
    html += "</form>";
    
    html += "</div>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
}

// 处理WiFi连接请求
void handleConnect() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    
    server.send(200, "text/html", "<html><body><h1>Connecting to WiFi...</h1></body></html>");
    ConnectWIFI(ssid.c_str(), password.c_str()); // 使用用户输入的WiFi名称和密码连接
}

// 重启ESP32
void handleRestart() {
    server.send(200, "text/html", "<html><body><h1>Restarting...</h1></body></html>");
    ESP.restart();
}

// 获取可连接的WIFI列表
void handleGetWiFiList() {
    int n = WiFi.scanNetworks(); // 扫描可用 WiFi
    String json = "[";

    for (int i = 0; i < n; i++) {
        json += "\"" + String(WiFi.SSID(i)) + "\"";
        if (i < n - 1) json += ",";
    }
    
    json += "]";
    server.send(200, "application/json", json); // 发送 JSON 响应
}


// 处理固件上传
void handleUpload() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        Serial.printf("Upload Start: %s\n", filename.c_str());
        
        // 开始更新
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        // 写入数据
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { // true表示提交更新
            Serial.printf("Update Success: %u B\n", upload.totalSize);
            server.send(200, "text/html", "<html><body><h1>Update Success!</h1></body></html>");
        } else {
            Update.printError(Serial);
            server.send(500, "text/html", "<html><body><h1>Update Failed!</h1></body></html>");
        }
    }
}

// 开启HTTP服务器
void startHTTPServer() {
    server.on("/", handleRoot);        // 主页路由
    server.on("/connect", handleConnect);  // 连接WiFi的路由
    server.on("/upload", HTTP_POST, []() {
        server.send(200, "text/html", "<html><body><h1>Uploading...</h1></body></html>");
    }, handleUpload);  // 处理文件上传的路由
    server.on("/restart", handleRestart);  // 重启ESP32的路由
    server.on("/getWiFiList", handleGetWiFiList); // 获取可连接的WIFI列表

    server.begin();  // 启动HTTP服务器并监听
    Serial.println("HTTP server started");
}

// 连接WIFI
bool ConnectWIFI() {
    Serial.print("Function::ConnectWIFI: Connecting to WiFi ");
    WiFi.begin(ssid, password);
    
    // 等待WiFi连接
    int timeout = 15000;  // 设定超时
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(500);
        Serial.print(".");
        timeout -= 500;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi:");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("Failed to connect to WiFi");
        return false;
    }
}

// 连接WIFI
bool ConnectWIFI(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    
    // 等待WiFi连接
    int timeout = 15000;  // 设定超时
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(500);
        Serial.print(".");
        timeout -= 500;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to WiFi");
    } else {
        Serial.println("Failed to connect to WiFi");
    }
}
