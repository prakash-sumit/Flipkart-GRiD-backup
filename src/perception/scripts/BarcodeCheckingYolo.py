#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String, Int64, Float64
from cv_bridge import CvBridge, CvBridgeError

import time
import torch

N = 0

class Barcode_Detection:

    def __init__(self):
        self.model = torch.hub.load('/home/sedrica/Flipkart-GRiD-backup/src/perception/scripts/yolov5', 'custom', source = 'local', path = '/home/sedrica/Flipkart-GRiD-backup/src/perception/scripts/yolov5/barcode_model_final/best.pt', force_reload=True)
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
            h, w, _ = image.shape
            image_area = h*w
            # image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            # cv2.imshow('d',image)
            # cv2.waitKey(0)
            result_image = image.copy()
            img = [image]
            results = self.model(img)
            coordinates = results.xyxyn[0][:,:-1]
            print(coordinates)
            if len(coordinates.tolist()) != 0:
                i = -1
                highest_score = 0
                index = 0
                for box in coordinates:
                    # print(box)
                    x1, y1, x2, y2, score = box.tolist()
                    # print(score)
                    i = i+1
                    if highest_score < score:
                        highest_score = score
                        index = i
                    
                main_box = coordinates.tolist()[index]
                # print(main_box)
                x1, y1, x2, y2, _ = main_box
                total_area = (y2 - y1)*h*(x2 - x1)*w
                cv2.rectangle(result_image, (int(x1*w), int(y1*h)), (int(x2*w), int(y2*h)), (0,255,0), 2)
            else:
                total_area = 0.0
            
            barcode_area = Float64()

            if(float(total_area) >= 400):
                print('barcode is present')
                barcode_area.data = total_area#(total_area/image_area) * 100000
            else:
                barcode_area.data = total_area#(total_area/image_area) * 100000
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