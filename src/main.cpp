#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <std_msgs/String.h>
#include <zbar.h>

using namespace cv;
using namespace std;
using namespace zbar;
string image_topic;

class QrDetector {

  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  ros::Publisher str_pub;  
  std_msgs::String str_msg;
  Mat gray;

public:
  QrDetector()
    : it_(nh_) {
    image_sub_ = it_.subscribe(image_topic, 1, &QrDetector::cameraCallback, this);
    str_pub = nh_.advertise<std_msgs::String>("/qr_scanner/data", 1000);
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
  }

  void cameraCallback(const sensor_msgs::ImageConstPtr& msg) {
    
    cv_bridge::CvImagePtr cv_ptr;
    try {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e) {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    cvtColor(cv_ptr->image, gray, COLOR_RGB2GRAY);
    Image imageZbar(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
    scanner.scan(imageZbar);

    Image::SymbolIterator symbol = imageZbar.symbol_begin();
    for (; symbol != imageZbar.symbol_end(); ++symbol) {
      std::stringstream ss;
      ss << symbol->get_data();
      str_msg.data = ss.str();
      str_pub.publish(str_msg);
    }
  }

private:
  zbar::ImageScanner scanner;
};

int main (int argc, char** argv) {
  ros::init(argc, argv, "qr_detector");
  ros::NodeHandle nh_;
  nh_.getParam("/camera_topic", image_topic);
  QrDetector ic;
  ros::spin ();
  return 0;
}