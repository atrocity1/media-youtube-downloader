#include "crow.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace crow;

int main(){
    SimpleApp twitter;
    CROW_ROUTE(twitter, "/download/twitter").methods(HTTPMethod::POST)([](const request& req){
        query_string params("?" + req.body);
    });
}