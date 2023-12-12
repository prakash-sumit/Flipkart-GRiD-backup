#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float64, Int64

previous_area = 0.0

class Barcode_Detection:

    def __init__(self):
        rospy.Subscriber('/barcode_area', Float64, self.Callback, queue_size=5)
        rospy.Subscriber('/task', Int64, self.task_Callback, queue_size=5)
        self.pub = rospy.Publisher('/task_for_yaw_servo', Int64, queue_size=5)
        self.task = 3  # for testing

    def task_Callback(self, task):
        self.task = task.data

    def Callback(self, area):
        global previous_area

        if(self.task == 3):
            self.task_to_controls = Int64()
            self.current_area = area.data

            if(self.current_area >= previous_area):
                self.task_to_controls.data = 1  # rotate

            else:
                self.task_to_controls.data = 0  # stop

            previous_area = self.current_area
            self.pub.publish(self.task_to_controls)

        else:
            pass
         

if __name__ == '__main__':
    try:
        rospy.init_node("Barcode_Finding_Node")
        obj = Barcode_Detection()
        rospy.spin()

    except rospy.ROSInterruptException:
        rospy.loginfo("node terminated.")