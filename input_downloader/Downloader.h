#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
class Downloader {
    public:
        Downloader(const std::string& cacheDir);
        std::string fetchInput(const std::string& day, const std::string& year);
        std::string fetchSampleInput(const std::string& day, const std::string& year);
    
    private:
        std::string computeHash(const std::string& input);
        std::string getCacheFilePath(const std::string& hash);
        bool isCached(const std::string& hash);
        void saveToCache(const std::string& hash, const std::string& content);
        std::string loadFromCache(const std::string& hash);
        std::string getSessionCookie();
        std::string trim(const std::string& str);
        std::string extractPreContent(const std::string& str);

        std::string cacheDirectory;
        std::string field1;
};

#endif