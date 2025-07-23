#include "C:\Users\diogo\Desktop\Personal\Code\hft\include\trading_engine.h"
#include "C:\Users\diogo\Desktop\Personal\Code\hft\include\live_data_feed.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <atomic>

// Global flag for graceful shutdown
std::atomic<bool> should_stop(false);

void checkForUserInput() {
    std::string input;
    while (!should_stop) {
        std::getline(std::cin, input);
        if (input == "q" || input == "quit" || input == "stop") {
            should_stop = true;
            break;
        }
    }
}

int main() {
    TradingEngine engine;  
    LiveDataFeed data_feed;
    
    // Use mock data by default (set to false if you have API keys)
    data_feed.setMockFallback(true);
    
    // Add trading strategies for symbols - more popular stocks
    std::vector<std::string> symbols = {"AAPL", "MSFT", "GOOGL", "TSLA", "AMZN"};
    for (const auto& symbol : symbols) {
        engine.addStrategy(symbol);
    }
    
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                           TRADING ALGORITHM STARTED                          ║" << std::endl;
    std::cout << "║                                                                              ║" << std::endl;
    std::cout << "║  • Using: " << (data_feed.isMockFallback() ? "Mock Data (Demo Mode)" : "Live Market Data") << std::string(47, ' ') << "║" << std::endl;
    std::cout << "║  • Strategy: Mean Reversion with Technical Indicators                       ║" << std::endl;
    std::cout << "║  • Symbols: AAPL, MSFT, GOOGL, TSLA, AMZN                                  ║" << std::endl;
    std::cout << "║                                                                              ║" << std::endl;
    std::cout << "║  Press 'q' + Enter to stop gracefully                                       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\nInitializing..." << std::endl;
    
    // Start input monitoring thread
    std::thread input_thread(checkForUserInput);
    input_thread.detach();
    
    auto start_time = std::chrono::system_clock::now();
    int tick_count = 0;
    
    try {
        while (!should_stop) {
            // Generate market data for each symbol
            for (const auto& symbol : symbols) {
                MarketData tick = data_feed.getRealTimeData(symbol);
                engine.processMarketData(tick);
            }
            
            tick_count++;
            
            // Update display every 10 ticks (1 second) instead of printing each tick
            if (tick_count % 10 == 0) {
                engine.printStatus();
            }
            
            // Sleep to simulate real-time data (100ms per tick)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            // Run for 10 minutes max or until user stops
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now() - start_time);
            if (elapsed.count() > 600) {
                std::cout << "\n⏰ Simulation completed after 10 minutes." << std::endl;
                should_stop = true;
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "❌ Error: " << e.what() << std::endl;
    }
    
    if (should_stop) {
        std::cout << "\n🛑 Simulation stopped by user." << std::endl;
    }
    
    engine.printStatus();
    std::cout << "\n✅ Trading simulation ended. Thank you!" << std::endl;
    
    return 0;
}