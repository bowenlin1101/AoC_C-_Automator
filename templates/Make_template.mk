CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -I../input_downloader
LDFLAGS := -L.. -ldownloader -lcurl -lcrypto -lssl

sol: sol.cpp
	$(CXX) $(CXXFLAGS) sol.cpp $(LDFLAGS) -o sol

.PHONY: build
build: sol
	./sol

.PHONY: clean
clean:
	rm -f sol

.PHONY: clear_cache
clear_cache:
	rm -rf ./cache