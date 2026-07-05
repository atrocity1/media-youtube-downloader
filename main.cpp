#include "crow.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>

using namespace crow;
using namespace std;
namespace fs = std::filesystem;

int main(){
    SimpleApp app;
    CROW_ROUTE(app, "/download").methods(HTTPMethod::POST)([](const request& req){
        query_string params("?" + req.body);
        if (!params.get("url"))
        {
            return response(400, "No Url");
        }
        std::string filepath = "public/video.mp4";
        if (fs::exists(filepath))
        {
            fs::remove(filepath);
        }
        string youtube_url = params.get("url");
        cout << "Downloading: " << youtube_url << "\n";
        string cmd = "yt-dlp -q --no-warnings -f \"b[ext=mp4]\" -o 'public/video.mp4' \"" + youtube_url + "\"";
        int terminal = system(cmd.c_str());
        if (terminal)
        {
            cout << "ERROR: video not founded";
            return response(500, "Error to download video");
        }
        response res;
        res.set_static_file_info("public/video.mp4");
        res.set_header("Content-Type", "video/mp4");
        res.set_header("Content-Disposition", "attachment; filename=\"video.mp4\"");
        std::thread([filepath]() {
            // so pra nao crashar
            std::this_thread::sleep_for(std::chrono::seconds(5));
            if (fs::exists(filepath)) {
                fs::remove(filepath);
                std::cout << "Arquivo temporario " << filepath << " apagado com sucesso!\n";
            }
        }).detach();
        return res;
    });
    app.port(8080).multithreaded().run();
    return 0;
}