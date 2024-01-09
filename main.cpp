#include <iostream>
#include "json.hpp"
#include <curl/curl.h>

// Callback function for writing received data
size_t WriteCallback(const char* contents, const size_t size, const size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*) contents, totalSize);
    return totalSize;
}

int main() {
    // Initialize the CURL library
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a CURL handle
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set the URL for the GET request
        std::string url = "http://jsonplaceholder.typicode.com/todos/?_limit=1";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function for writing received data
        std::string responseData;
        //used to set a callback function that will be called to write the received data.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        //this sets the output parameter for the write callback to be the address of the responseData string.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        // Perform the GET request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "Curl failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Parse the JSON response using nlohmann JSON
            try {
                nlohmann::json jsonResponse = nlohmann::json::parse(responseData);
                
                 // Iterate through the array and print specific fields from each todo
                for (const auto& todo : jsonResponse) {
                    std::cout << "ID: " << todo["id"] << std::endl;
                    std::cout << "Title: " << todo["title"] << std::endl;
                    std::cout << "Completed: " << todo["completed"] << std::endl;
                    std::cout << "------------------------" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    // Cleanup the CURL library
    curl_global_cleanup();

    return 0;
}
