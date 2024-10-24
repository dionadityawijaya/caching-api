#include "cache.h"
#include <iostream>
#include <thread>
#include <crow.h>
#include <fstream>
#include <sstream>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char const *argv[])
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]{
        std::string content = readFile("templates/index.html");
        return crow::response(content);
    });



    Cache apiCache(3, std::chrono::seconds(10));


    // Adding data to the cache
    apiCache.put("user/1", "User 1 Data");
    apiCache.put("user/2", "User 2 Data");
    apiCache.put("user/3", "User 3 Data");

    CROW_ROUTE(app, "/api/user/<string>")
([&apiCache](const std::string& userId){
    auto result = apiCache.get("user/" + userId);
    if(result.empty()) {
        return crow::response(404, "User not found or cache expired");
    }

    crow::json::wvalue jsonResponse;
    jsonResponse["user"] = result;
    return crow::response(jsonResponse);  // Wrap JSON in crow::response
});

    app.port(8000).multithreaded().run();

    return 0;
}
