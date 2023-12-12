#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge, CvBridgeError

import time

N = 0

class Barcode_Detection:

    def __init__(self):
        CamImg_topic = "/camera1/image_raw"
        rospy.Subscriber(CamImg_topic, Image, self.Callback)
        self.pub = rospy.Publisher('crop', Image, queue_size=5)

    def Callback(self,img_msg):
        global N
        N += 1

        bridge = CvBridge()
        if(N >= 0):
            #try:
            image = bridge.imgmsg_to_cv2(img_msg, "passthrough")

            gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

            # equalize lighting
            clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))
            gray = clahe.apply(gray)

            # edge enhancement
            edge_enh = cv2.Laplacian(gray, ddepth = cv2.CV_8U, 
                                    ksize = 3, scale = 1, delta = 0)

            # bilateral blur, which keeps edges
            blurred = cv2.bilateralFilter(edge_enh, 13, 50, 50)

            # use simple thresholding. adaptive thresholding might be more robust
            (_, thresh) = cv2.threshold(blurred, 55, 255, cv2.THRESH_BINARY)
            

            # do some morphology to isolate just the barcode blob
            kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (9, 9))
            closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)
            closed = cv2.erode(closed, None, iterations = 4)
            closed = cv2.dilate(closed, None, iterations = 4)
            

            # find contours left in the image
            '''(_, cnts, _) = cv2.findContours(closed.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            c = sorted(cnts, key = cv2.contourArea, reverse = True)[0]
            rect = cv2.minAreaRect(c)
            box = np.int0(cv2.boxPoints(rect))
            cv2.drawContours(image, [box], -1, (0, 255, 0), 3)
            print(box)
            cv2.imshow("found barcode", image)
            cv2.imwrite('closed.jpg ', image)
            cv2.waitKey(0)
            retval = cv2.imwrite("found.jpg", image)'''

            #self.pub.publish(data_to_pub)
            img = bridge.cv2_to_imgmsg(closed, "passthrough")
            self.pub.publish(img)
                
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
        rospy.loginfo("node terminated.")