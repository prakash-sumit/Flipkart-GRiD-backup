#!/usr/bin/env python3

import pyzed.sl as sl
import cv2
import numpy as np
import rospy
from cv_bridge import CvBridge
from sensor_msgs.msg import Image   
from geometry_msgs.msg import Point

class Zed:
    def __init__(self):
        
        # Create a Camera object
        self.zed = sl.Camera()
        image_publisher = rospy.Publisher('zed_camera/image', Image, queue_size=1)
        depth_publisher = rospy.Publisher('zed_camera/depth', Image, queue_size=1)
        self.pub_target_coordinates = rospy.Publisher('/motor/target_coordinates', Point, queue_size=5)
        bridge = CvBridge()
        # Create a InitParameters object and set configuration parameters
        init_params = sl.InitParameters()
        init_params.camera_resolution = sl.RESOLUTION.AUTO # Use HD720 opr HD1200 video mode, depending on camera type.
        init_params.camera_fps = 30  # Set fps at 30
        init_params.depth_mode = sl.DEPTH_MODE.NEURAL  # Use ULTRA depth mode
        init_params.coordinate_units = sl.UNIT.MILLIMETER  # Use meter units (for depth measurements)

        # Open the camera
        err = self.zed.open(init_params)
        rospy.Subscriber('/actual_coordinates', Point, self.Callback)
        if err != sl.ERROR_CODE.SUCCESS:
            rospy.logerr("Camera Open: " + repr(err) + ". Exit program.")
            return
        

        # Capture 50 frames and stop
        i = 0
        image = sl.Mat()
        depth = sl.Mat()
        self.point_cloud = sl.Mat()

        runtime_parameters = sl.RuntimeParameters()
        while not rospy.is_shutdown():
            # Grab an image, a RuntimeParameters object must be given to grab()
            if self.zed.grab(runtime_parameters) == sl.ERROR_CODE.SUCCESS:
                # A new image is available if grab() returns SUCCESS
                self.zed.retrieve_image(image, sl.VIEW.LEFT)
                timestamp = self.zed.get_timestamp(sl.TIME_REFERENCE.CURRENT)  # Get the timestamp at the time the image was captured
                print("Image resolution: {0} x {1} || Image timestamp: {2}\n".format(image.get_width(), image.get_height(),
                    timestamp.get_milliseconds()))
                self.img = np.array(image.get_data())
                self.zed.retrieve_measure(depth, sl.MEASURE.DEPTH)
                # cv2.imshow('image', self.img)
                # cv2.setMouseCallback('image', self.click_event)
                # cv2.waitKey(0)
                
                # cv2.destroyAllWindows()
                #print(depth.get_data()[y][x])
                # depth_image = depth.get_data()
                # depth_x = depth_image[100,100]
                # print(type(depth_x.item()))
                dep = bridge.cv2_to_imgmsg(depth.get_data(), "passthrough")
                
                depth_publisher.publish(dep)
                # print(type(self.img))
                img = bridge.cv2_to_imgmsg(self.img, "passthrough")
                
                image_publisher.publish(img)
        # Close the camera
        self.zed.close()
        
    def Callback(self, data):
        # Get the 3D point cloud values for pixel (i, j)
        self.zed.retrieve_measure(self.point_cloud, sl.MEASURE.XYZRGBA)
        point3D = self.point_cloud.get_value(int(data.x),int(data.y))
        x = point3D[1][0]
        y = point3D[1][1]
        z = point3D[1][2]
        print('x: ',x)
        print('y: ',y)
        print('z: ',z)
        result = Point()
        # result.x = - (x+5.958512306213379)/10  # negative sign for axes calibration for controls
        result.x = -x/10
        # result.y = - (y-4.249844551086426)/10  # negative sign for axes calibration for controls
        result.y = -y/10
        result.z = z/10
        self.pub_target_coordinates.publish(result)  # in cm

    def click_event(self, event, x, y, flags, params):
        if event == cv2.EVENT_LBUTTONDOWN:
            print(x, ' ', y)
            font = cv2.FONT_HERSHEY_SIMPLEX
            cv2.putText(self.img, str(x) + ',' + str(y), (x, y), font, 1, (255, 0, 0), 2)
            # cv2.imshow('image', self.img)

        if event == cv2.EVENT_RBUTTONDOWN:
            print(x, ' ', y)
            font = cv2.FONT_HERSHEY_SIMPLEX
            b = self.img[y, x, 0]
            g = self.img[y, x, 1]
            r = self.img[y, x, 2]
            cv2.putText(self.img, str(b) + ',' + str(g) + ',' + str(r), (x, y), font, 1, (255, 255, 0), 2)
            # cv2.imshow('image', self.img)

        

if __name__ == "__main__":
    try:
        rospy.init_node("Zed_node")
        obj = Zed()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated")
