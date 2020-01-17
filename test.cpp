#include <iostream>
#include <opencv2/opencv.hpp>
#include <customnetwork_print.h>
//#include <customnetwork_replay.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>

using namespace std;

int main(int argc,char**argv)
{
    dlib::anet_type net;
    dlib::deserialize("./print_attack_net_v6.dat") >> net;
    dlib::matrix<dlib::rgb_pixel> img;
    cv::Mat image = cv::imread(argv[1]);
    cv::resize(image,image,cv::Size(100,100));
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    dlib::assign_image(img, dlib::cv_image<dlib::rgb_pixel>(image));
    auto result = net(img);
    cout<<result<<endl;
    return 0;
}
