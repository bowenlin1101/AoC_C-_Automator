#include "../input_downloader/Downloader.h"
#include <iostream>

int main() {
    try {
        Downloader downloader("cache");
        std::string input = downloader.fetchInput("%DAY%", "2024");
        std::string sampleInput = downloader.fetchSampleInput("%DAY%", "2024");

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
