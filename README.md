# C++ Trading Bot 📈

A simple algorithmic trading simulation built in C++ for fun and learning. This project was completely **vibe coded**.

## ⚠️ Disclaimer

**This is a fun project created for educational purposes only!** 
- Not intended for real trading
- Use at your own risk
- No financial advice intended
- Results are simulated and not indicative of real market performance

## 🚀 Features

- **Real-time Market Data**: Supports Alpha Vantage and Finnhub APIs (with mock data fallback)
- **Technical Indicators**: Moving averages, RSI, Bollinger Bands
- **Mean Reversion Strategy**: Automated buy/sell signals based on multiple indicators
- **Position Management**: Track positions, P&L, and order history
- **Live Dashboard**: Real-time console display with fancy ASCII borders
- **Graceful Shutdown**: Press 'q + Enter' to stop the simulation

## 🏗️ Architecture

The bot is structured with clean C++ classes:
- LiveDataFeed     # Market data retrieval (APIs + mock data)
- TradingEngine    # Core trading logic and order management
- TradingStrategy  # Signal generation using technical indicators
- TechnicalIndicators # MA, RSI, Bollinger Bands calculations
- Position         # Position tracking and P&L calculations
- Order           # Order management and execution
- MarketData      # Market data structure

## 📊 Trading Strategy

The bot implements a **mean reversion strategy** with multiple confirmation signals:

- **Buy Signal**: Price below Bollinger lower band + RSI < 30 + MA5 < MA20
- **Sell Signal**: Price above Bollinger upper band + RSI > 70 + MA5 > MA20
- **Exit**: 2% profit/loss threshold

## 🛠️ Setup & Installation

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- Windows (for HTTP requests) or modify for your platform

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/DiogoTavares31/hft.git
cd hft

Compile the project:

bash# For Windows
g++ -std=c++17 -I./include src/*.cpp -o trading_algo.exe -lwininet

# For Linux/Mac (you'll need to implement HTTP client)
g++ -std=c++17 -I./include src/*.cpp -o trading_algo

Run the simulation:

bash./trading_bot
🔧 Configuration
API Keys (Optional)
If you want real market data instead of mock data:

Alpha Vantage: Get free API key at alphavantage.co
Finnhub: Get free API key at finnhub.io

Replace YOUR_API_KEY in live_data_feed.cpp and set:
cppdata_feed.setMockFallback(false);  // Use real data
Symbols
Currently trades: AAPL, MSFT, GOOGL, TSLA, AMZN
Modify the symbols vector in main.cpp to trade different stocks.
🎮 Usage

Start the bot - it will initialize with mock data
Watch the live dashboard update every second
Monitor buy/sell signals and P&L changes
Press 'q' + Enter to stop gracefully
Simulation auto-stops after 10 minutes

Sample Output
╔══════════════════════════════════════════════════════════════════════════════╗
║                              TRADING DASHBOARD                               ║
╠══════════════════════════════════════════════════════════════════════════════╣
║ Time: 2025-08-05 14:30:15                                                   ║
╠══════════════════════════════════════════════════════════════════════════════╣
║ SYMBOL │   PRICE   │  CHANGE  │ POSITION │ AVG COST │ UNREALIZED │ REALIZED ║
╠════════╪═══════════╪══════════╪══════════╪══════════╪════════════╪══════════╣
║ AAPL   │ $150.25   │  +0.5%   │      100 │ $149.80  │    +$45.00 │  +$12.50 ║
║ GOOGL  │ $280.50   │  -0.2%   │        0 │   $0.00  │     $0.00  │   $8.75  ║
╚══════════════════════════════════════════════════════════════════════════════╝
📁 File Structure
trading-bot/
├── include/
│   ├── live_data_feed.h
│   ├── trading_engine.h
│   ├── trading_strategy.h
│   ├── technical_indicators.h
│   ├── position.h
│   ├── order.h
│   └── market_data.h
├── src/
│   ├── live_data_feed.cpp
│   ├── trading_engine.cpp
│   ├── trading_strategy.cpp
│   ├── technical_indicators.cpp
│   ├── position.cpp
│   ├── order.cpp
│   ├── market_data.cpp
│   └── main.cpp
└── README.md

🎯 To Do
 Improvements:

 Better error handling
 More sophisticated strategies
 Backtesting framework
 Web interface
 Database integration
 Risk management
 More technical indicators
 Multi-threading for real-time data
 Linux/Mac HTTP client implementation
