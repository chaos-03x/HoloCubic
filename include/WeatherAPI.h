// WeatherAPI.h
#ifndef WEATHEAPI_H
#define WEATHEAPI_H

#include <ArduinoJson.h>  // Json库
#include <WiFiClient.h>   // WiFiClient库
#include <WiFi.h>
#include "WIFIManager.h"

class WeatherAPI {
public:
    WeatherAPI(const char* host, const char* apiKey);
    bool fetchWeatherData();
    String getAddress() const;
    String getWeather() const;
    String getTemperature() const;
    String getHighTemperature() const;
    String getLowTemperature() const;
    String getRainfall() const;
    String getWindDirection() const;
    String getWindScale() const;
    String getHumidity() const;
    int getWeatherCode() const;

private:
    const char* host;
    const char* apiKey;
    WiFiClient client;
    String now_address;
    String now_weather;
    String now_high_tem;
    String now_low_tem;
    String now_rainfall;
    String now_wind_direction;
    String now_wind_scale;
    String now_hum;
    int weatherCode;

    bool parseWeatherData(const String& jsonAnswer);
};

#endif // WEATHERAPI_H