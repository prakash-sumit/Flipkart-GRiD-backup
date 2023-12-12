#!/usr/bin/env python3

# import rospy
# import sys
# from std_msgs.msg import String

# #from BoxChecking import is_box
# #from BarcodeChecking import is_barcode

# class DMNode():
#     def __init__(self):

#         self.task_topic_pub = rospy.Publisher('/task', String, queue_size = 1)
#         self.rate = rospy.Rate(10)

#         #self.task = 0
#         self.info_list = {'Reached_trolley_top': 0, 'Box_picked': 1, 'Reached_tray': 2, 'barcode_found': 3}
#         self.task_list = {'Start': 0, 'Forward': 1, 'Backward_to_tray': 2, 'Find_barcode': 3, 'Backward_to_final': 4}
#         print('h')
#         #self.task_topic_pub.publish('Start')


#     def info_callback(self, task):
#         self.info = self.info_list[task.data]
#         #self.info = self.info_list[task.data]
        
#         if(self.info == 0):
#             #self.is_box = is_box()
#             self.is_box = True

#             if not self.is_box:
#                 sys.exit()

#             #self.is_barcode = is_barcode()
#             self.is_barcode = True

#             self.task_topic_pub.publish('Forward')

#         elif (self.info == 1):
#             if self.is_barcode :
#                 self.task_topic_pub.publish('Backward_to_final')

#             else :
#                 self.task_topic_pub.publish('Backward_to_tray')

#         elif (self.info == 2):
#             self.task_topic_pub.publish('Find_barcode')

#         elif (self.info == 3):
#             self.task_topic_pub.publish('Backward_to_final')

#         self.rate.sleep()

        
#     def run_algo(self):
#         print('helo')
#         info_topic = '/dm/info_from_controls'
#         #self.task_topic_pub.publish('Forward')
#         task = String()
#         task.data = 'Forward'
#         self.task_topic_pub.publish(task)
#         print('helo2')
#         info_topic_sub = rospy.Subscriber(info_topic, String, self.info_callback, queue_size = 1)
        
        
# if __name__ == '__main__':
#     #print("starting the node")
#     try:
#         print("initialinzing node")
#         rospy.init_node("DMNode")
#         print("creating obejct")
#         obj = DMNode()
#         obj.run_algo()
#         print("entering while loop")
#         rospy.spin()

#     except rospy.ROSInterruptException:
#         rospy.loginfo("node terminated")

import rospy
from std_msgs.msg import String
import sys

class DMNode():
    def __init__(self):
        self.task_topic_pub = rospy.Publisher('/task', String, queue_size=1)
        rospy.init_node("DMNode")
        self.rate = rospy.Rate(10)
        self.publish_topic('Start')
        #print('hello')
        #self.task_to_pub = 'Forward'
        
        #self.task = 0
        self.info_list = {'Reached_trolley_top': 0, 'Box_picked': 1, 'Reached_tray': 2, 'Barcode_found': 3}
        self.task_list = {'Start': 0, 'Forward': 1, 'Backward_to_tray': 2, 'Find_barcode': 3, 'Backward_to_final': 4}
        self.run_algo()


    def publish_topic(self, data):
        #rate = rospy.Rate(1)  # Set a rate of 1 Hz (1 message per second)
        
        task = String()
        task.data = data
        self.task_topic_pub.publish(task)
        rospy.loginfo(f"Published {data} message to /task topic")
        self.rate.sleep()

    def info_callback(self, task):
        self.info = self.info_list[task.data]
        
        if(self.info == 0):
            #self.is_box = is_box()
            self.is_box = True
            
            if not self.is_box:
                sys.exit()

            #self.is_barcode = is_barcode()
            self.is_barcode = True
            self.task_to_pub = 'Forward'

        elif (self.info == 1):
            if self.is_barcode :
                self.task_to_pub = 'Backward_to_final'

            else :
                self.task_to_pub = 'Backward_to_tray'

        elif (self.info == 2):
            self.task_to_pub = 'Find_barcode'

        elif (self.info == 3):
            self.task_to_pub = 'Backward_to_final'

        self.publish_topic(self.task_to_pub)
        self.rate.sleep()

    def run_algo(self):
        info_topic = '/dm/info_from_controls'
        self.info_topic_sub = rospy.Subscriber(info_topic, String, self.info_callback, queue_size = 1)

        rospy.spin()  # Keep the node spinning

if __name__ == '__main__':
    try:
        obj = DMNode()
    except rospy.ROSInterruptException:
        rospy.loginfo("Node terminated")
