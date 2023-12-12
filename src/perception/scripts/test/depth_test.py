#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

def depth_callback(data):
    try:
        bridge = CvBridge()
        # Convert the depth image to a OpenCV format
        depth_image = bridge.imgmsg_to_cv2(data, desired_encoding="passthrough")
        
        # You can now process and print the depth data as per your requirements
        # For example, to print the depth value at the center pixel:
        center_pixel_depth = depth_image[depth_image.shape[0] // 2, depth_image.shape[1] // 2]
        print("Depth at center pixel:", center_pixel_depth)

    except Exception as e:
        rospy.logerr("Error processing depth image: %s", str(e))

def main():
    rospy.init_node("zed2i_depth_subscriber", anonymous=True)
    rospy.Subscriber("/zed2i/zed_node/depth/depth_registered", Image, depth_callback)

    rospy.spin()

if __name__ == "__main__":
    main()
