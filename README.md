# capture_pcd_img
To capture PCD files and Images from a rosbag with topic like sensor_msgs::PointCloud2 and sensor_msgs::Image
# To use this code to capture image and pcd from lidar and camear both online or offline.

- Copy this package into your workspace,and build with catkin_make.

- You can use the following command to start capture

> Input i in the terminal to view the frame of image and lidar.
> Input o to save this frame of image and lidar.
> Input e to quit this process.



``` shell
	roscore
	source devel/setup.bash
	rosrun capture_pcd_img capture_node
```
