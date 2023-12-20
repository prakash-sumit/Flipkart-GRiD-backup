#!/usr/bin/env python3

import rospy
import pcl
from sensor_msgs.msg import PointCloud2, Image
from sensor_msgs import point_cloud2
from cv_bridge import CvBridge
import numpy as np

bridge = CvBridge()

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

        # Assume depth image is available (replace with the actual topic)
        depth_image_topic = "/zed2i/zed_node/depth/image_rect_color"
        depth_image_msg = rospy.wait_for_message(depth_image_topic, Image)

        # Convert depth image to a NumPy array
        depth_image = bridge.imgmsg_to_cv2(depth_image_msg, desired_encoding="passthrough")

        # Calculate actual x and y distances using camera intrinsics
        fx = 500.0  # replace with actual camera intrinsics
        fy = 500.0  # replace with actual camera intrinsics
        cx = depth_image.shape[1] / 2.0
        cy = depth_image.shape[0] / 2.0

        x_actual = (x / z) * fx + cx
        y_actual = (y / z) * fy + cy

        print("X Actual Distance:", x_actual)
        print("Y Actual Distance:", y_actual)
        print("Z Distance:", z)

def main():
    rospy.init_node('zed_distance_node', anonymous=True)

    # Change the topic name to the actual topic name of your ZED camera
    pointcloud_topic = "/zed2i/zed_node/point_cloud/cloud_registered"

    rospy.Subscriber(pointcloud_topic, PointCloud2, pointcloud_callback)

    rospy.spin()

if __name__ == '__main__':
    main()
