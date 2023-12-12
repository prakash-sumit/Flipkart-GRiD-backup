#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String, Int64
from cv_bridge import CvBridge, CvBridgeError

import time

N = 0

class Barcode_Detection:

    def __init__(self):
        #CamImg_topic = "/zed2i/zed_node/rgb/image_rect_color"
        CamImg_topic = '/cv/box_cropped'
        rospy.Subscriber(CamImg_topic, Image, self.Callback, queue_size=1)
        self.pub = rospy.Publisher('crop', Image, queue_size=5)
        self.pub_barcode_area = rospy.Publisher('/barcode_area', Int64, queue_size=5)

    def Callback(self,img_msg):
        global N
        N += 1

        bridge = CvBridge()
        if(N >= 0):
            #try:
            image = bridge.imgmsg_to_cv2(img_msg, "passthrough")

            lower = np.array([0, 46, 0])
            upper = np.array([179, 255, 255])

            # Create HSV Image and threshold into a range.
            hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
            mask = cv2.inRange(hsv, lower, upper)
            output_mask = cv2.bitwise_and(image,image, mask= mask)
            mask = cv2.bitwise_not(mask)
            #mask_area = mask.shape[0]* mask.shape[1]
            #print(mask_area)
            # Find contours in the binary mask
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            #cv2.imwrite('image.png', mask)

            result_image = image.copy()
            total_area = 0.0
            for contour in contours:
                area = cv2.contourArea(contour)
                #print(area)
                total_area += area
                if area > 100:  # Adjust this threshold based on your needs
                    cv2.drawContours(result_image,[contour], -1, (0,255,0), 2)

            h, w, _ = image.shape
            image_area = h*w
            #print(image_area)
            #print(total_area)
            barcode_area = Int64()

            if(float(total_area/image_area) >= 0.1):
                print('barcode is present')
                barcode_area.data = int(total_area)
            else:
                barcode_area.data = 0
                print('barcode not present')

            #cv2.imwrite('contour.png', result_image)

            
            #self.pub.publish(data_to_pub)
            img = bridge.cv2_to_imgmsg(result_image, "passthrough")
            self.pub.publish(img)
            self.pub_barcode_area.publish(barcode_area)
                
                #N += 1
                #time.sleep(1)

            '''except CvBridgeError as e:
                print(e)
                return'''
            
        else:
            #N += 1
            pass
            

if __name__ == '__main__':
    try:
        rospy.init_node("Barcode_Detection_Node")
        obj = Barcode_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated")