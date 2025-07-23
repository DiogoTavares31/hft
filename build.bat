@echo off
echo Building Trading Algorithm...

cl /EHsc /std:c++17 /O2 /c src/main.cpp
cl /EHsc /std:c++17 /O2 /c src/market_data.cpp  
cl /EHsc /std:c++17 /O2 /c src/order.cpp
cl /EHsc /std:c++17 /O2 /c src/position.cpp
cl /EHsc /std:c++17 /O2 /c src/technical_indicators.cpp
cl /EHsc /std:c++17 /O2 /c src/trading_strategy.cpp
cl /EHsc /std:c++17 /O2 /c src/live_data_feed.cpp
cl /EHsc /std:c++17 /O2 /c src/trading_engine.cpp

echo Linking...
cl *.obj /Fe:trading_algo.exe

echo Build complete!
echo Run with: trading_algo.exe

pause