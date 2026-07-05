#include "crow.h"
#include <iostream>
#include <cstdlib>
#include <string>

using namespace crow;

int main(){
    SimpleApp app;
    CROW_ROUTE(app, "/download").methods(HTTPMethod::POST)([](const request& req){
        query_string params("?" + req.body);
        if (!params.get("url"))
        {
            return response(400, "No Url");
        }
        std::string youtube_url = params.get("url");
        std::cout << "Downloading: " << youtube_url << "\n";
        std::string cmd = "yt-dlp -q --no-warnings -f \"b[ext=mp4]\" -o 'public/video.mp4' \"" + youtube_url + "\"";
        int terminal = std::system(cmd.c_str());
        if (terminal)
        {
            std::cout << "ERROR: video not founded";
            return response(500, "Error to download video");
        }
        
        
        response res;
        res.set_static_file_info("public/video.mp4");
        res.set_header("Content-Type", "video/mp4");
        res.set_header("Content-Disposition", "attachment; filename=\"video.mp4\"");
        return res;
    });
    app.port(8080).multithreaded().run();
    return 0;
}