#pragma once
#include <iostream>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
namespace fs = std::filesystem;

void convert_image(fs::path);
unsigned create_thumb(string, string);
void create_thumb_buf(string, vector<uchar>&);
void display_image(const fs::path&, bool);
