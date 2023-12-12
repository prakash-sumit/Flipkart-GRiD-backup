#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
from sensor_msgs.msg import Image
from std_msgs.msg import String, Int64
from geometry_msgs.msg import Point
from cv_bridge import CvBridge, CvBridgeError

import time
import torch


class Box_Detection:

    def __init__(self):
        self.model = torch.hub.load('/home/sedrica/Flipkart-GRiD/src/perception/scripts/yolov5', 'custom', source = 'local', path = '/home/sedrica/Flipkart-GRiD/src/perception/scripts/yolov5/best.pt', force_reload=True)
        #Arm_cam_topic = "/zed2i/zed_node/rgb/image_rect_color"
        Arm_cam_topic = "zed_camera/image"
        rospy.Subscriber('/task', Int64, self.task_Callback)
        rospy.Subscriber(Arm_cam_topic, Image, self.Callback)
        #rospy.Subscriber("/zed2i/zed_node/depth/depth_registered", Image, self.depth_callback)
        rospy.Subscriber("zed_camera/depth", Image, self.depth_callback)
        Tray_cam_topic = '/camera1/image_raw'
        rospy.Subscriber(Tray_cam_topic, Image, self.tray_Callback)
        self.pub_cropped_img = rospy.Publisher('/cv/box_cropped', Image, queue_size=5)
        self.pub_target_coordinates = rospy.Publisher('/actual_coordinates', Point, queue_size=5)
        self.task = None # for testing

    def task_Callback(self, task):
        self.task = task.data
    
    def depth_callback(self, data):
        try:
            bridge = CvBridge()
            # Convert the depth image to a OpenCV format
            self.depth_image = bridge.imgmsg_to_cv2(data, desired_encoding="passthrough")
            
        except Exception as e:
            rospy.logerr("Error processing depth image: %s", str(e))

    def Callback(self,img_msg):
        if(self.task == 1): 
            bridge = CvBridge()
            #try:
            image = bridge.imgmsg_to_cv2(img_msg, "passthrough")
            h, w, _ = image.shape

            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            # cv2.imshow('d',image)
            # cv2.waitKey(0)
            img = [image]
            results = self.model(img)
            coordinates = results.xyxyn[0][:,:-1]
            filtered_boxes = coordinates[coordinates[:, 4] > 0.8, :4] # filter boxes by score > 0.6

            # Calculate midpoints of bounding boxes
            midpoints = []

            for box in filtered_boxes:
                x1, y1, x2, y2 = box.tolist()
                midpoint_x = (x1 + x2) / 2
                midpoint_y = (y1 + y2) / 2
                midpoints.append([int(midpoint_x * w), int(midpoint_y * h)])  # multiplying for conversion to pixel coordinates
            
            print(midpoints)

            closest_point_depth = 1000000
            closest_point = []
            
            try:
                for i in range(len(midpoints)):
                    if(self.depth_image[midpoints[i][1], midpoints[i][0]].item() < closest_point_depth):  # 1,0 coz conversion from x-y to pixel coordinate system
                        closest_point = midpoints[i]
                        closest_point_depth = self.depth_image[midpoints[i][1], midpoints[i][0]].item()
                        index = i

                target_coordinate = Point()
                target_coordinate.x = closest_point[0]
                target_coordinate.y = closest_point[1]
                target_coordinate.z = self.depth_image[closest_point[1], closest_point[0]]  # 1,0 coz conversion from x-y to pixel coordinate system

                # pub_target_coordinates.publish(target_coordinate)

                x1, y1, x2, y2 = filtered_boxes[index].tolist()
                #print(np.array(image).shape)
                cropped_image = image[int(y1*h):int(y2*h+1), int(x1*w):int(x2*w+1)]  # multiplying for conversion to pixel coordinates
                
                # circle_radius = 10
                # circle_color = (0, 0, 255)  # Red color in BGR format
                # circle_thickness = 2

                # for point in midpoints:
                #     cv2.circle(image, point, circle_radius, circle_color, circle_thickness)

                cropped_image = bridge.cv2_to_imgmsg(cropped_image, "passthrough")

                if(target_coordinate.z > 30): # in cm
                    self.pub_cropped_img.publish(cropped_image)
                    self.pub_target_coordinates.publish(target_coordinate)
                
                # self.pub_cropped_img.publish(cropped_image)

                self.task = None
                #time.sleep(0.5) # ??
            except:
                print('No box detected')
            
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
        rospy.init_node("Box_Detection_Node")
        obj = Box_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated")