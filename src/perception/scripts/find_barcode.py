#!/usr/bin/env python3

# import rospy
# from std_msgs.msg import Float64, Int64

# previous_area = 0.0

# class Barcode_Detection:

#     def __init__(self):
#         rospy.Subscriber('/barcode_area', Float64, self.Callback, queue_size=5)
#         rospy.Subscriber('/yaw_task', Int64, self.task_Callback, queue_size=5)
#         self.pub = rospy.Publisher('/yaw_confirm', Int64, queue_size=5)
#         # self.yaw_arduino_pub = rospy.Publisher('/yaw_arduino_task', Int64, queue_size=5)
#         self.task = None
#         # self.task = 69  # for testing
#         self.timer = rospy.Timer(rospy.Duration(0.5), self.timer_callback)
#         self.area = None

#     def task_Callback(self, task):
#         self.task = task.data
#         if(self.task == 69):
#             yaw_arduino_task = Int64()
#             yaw_arduino_task.data = 100
#             self.pub.publish(yaw_arduino_task)

#     def timer_callback(self, timer):
#         if self.area != None:
#             global previous_area

#             if(self.task == 69):
#                 self.task_to_controls = Int64()
#                 self.current_area = self.area

#                 if(self.current_area - previous_area >= -2500):
#                     # self.task_to_controls.data = 0  # rotate
#                     pass

#                 else:
#                     self.task_to_controls.data = 1  # stop and send controls barcode is found
#                     self.pub.publish(self.task_to_controls)
#                     self.task = None

#                 previous_area = self.current_area

#             else:
#                 pass
#         else:
#             pass

#     def Callback(self, area):
#         self.area = area.data
         

# if __name__ == '__main__':
#     try:
#         rospy.init_node("Barcode_Finding_Node")
#         obj = Barcode_Detection()
#         rospy.spin()

#     except rospy.ROSInterruptException:
#         rospy.loginfo("node terminated.")

import rospy
from std_msgs.msg import Float64, Int64

# previous_area = 0.0

class Barcode_Detection:

    def __init__(self):
        rospy.Subscriber('/barcode_area', Float64, self.Callback, queue_size=1)
        rospy.Subscriber('/yaw_task', Int64, self.task_Callback, queue_size=5)
        
        self.pub = rospy.Publisher('/yaw_confirm', Int64, queue_size=5)
        # self.yaw_arduino_pub = rospy.Publisher('/yaw_arduino_task', Int64, queue_size=5)
        self.task = None
        # self.counter = 0
        # self.task = 69  # for testing

    def task_Callback(self, task):
        self.task = task.data
        if(self.task == 69):
            self.max_area = 0.0
            yaw_arduino_task = Int64()
            yaw_arduino_task.data = 100
            self.pub.publish(yaw_arduino_task)
        elif self.task == 10:
            yaw_arduino_task = Int64()
            yaw_arduino_task.data = 10
            self.pub.publish(yaw_arduino_task)
        
    def Callback(self, area):
        # global previous_area

        if(self.task == 69):
            self.task_to_controls = Int64()
            self.current_area = area.data

            if(self.current_area - float(self.max_area) >= -2000):
                # self.task_to_controls.data = 0  # rotate
                print(self.current_area - self.max_area)

            else:
                # self.task_to_controls.data = 1  # stop and send controls barcode is found
                # self.pub.publish(self.task_to_controls)
                self.task = None
            
            if(self.current_area > self.max_area):
                self.max_area = self.current_area

        else:
            pass
         

if __name__ == '__main__':
    try:
        rospy.init_node("Barcode_Finding_Node")
        obj = Barcode_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated.")
