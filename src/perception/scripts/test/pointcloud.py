#!/usr/bin/env python3

import rospy
import pcl
from sensor_msgs.msg import PointCloud2
from sensor_msgs import point_cloud2

def pointcloud_callback(msg):
    # Convert ROS PointCloud2 to PCL PointCloud
    pc_data = point_cloud2.read_points(msg, field_names=("x", "y", "z"), skip_nans=True)
    points_list = []

    for point in pc_data:
        points_list.append([point[0], point[1], point[2]])

    cloud = pcl.PointCloud()
    cloud.from_list(points_list)

    # Extract XYZ coordinates from the point cloud
    points = cloud.to_array()
    if points.size > 0:
        x, y, z = points[0, 0:3]
        print("X Distance:", x)
        print("Y Distance:", y)
        print("Z Distance:", z)

def main():
    rospy.init_node('zed_distance_node', anonymous=True)

    # Change the topic name to the actual topic name of your ZED camera
    pointcloud_topic = "/zed2i/zed_node/point_cloud/cloud_registered"

    rospy.Subscriber(pointcloud_topic, PointCloud2, pointcloud_callback)

    rospy.spin()

if __name__ == '__main__':
    main()
