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
        CamImg_topic = "/zed2i/zed_node/rgb/image_rect_color"
        rospy.Subscriber(CamImg_topic, Image, self.Callback)
        self.pub = rospy.Publisher('crop', Image, queue_size=2)

    def Callback(self,img_msg):
        global N
        N += 1

        bridge = CvBridge()
        if(N >= 0):
            try:
                img = bridge.imgmsg_to_cv2(img_msg, "bgr8")
                #img = img[130:230, 240:400]
                #print(img.shape)
                cv2.imwrite('/home/umic/zed_images_new/box{}.jpg'.format(N), img)
                
                print('image taken{}'.format(N))
                time.sleep(5)
                
                #img = img[80:280, 150:330]
                #cap.set(3,640)
                #cap.set(4,480)
                bardet = cv2.barcode_BarcodeDetector()
                #bardet = cv2.barcode.BarcodeDetector()
                data_to_pub = String()

                '''decoded_info, decoded_type, corners = bardet.detectAndDecode(img)
                print(decoded_info+'\n')

                if(len(decoded_info)!=0):
                    print('barcode found')
                    data_to_pub.data = 'barcode found'
                else:
                    print('barcode not found')
                    data_to_pub.data = 'barcode not found'
                '''
                #self.pub.publish(data_to_pub)
                img = bridge.cv2_to_imgmsg(img, "bgr8")
                self.pub.publish(img)
                
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
        rospy.init_node("Barcode_Detection_Node")
        obj = Barcode_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated.")