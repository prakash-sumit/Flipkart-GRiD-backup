#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String, Int64, Float64
from cv_bridge import CvBridge, CvBridgeError

import time

N = 0

class Barcode_Detection:

    def __init__(self):
        #CamImg_topic = "/zed2i/zed_node/rgb/image_rect_color"
        CamImg_topic = '/cv/box_cropped'
        rospy.Subscriber(CamImg_topic, Image, self.Callback, queue_size=1)
        self.pub = rospy.Publisher('crop', Image, queue_size=5)
        self.pub_barcode_area = rospy.Publisher('/barcode_area', Float64, queue_size=1)

    def Callback(self,img_msg):
        global N
        N += 1

        bridge = CvBridge()
        if(N >= 0):
            #try:
            image = bridge.imgmsg_to_cv2(img_msg, "passthrough")
            print(image.shape)

            lower = np.array([0, 0, 0])
            upper = np.array([179, 26, 255])

            # Create HSV Image and threshold into a range.
            hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
            mask = cv2.inRange(hsv, lower, upper)
            output = cv2.bitwise_and(image, image, mask=mask)
            # cv2.imshow('hello', output)

            # Convert the modified HSV image back to BGR
            # result_image = cv2.cvtColor(output, cv2.COLOR_HSV2BGR)

            # lower_bound = np.array([182, 151, 166])
            lower_bound = np.array([140, 150, 150])
            upper_bound = np.array([255, 255, 255])

            # Create mask within BGR range
            mask = cv2.inRange(output, lower_bound, upper_bound)

            # Set minimum and max BGR values to display
            # lower_bound = np.array([140, 150, 150])
            # upper_bound = np.array([255, 255, 255])
            # # bgr = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

            # # Create mask within BGR range
            # mask = cv2.inRange(image, lower_bound, upper_bound)

            # lower = np.array([0, 0, 0])
            # upper = np.array([179, 34, 255])

            # # Create HSV Image and threshold into a range.
            # hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
            # mask = cv2.inRange(hsv, lower, upper)
            # output_mask = cv2.bitwise_and(image,image, mask= mask)
            # mask = cv2.bitwise_not(mask)
            #mask_area = mask.shape[0]* mask.shape[1]
            #print(mask_area)
            # Find contours in the binary mask
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            #cv2.imwrite('image.png', mask)

            result_image = image.copy()
            total_area = 0.0
            h, w, _ = image.shape
            image_area = h*w

            for contour in contours:
                area = cv2.contourArea(contour)
                #print(area)
                if (area/image_area)*100000 > 4000:  # Adjust this threshold based on your needs
                    total_area += area
                    cv2.drawContours(result_image,[contour], -1, (0,255,0), 2)

            #print(image_area)
            #print(total_area)
            barcode_area = Float64()

            if(float(total_area/image_area) >= 0.156):
                print('barcode is present')
                barcode_area.data = (total_area/image_area) * 100000
            else:
                barcode_area.data = (total_area/image_area) * 100000
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