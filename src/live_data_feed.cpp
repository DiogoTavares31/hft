#include "live_data_feed.h"
#include <iostream>
#include <sstream>
#include <random>
#include <cmath>
#include <thread>
#include <chrono>

// For Windows HTTP requests
#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#endif

LiveDataFeed::LiveDataFeed() : use_mock_fallback(true) {
    // Initialize with realistic starting prices
    mock_prices["AAPL"] = 150.25;
    mock_prices["GOOGL"] = 280.50;
    mock_prices["MSFT"] = 300.75;
    mock_prices["TSLA"] = 220.40;
    mock_prices["AMZN"] = 320.80;
    
    last_prices = mock_prices;
    
    // Initialize price changes to 0
    for (const auto& [symbol, price] : mock_prices) {
        price_changes[symbol] = 0.0;
    }
}

std::string LiveDataFeed::makeHttpRequest(const std::string& url) {
#ifdef _WIN32
    std::string result;
    
    HINTERNET hInternet = InternetOpenA("Trading Bot", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return "";
    
    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return "";
    }
    
    char buffer[4096];
    DWORD bytesRead;
    
    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        result += buffer;
    }
    
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    
    return result;
#else
    // For non-Windows platforms, you'd use libcurl or similar
    return "";
#endif
}

MarketData LiveDataFeed::getRealTimeData(const std::string& symbol) {
    if (use_mock_fallback) {
        return getMockData(symbol);
    }
    
    // Try Alpha Vantage first (comment out if you don't have API key)
    // MarketData data = getAlphaVantageData(symbol);
    // if (data.price > 0) return data;
    
    // Try Finnhub second (comment out if you don't have API key)
    // MarketData data = getFinnhubData(symbol);
    // if (data.price > 0) return data;
    
    // Fallback to mock data
    return getMockData(symbol);
}

MarketData LiveDataFeed::getAlphaVantageData(const std::string& symbol) {
    // Replace YOUR_API_KEY with actual Alpha Vantage API key
    // Get free key at: https://www.alphavantage.co/support/#api-key
    std::string api_key = "YOUR_API_KEY";
    std::string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + 
                     symbol + "&apikey=" + api_key;
    
    std::string response = makeHttpRequest(url);
    MarketData data(symbol, 0, 0);
    
    if (parseAlphaVantageResponse(response, symbol, data)) {
        last_prices[symbol] = data.price;
        return data;
    }
    
    return getMockData(symbol); // Fallback
}

MarketData LiveDataFeed::getFinnhubData(const std::string& symbol) {
    // Replace YOUR_API_KEY with actual Finnhub API key  
    // Get free key at: https://finnhub.io/
    std::string api_key = "YOUR_API_KEY";
    std::string url = "https://finnhub.io/api/v1/quote?symbol=" + symbol + "&token=" + api_key;
    
    std::string response = makeHttpRequest(url);
    MarketData data(symbol, 0, 0);
    
    if (parseFinnhubResponse(response, symbol, data)) {
        last_prices[symbol] = data.price;
        return data;
    }
    
    return getMockData(symbol); // Fallback
}

bool LiveDataFeed::parseAlphaVantageResponse(const std::string& response, const std::string& symbol, MarketData& data) {
    // Simple JSON parsing for Alpha Vantage response
    // Look for "05. price": "XXX.XX"
    size_t price_pos = response.find("\"05. price\":");
    if (price_pos == std::string::npos) return false;
    
    size_t start = response.find("\"", price_pos + 12);
    size_t end = response.find("\"", start + 1);
    
    if (start == std::string::npos || end == std::string::npos) return false;
    
    try {
        std::string price_str = response.substr(start + 1, end - start - 1);
        data.price = std::stod(price_str);
        data.volume = 1000; // Alpha Vantage doesn't provide real-time volume in this endpoint
        return data.price > 0;
    } catch (...) {
        return false;
    }
}

bool LiveDataFeed::parseFinnhubResponse(const std::string& response, const std::string& symbol, MarketData& data) {
    // Simple JSON parsing for Finnhub response
    // Look for "c":XXX.XX (current price)
    size_t price_pos = response.find("\"c\":");
    if (price_pos == std::string::npos) return false;
    
    size_t start = price_pos + 4;
    size_t end = response.find_first_of(",}", start);
    
    if (end == std::string::npos) return false;
    
    try {
        std::string price_str = response.substr(start, end - start);
        data.price = std::stod(price_str);
        data.volume = 1000; // Finnhub quote endpoint doesn't include volume
        return data.price > 0;
    } catch (...) {
        return false;
    }
}

MarketData LiveDataFeed::getMockData(const std::string& symbol) {
    static std::mt19937 gen(std::random_device{}());
    static std::normal_distribution<double> change_dist(0.0, 0.002); // 0.2% volatility
    
    if (mock_prices.find(symbol) == mock_prices.end()) {
        mock_prices[symbol] = 100.0; // Default price
        last_prices[symbol] = 100.0;
        price_changes[symbol] = 0.0;
    }
    
    // Generate realistic price movement
    double change_pct = change_dist(gen);
    double old_price = mock_prices[symbol];
    double new_price = old_price * (1.0 + change_pct);
    
    // Keep prices in reasonable bounds
    if (new_price < old_price * 0.8) new_price = old_price * 0.8;
    if (new_price > old_price * 1.2) new_price = old_price * 1.2;
    
    mock_prices[symbol] = new_price;
    price_changes[symbol] = new_price - last_prices[symbol];
    last_prices[symbol] = new_price;
    
    // Generate realistic volume
    double volume = std::abs(std::normal_distribution<double>(50000, 10000)(gen));
    
    return MarketData(symbol, new_price, volume);
}