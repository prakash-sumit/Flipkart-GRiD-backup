#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge, CvBridgeError

import time

#N = 0

class Barcode_Detection:

    def __init__(self):
        CamImg_topic = "/zed2i/zed_node/left/image_rect_color"
        rospy.Subscriber(CamImg_topic, Image, self.Callback)
        self.pub = rospy.Publisher('crop', Image, queue_size=5)

    def Callback(self,img_msg):
        #global N
        #N += 1

        bridge = CvBridge()
        #if(N >= 0):
        try:
            img = bridge.imgmsg_to_cv2(img_msg, "bgr8")
            #img = img[80:280, 150:330]
            #cap.set(3,640)
            #cap.set(4,480)
            #print(img.shape)
            img = img[130:230, 240:400]
            img = bridge.cv2_to_imgmsg(img, "bgr8")
            self.pub.publish(img)

        except CvBridgeError as e:
            print(e)
            return
            
        '''else:
            #N += 1
            pass'''
            

if __name__ == '__main__':
    try:
        rospy.init_node("crop_test_node")
        obj = Barcode_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated.")