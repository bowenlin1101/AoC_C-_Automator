#include "Downloader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <openssl/sha.h>
#include <curl/curl.h>
#include <filesystem>

namespace fs = std::filesystem;


static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Downloader::Downloader(const std::string& cacheDir) : cacheDirectory(cacheDir) {
    if (!fs::exists(cacheDirectory)) {
        fs::create_directories(cacheDirectory);
    }
}

std::string Downloader::fetchSampleInput(const std::string& day, const std::string& year) {
    std::string url = "https://adventofcode.com/"+year+"/day/"+day;
    std::string hash = computeHash(url);
    std::string cachePath = getCacheFilePath(hash);

    if (isCached(hash)) {
        return loadFromCache(hash);
    }

    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("Failed to fetch URL: " + url);
        }
        saveToCache(hash, extractPreContent(response));
    }
    return extractPreContent(response);
}

std::string Downloader::fetchInput(const std::string& day, const std::string& year) {
    std::string url = "https://adventofcode.com/"+year+"/day/"+day+"/input";
    std::string hash = computeHash(url);
    std::string cachePath = getCacheFilePath(hash);

    if (isCached(hash)) {
        return loadFromCache(hash);
    }

    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        std::string sessionCookie = "session=" + getSessionCookie();
        curl_easy_setopt(curl, CURLOPT_COOKIE, sessionCookie.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "https://github.com/bowenlin1101/AoC_Cpp_Automator by bowen@lin.ca");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("Failed to fetch URL: " + url);
        }

        saveToCache(hash, response);
    }

    return response;
}

std::string Downloader::computeHash(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*) input.c_str(), input.size(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        oss << std::hex << std::setw(2) <<std::setfill('0') << (int) hash[i];
    }
    return oss.str();
}

std::string Downloader::getCacheFilePath(const std::string& hash) {
    return cacheDirectory + "/" + hash + ".txt";
}

bool Downloader::isCached(const std::string& hash) {
    return fs::exists(getCacheFilePath(hash));
}

void Downloader::saveToCache(const std::string& hash, const std::string& content) {
    std::ofstream outFile(getCacheFilePath(hash));
    if (!outFile) {
        throw std::runtime_error("Failed to save cache file");
    }
    outFile << content;
}

std::string Downloader::loadFromCache(const std::string& hash) {
    std::ifstream inFile(getCacheFilePath(hash));
    if(!inFile) {
        throw std::runtime_error("Failed to load cache file");
    }
    std::ostringstream oss;
    oss <<inFile.rdbuf();
    return oss.str();
}

std::string Downloader::getSessionCookie() {
    std::ifstream inFile("../.env");
    if (!inFile) {
        throw std::runtime_error("Failed to load .env file");
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.rfind("SESSION_COOKIE=", 0) == 0) {
            return trim(line.substr(15)); // 15 is the length of "SESSION_COOKIE="
        }
    }

    // If the cookie is not found, throw an error
    throw std::runtime_error("SESSION_COOKIE not found in .env file");
}

std::string Downloader::trim(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return ""; // All spaces
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string Downloader::extractPreContent(const std::string &html) {
    std::string startTag = "<pre><code>";
    std::string endTag = "</code></pre>";
    
    size_t start = html.find(startTag);
    if (start == std::string::npos) {
        throw std::runtime_error("No <pre><code> tag found in HTML.");
    }
    start += startTag.length();

    size_t end = html.find(endTag, start);
    if (end == std::string::npos) {
        throw std::runtime_error("No matching </code></pre> tag found.");
    }

    return html.substr(start, end - start);
}