#pragma once
#include "market_data.h"
#include <string>
#include <map>
#include <vector>

class LiveDataFeed {
private:
    std::map<std::string, double> last_prices;
    std::map<std::string, double> price_changes;
    
    // For fallback mock data when API fails
    std::map<std::string, double> mock_prices;
    bool use_mock_fallback;
    
    std::string makeHttpRequest(const std::string& url);
    bool parseAlphaVantageResponse(const std::string& response, const std::string& symbol, MarketData& data);
    bool parseFinnhubResponse(const std::string& response, const std::string& symbol, MarketData& data);
    
public:
    LiveDataFeed();
    
    // Try to get real data, fallback to mock if failed
    MarketData getRealTimeData(const std::string& symbol);
    
    // Get data from Alpha Vantage (free tier: 5 calls/minute, 500 calls/day)
    MarketData getAlphaVantageData(const std::string& symbol);
    
    // Get data from Finnhub (free tier: 60 calls/minute)
    MarketData getFinnhubData(const std::string& symbol);
    
    // Fallback mock data that behaves more realistically
    MarketData getMockData(const std::string& symbol);
    
    void setMockFallback(bool use_mock) { use_mock_fallback = use_mock; }

    bool isMockFallback() const { return use_mock_fallback; }
};