#include <iostream>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

Mat _create_thumb(const string& path) {
    Mat image;
    image = imread(path, IMREAD_COLOR);

    if (image.empty()) {
        cout << "Error loading image" << std::endl;
        throw("Error loading image");
    }

    // Resizing
    Mat resized_image;
    //cout << "Image is " << image.rows << "x" << image.cols;

    double max_thumb_size = 600.0;
    double scale_factor;

    // Determine which is larger and use that to scale to what we want
    if (image.rows >= image.cols)
        scale_factor = max_thumb_size / image.rows;
    else
        scale_factor = max_thumb_size / image.cols;

    resize(image, resized_image, Size(), scale_factor, scale_factor, INTER_LINEAR);

    return resized_image;
}

unsigned create_thumb(const string& path, const string& out_filename) {
    vector<uchar> buf;
    Mat resized_image = _create_thumb(path);
    imwrite(out_filename, resized_image);
    return 0;
}

void create_thumb_buf(const string path, vector<uchar> &buf) {
    Mat thumbnail = _create_thumb(path);
    imencode(".jpg", thumbnail, buf);
    
}

void convert_image(fs::path file_path) {
    auto base_path = file_path.parent_path();

    if (file_path.extension() != ".png" &&
        file_path.extension() != ".jpg" &&
        base_path.string().substr(base_path.string().size() - 5) != "thumb") // If parent dir is thumb
        return;
    //cout << file_path << endl;
    cout << ".";
    auto thumb_path = base_path / "thumb";
    if (!fs::exists(thumb_path)) {
        fs::create_directories(thumb_path);
    } // TODO: what if it is a file not a directory!
    auto thumb_filename = (thumb_path / file_path.filename());

    create_thumb(file_path.string(), thumb_filename.string());
}



void display_image(const fs::path& file_path, bool window_for_each = true) {
    Mat image = imread(file_path.string());
    string win_title;
    if (window_for_each) {
        win_title = "Image " + file_path.stem().string();
    }
    else {
        win_title = "Image";
    }
    namedWindow(win_title, WINDOW_AUTOSIZE);
    imshow(win_title, image);
}
