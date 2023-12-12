#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String, Int64
from cv_bridge import CvBridge, CvBridgeError

import time

class Barcode_Detection:

    def __init__(self):
        Arm_cam_topic = "/zed2i/zed_node/rgb/image_rect_color"
        rospy.Subscriber('/task', Int64, self.task_Callback)
        rospy.Subscriber(Arm_cam_topic, Image, self.Callback)
        Tray_cam_topic = '/camera1/image_raw'
        rospy.Subscriber(Tray_cam_topic, Image, self.tray_Callback)
        self.pub_cropped_img = rospy.Publisher('/cv/box_cropped', Image, queue_size=5)
        self.pub_target_coordinates = rospy.Publisher('/motor/target_coordinates', Int64, queue_size=5)

    def task_Callback(self, task):
        self.task = task.data

    def Callback(self,img_msg):
        if(self.task == 1): 
            bridge = CvBridge()
            #try:
            image = bridge.imgmsg_to_cv2(img_msg, "passthrough")

            pipeline_outputs = yolo_pipeline(images = image, iou_thres = 0.6, conf_thres = 0.001)

            #process the pipeline_outputs and publish the cropped image and target midpoint coordinates
                
                #N += 1
                #time.sleep(1)

            '''except CvBridgeError as e:
                print(e)
                return'''
        else:
            pass

    def tray_Callback(self, img_msg):
        if(self.task == 3):
            self.pub_cropped_img.publish(img_msg)

            '''except CvBridgeError as e:
                print(e)
                return'''
        else:
            pass
            

if __name__ == '__main__':
    try:
        rospy.init_node("Barcode_Detection_Node")
        obj = Barcode_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated")