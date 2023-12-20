#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import PointCloud2
from sensor_msgs import point_cloud2
import pcl
import numpy as np

class PointCloudSubscriber:
    def __init__(self):
        rospy.init_node('point_cloud_subscriber', anonymous=True)
        rospy.Subscriber('/zed2i/zed_node/point_cloud/cloud_registered', PointCloud2, self.callback)
        self.pc_data = None

    def callback(self, msg):
        self.pc_data = msg

    def get_point_cloud(self):
        return self.pc_data

def pixel_to_point(pixel_x, pixel_y, pc_data):
    pc_array = point_cloud2.read_points(pc_data, field_names=('x','y','z'),skip_nans=True)
    # m = [point for i, point in enumerate(pc_array)]
    print('hi')
    print(pc_data.width)
    for i, point in enumerate(pc_array):
        if i == pixel_y * pc_data.height + pixel_x:
            return point

    return None

def calculate_distance(point1, point2):
    print('points')
    print(point1)
    print(point2)
    return np.linalg.norm(np.array(point1) - np.array(point2))

def main():
    pc_subscriber = PointCloudSubscriber()

    while not rospy.is_shutdown():
        pc_data = pc_subscriber.get_point_cloud()

        if pc_data is not None:
            point1 = pixel_to_point(529, 273, pc_data)
            point2 = pixel_to_point(602, 272, pc_data)

            if point1 is not None and point2 is not None:
                distance = calculate_distance(point1, point2)
                print(f"Distance between points: {distance} meters")

        rospy.sleep(1.0)

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
