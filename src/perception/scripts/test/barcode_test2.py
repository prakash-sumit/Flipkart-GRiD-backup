#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge, CvBridgeError
from pyzbar.pyzbar import decode

import time

N = 0

class Barcode_Detection:

    def __init__(self):
        CamImg_topic = "/zed2i/zed_node/left/image_rect_color"
        rospy.Subscriber(CamImg_topic, Image, self.Callback)
        self.pub = rospy.Publisher('Barcode_Publisher_2', String, queue_size=5)

    def Callback(self,img_msg):
        global N
        N += 1

        bridge = CvBridge()
        if(N >= 0):
            try:
                img = bridge.imgmsg_to_cv2(img_msg, "bgr8")
                #img = img[80:280, 150:330]
                
                detectedBarcodes = decode(img)
                loc = []
                data_to_pub = String()
	
                # If not detected then print the message
                if not detectedBarcodes:
                    print("Barcode Not Detected or your barcode is blank/corrupted!")
                    data_to_pub.data = "Barcode Not Detected or your barcode is blank/corrupted!"

                else:
                
                    # Traverse through all the detected barcodes in image
                    for barcode in detectedBarcodes:
                    
                        # Locate the barcode position in image
                        (x, y, w, h) = barcode.rect
                        loc.append((x,y,w,h))
                        print(x,' ',y,' ',w,' ',h)
                        print('hh')

                    data_to_pub.data = 'barcode found'
                        
                self.pub.publish(data_to_pub)
                
                #N += 1
                #time.sleep(1)

            except CvBridgeError as e:
                print(e)
                return
            
        else:
            #N += 1
            pass
            

if __name__ == '__main__':
    try:
        rospy.init_node("Barcode_Detection_Node_2")
        obj = Barcode_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated.")