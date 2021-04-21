#include <ros/ros.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <termios.h>
#include <typeinfo>


#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>

#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

using namespace std;

int c;
//char c;
//string save_image_path;
//string save_pcd_path;
int count_img=0;
int count_pcd=0;


ros::Subscriber sub_img;
ros::Subscriber sub_velo;
ros::Publisher pub_img;
ros::Publisher pub_velo;


int getch()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings
  int c = getchar();  // read character (non-blocking)
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}

void img_cb(const sensor_msgs::Image::ConstPtr& image)
{
	cout << "Image call back ..." << endl;
	stringstream ss;
	ss << "/home/young/catkin_calib/src/capture_pcd_img/data/image/" << count_img << ".jpg" << endl;
	
	cv_bridge::CvImagePtr cv_ptr;
	try
	{
		cv_ptr = cv_bridge::toCvCopy(image,"bgr8");
	}
	catch(cv_bridge::Exception& e)
	{
		ROS_ERROR("cv_bridge exception:%s",e.what());
	}
	
	if (c == 'i') // flag to take an input sample
	{
		pub_img.publish(image);
		cout << "I got image  !" << endl;
	}
	
	else if (c == 'o')
	{
		ROS_INFO("Saved this Image sample!");
		cv::imwrite(ss.str(),cv_ptr->image);
		count_img++;
		
	}
	

}

void velo_cb(const sensor_msgs::PointCloud2::ConstPtr& in_cloud)
{
	pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);
	pcl::fromROSMsg(*in_cloud, *cloud);
	
	cout << "Velodyne call back ..." << endl;
	
	stringstream ss;
	ss << "/home/young/catkin_calib/src/capture_pcd_img/data/pointcloud/" << count_pcd << ".pcd";
	//string filename = ss.str();
	//cout << "Type:" << typeid(ss.str()).name() << endl;
	//cout << "Type:" << typeid("/home/young/catkin_calib/src/capture_pcd_img/data/pointcloud/0001.pcd").name() << endl;
	
	if (c == 'i') // flag to take an input sample
	{
		pub_velo.publish(in_cloud);
		cout << "I got velodyne  !" << endl;
	}
	
	else if (c == 'o')
	{
		ROS_INFO("Saved this velodyne sample!");
		pcl::PCDWriter writer;
		writer.write<pcl::PointXYZI> (ss.str(),*cloud,false);
		count_pcd++;
	}
}


int main(int argc,char** argv)
{
	ros::init(argc,argv,"capture_node");
	ros::NodeHandle nh("~");
	
	
	sub_img = nh.subscribe("/velodyne/image", 1, img_cb);
	sub_velo = nh.subscribe("/velodyne_points", 1, velo_cb);
	pub_img = nh.advertise<sensor_msgs::Image>("/calib/Image", 10);
	pub_velo = nh.advertise<sensor_msgs::PointCloud2>("/calib/pointcloud", 10);
	
	while(ros::ok())
	{
		std::cout << " Now, press an appropriate key ... " << std::endl;
		std::cout << " 'i' to take an input sample" << std::endl;
		std::cout << " 'o' to save an input sample" << std::endl;
		std::cout << " 'e' to end the calibration process" << std::endl;
		//c = getchar();
		c = getch();
		
		if (c == 'e')
		{
			ros::shutdown();
		}
		
		ros::spinOnce();
	}
	
	return 0;
}



