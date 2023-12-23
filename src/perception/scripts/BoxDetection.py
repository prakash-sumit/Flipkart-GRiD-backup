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
        self.model = torch.hub.load('/home/sedrica/Flipkart-GRiD-backup/src/perception/scripts/yolov5', 'custom', source = 'local', path = '/home/sedrica/Flipkart-GRiD-backup/src/perception/scripts/yolov5/runs/train/exp14/weights/best.pt', force_reload=True)
        self.model2 = torch.hub.load('/home/sedrica/Flipkart-GRiD-backup/src/perception/scripts/yolov5', 'custom', source = 'local', path = '/home/sedrica/Flipkart-GRiD-backup/src/perception/scripts/yolov5/runs/train/exp/weights/best.pt', force_reload=True)
        # Arm_cam_topic = "/zed2i/zed_node/left_raw/image_rect_color"
        Arm_cam_topic = "zed_camera/image"
        rospy.Subscriber('/task', Int64, self.task_Callback)
        rospy.Subscriber(Arm_cam_topic, Image, self.Callback)
        # rospy.Subscriber("/zed2i/zed_node/depth/depth_registered", Image, self.depth_callback)
        rospy.Subscriber("zed_camera/depth", Image, self.depth_callback)
        Tray_cam_topic = '/camera1/image_raw'
        rospy.Subscriber(Tray_cam_topic, Image, self.tray_Callback)
        self.pub_cropped_img = rospy.Publisher('/cv/box_cropped', Image, queue_size=5)
        self.pub_target_coordinates = rospy.Publisher('/motor/target_coordinates', Point, queue_size=5)
        rospy.Subscriber('/yaw_confirm', Int64, self.yaw_task_Callback, queue_size=5)
        # self.pub_target_coordinates = rospy.Publisher('/actual_coordinates', Point, queue_size=5)
        self.task = None
        self.yaw_task = None
        # self.yaw_task = 100
        # self.task = 1
        # self.task = 3 # for testing
        self.pub_display = rospy.Publisher('/display_picture', Image, queue_size=1)
        self.tray_counter = 0
        # self.detect()

    def yaw_task_Callback(self, data):
        self.yaw_task = data.data
    
    def undistort(self, image):
        camera_matrix = None
        dist_coeffs = None
        camera_matrix = np.array([[703.3630, 0,584.4595], [0, 704.3759, 308.9572], [0, 0, 1.000]])
        dist_coeffs = np.array([[0.0064, -0.2324, 0., 0., 0.]])
        width, height = np.shape(image)[1], np.shape(image)[0] 
        newcameramatrix, _ = cv2.getOptimalNewCameraMatrix(camera_matrix, dist_coeffs, (width, height), 1, (width, height))
        undistorted_image = cv2.undistort(image, camera_matrix, dist_coeffs, None, newcameramatrix)
        return undistorted_image

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
            # image = self.undistort(image)
            h, w, _ = image.shape
            print(image.shape)

            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            # cv2.imshow('d',image)
            # cv2.waitKey(0)
            img = [image]
            results = self.model(img)
            coordinates = results.xyxyn[0][:,:-1]
            filtered_boxes = coordinates[coordinates[:, 4] > 0.6, :4] # filter boxes by score > 0.6
            print(coordinates)
            print(filtered_boxes)

            # Calculate midpoints of bounding boxes
            midpoints = []

                        # Define the color of the bounding box (in BGR format)
            color = (0, 255, 0)  # Green in BGR

            # Define the line thickness of the bounding box
            thickness = 2
            
            image2 = image.copy()

            for box in filtered_boxes:
                x1, y1, x2, y2 = box.tolist()
                print(y1)
                cv2.rectangle(image2, (int(x1*w), int(y1*h)), (int(x2*w), int(y2*h)), color, thickness)
                midpoint_x = (x1 + x2) / 2
                midpoint_y = (y1 + y2) / 2
                midpoints.append([int(midpoint_x * w), int(midpoint_y * h)])  # multiplying for conversion to pixel coordinates
            
            print(midpoints)

            closest_point_depth = 1000000
            closest_point = []
            index = 0
            i =0
            # try:
            for i in range(len(midpoints)):
                if(self.depth_image[midpoints[i][1], midpoints[i][0]] < closest_point_depth):  # 1,0 coz conversion from x-y to pixel coordinate system
                    closest_point = midpoints[i]
                    closest_point_depth = self.depth_image[midpoints[i][1], midpoints[i][0]]
                    index = i
                    print(i)
            print(self.depth_image[midpoints[i][1], midpoints[i][0]])

            print(closest_point)
            target_coordinate = Point()
            target_coordinate.z = self.depth_image[closest_point[1], closest_point[0]]/10   # 1,0 coz conversion from x-y to pixel coordinate system
            target_coordinate.x = (616 - closest_point[0])*(0.0018283*target_coordinate.z + 0.0043574)    # conversion factor
            target_coordinate.y = (365 - closest_point[1])*(0.0018283*target_coordinate.z + 0.0043574)    # conversion factor
            # target_coordinate.x = closest_point[0]
            # target_coordinate.y = closest_point[1]

            cv2.circle(image2, (closest_point[0], closest_point[1]), 20, (255,0,0), thickness=2, lineType=8, shift=0)
            # pub_target_coordinates.publish(target_coordinate)

            x1, y1, x2, y2 = filtered_boxes[index].tolist()
            #print(np.array(image).shape)
            cropped_image = image[int(y1*h):int(y2*h+1), int(x1*w):int(x2*w+1)]  # multiplying for conversion to pixel coordinates
            cv2.circle(cropped_image, (closest_point[0], closest_point[1]), 10, (255,0,0), thickness=2, lineType=8, shift=0)
            # circle_radius = 10
            # circle_color = (0, 0, 255)  # Red color in BGR format
            # circle_thickness = 2

            # for point in midpoints:
            #     cv2.circle(image, point, circle_radius, circle_color, circle_thickness)
            image2 = cv2.cvtColor(image2, cv2.COLOR_BGR2RGB)
            cropped_image = cv2.cvtColor(cropped_image, cv2.COLOR_BGR2RGB)
            image2 = bridge.cv2_to_imgmsg(image2, "passthrough")
            cropped_image = bridge.cv2_to_imgmsg(cropped_image, "passthrough")


            if(target_coordinate.z > 30): # in cm
                self.pub_cropped_img.publish(cropped_image)
                self.pub_display.publish(image2)
                self.pub_target_coordinates.publish(target_coordinate)
            
            # self.pub_cropped_img.publish(cropped_image)

            self.task = None  # comment for testing
            #time.sleep(0.5) # ??
            # except:
            #     print('No box detected')
            
            #process the pipeline_outputs and publish the cropped image and target midpoint coordinates
                
                #N += 1
                #time.sleep(1)
            
            '''except CvBridgeError as e:
                print(e)
                return'''
        else:
            pass

    def tray_Callback(self, img_msg):
        if (self.yaw_task == 100):
            self.tray_image = img_msg
            print('hi')
            bridge = CvBridge()
            try:
                image = bridge.imgmsg_to_cv2(self.tray_image, "passthrough")
                start_time = time.time()
                h, w, _ = image.shape

                image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                # cv2.imshow('d',image)
                # cv2.waitKey(0)
                img = [image]
                results = self.model2(img)
                coordinates = results.xyxyn[0][:,:-1]
                # print(coordinates)
                
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
                # print(x1)

                cropped_image = image[int(y1*h+ 20):int(y2*h-20), int(x1*w+40):int(x2*w-40)]  # multiplying for conversion to pixel coordinates

                image_pub = bridge.cv2_to_imgmsg(cropped_image, "passthrough")

                self.pub_cropped_img.publish(image_pub)
                end_time = time.time()
                print('time taken: ', (end_time-start_time))

            except Exception as e:
                print(e)
        else:
            pass

    # def detect(self):
    #     print('hello')
        
    #     while(self.task == 3):
    #         print('hiiiii')
    #         bridge = CvBridge()
    #         try:
    #             image = bridge.imgmsg_to_cv2(self.tray_image, "passthrough")
    #             start_time = time.time()
    #             h, w, _ = image.shape

    #             image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    #             # cv2.imshow('d',image)
    #             # cv2.waitKey(0)
    #             img = [image]
    #             results = self.model2(img)
    #             coordinates = results.xyxyn[0][:,:-1]
    #             # print(coordinates)
                
    #             i = -1
    #             highest_score = 0
    #             index = 0
    #             for box in coordinates:
    #                 # print(box)
    #                 x1, y1, x2, y2, score = box.tolist()
    #                 # print(score)
    #                 i = i+1
    #                 if highest_score < score:
    #                     highest_score = score
    #                     index = i
                    
    #             main_box = coordinates.tolist()[index]
    #             # print(main_box)
    #             x1, y1, x2, y2, _ = main_box
    #             # print(x1)

    #             cropped_image = image[int(y1*h):int(y2*h+1), int(x1*w):int(x2*w+1)]  # multiplying for conversion to pixel coordinates

    #             image_pub = bridge.cv2_to_imgmsg(cropped_image, "passthrough")

    #             self.pub_cropped_img.publish(image_pub)
    #             end_time = time.time()
    #             print('time taken: ', (end_time-start_time))

    #         except Exception as e:
    #             print(e)
                
            

if __name__ == '__main__':
    try:
        rospy.init_node("Box_Detection_Node_New")
        # print('hello')
        obj = Box_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated")