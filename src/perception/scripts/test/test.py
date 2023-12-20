#!/usr/bin/env python3

import cv2
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge

# Initialize ROS node
rospy.init_node('image_subscriber', anonymous=True)

# Global variables for image and CvBridge
img = None
bridge = CvBridge()

# Callback function to process incoming images
def image_callback(msg):
    global img
    try:
        # Convert ROS Image message to OpenCV format
        img = bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")

        # Display the image
        cv2.imshow('image', img)
        cv2.waitKey(1)

        # Your original click_event function
        def click_event(event, x, y, flags, params):
            if event == cv2.EVENT_LBUTTONDOWN:
                print(x, ' ', y)
                font = cv2.FONT_HERSHEY_SIMPLEX
                cv2.putText(img, str(x) + ',' + str(y), (x, y), font, 1, (255, 0, 0), 2)
                cv2.imshow('image', img)

            if event == cv2.EVENT_RBUTTONDOWN:
                print(x, ' ', y)
                font = cv2.FONT_HERSHEY_SIMPLEX
                b = img[y, x, 0]
                g = img[y, x, 1]
                r = img[y, x, 2]
                cv2.putText(img, str(b) + ',' + str(g) + ',' + str(r), (x, y), font, 1, (255, 255, 0), 2)
                cv2.imshow('image', img)

        # Set mouse handler for the image
        cv2.setMouseCallback('image', click_event)

    except Exception as e:
        print(e)

# Subscribe to the ROS image topic (replace 'your_image_topic' with the actual topic name)
rospy.Subscriber('/zed2i/zed_node/left/image_rect_color', Image, image_callback)

# Main loop to keep the script running
while not rospy.is_shutdown():
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down...")
        break

# Close OpenCV window
cv2.destroyAllWindows()
