#include <iostream>
#include <sstream>
#include <vector>
#include "lru_cache.h"
#include "lfu_cache.h"
#include "fib_runner.h"


int main() {
    LRUCache lruCache(5);
    LFUCache lfuCache(5);

    FibRunner lruRunner(lruCache);
    FibRunner lfuRunner(lfuCache);

    while (true) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cout << "Error" << std::endl;
            break;
        }
        std::istringstream iss(line);
        std::string word;
        std::vector<std::string> tokens;
        while (iss >> word) {
            tokens.push_back(word);
        }
        if (tokens.empty()) {
            continue;
        }
        std::string cmd = tokens[0];
        if (cmd == "exit") {
            std::cout << "bye" << std::endl;
            break;
        }
        else if (cmd == "LRU" or cmd == "LFU") {
            if (tokens.size() < 2) {
                std::cout << "Error: need n" << std::endl;
                continue;
            }
            int n;
            try {
                n = std::stoi(tokens[1]);
                if (n < 0) {
                    std::cout << "Error: bad n" << std::endl;
                    continue;
                }
            }
            catch (...) {
                std::cout << "Error: bad n" << std::endl;
                continue;
            }
            if (cmd == "LRU") {
                int result = lruRunner.computeFib(n);
                std::cout << "Fib(" <<  n << ") = " << result << std::endl;
            }
            else {
                int result = lfuRunner.computeFib(n);
                std::cout << "Fib(" <<  n << ") = " << result << std::endl;
            }
        }
        else {
            std::cout << "Error: unknown command" << std::endl;
        }
    }
}
