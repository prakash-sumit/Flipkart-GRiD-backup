#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Point
from std_msgs.msg import Int64, Int64MultiArray
import math

class MotorControlNode:
    def __init__(self):
        rospy.Subscriber('/task', Int64, self.task_callback, queue_size=1)
        self.task = None
        rospy.Subscriber('/info_from_arduino', Int64, self.info_callback, queue_size=5)
        self.pub_to_dm = rospy.Publisher('/dm/info_from_controls', Int64, queue_size=5)
        self.curr_poss = 42.39  # distance from suction centre to shaft centre        #42.1 before
        self.motor_angles_pub = rospy.Publisher('/motor/target_angles', Int64, queue_size=1)
        self.motor_target_sub = rospy.Subscriber('/motor/target_coordinates', Point, self.motor_target_callback, queue_size=5)
        self.suck_drop = rospy.Subscriber('/suction_drop', Int64, self.suck_drop_callback, queue_size=5)
        self.yaw_pub = rospy.Publisher('/yaw_confirm', Int64, queue_size=5)
        
        self.steps_x = None
        self.steps_y = None
        self.steps_z = None
        
        self.confirmation = 0
            
    def suck_drop_callback(self, data):
        self.suction_drop_msg = data.data
        if (self.suction_drop_msg == 1503):
            self.yaw_pub.publish(1503)

        else:
            pass
            
        
    def task_callback(self, data):
        self.task = data.data
        print("new task came from dm")
        if (self.task == 0):
            self.pub_to_dm.publish(0)
            
        elif (self.task == 1):
            while (self.steps_x == None):
                # print('No position from cv')
                pass
                
            # angles = [None]*4
            # angles[0] = self.task
            # angles[1] = int(self.steps_x)
            # angles[2] = int(self.steps_y)
            # angles[3] = int(self.steps_z)
            
            # angles_to_pub = Int64MultiArray()
            # angles_to_pub.data = angles
            
            # packed_data = (int(self.steps_z) << 27) | (int(self.steps_y) << 15) | (int(self.steps_x) << 3) | self.task
            
            x,y,z = '','',''
            t = str(self.task)
            if(self.steps_x < 0):
                m = str(abs(self.steps_x))
                x = '1'+'0'*(4-len(m))+m
            else:
                m = str(abs(self.steps_x))
                x = '0'+'0'*(4-len(m))+m
            if(self.steps_y < 0):
                m = str(abs(self.steps_y))
                y = '1'+'0'*(4-len(m))+m
            else:
                m = str(abs(self.steps_y))
                y = '0'+'0'*(4-len(m))+m
            if(self.steps_z < 0):
                m = str(abs(self.steps_z))
                z = '1'+'0'*(4-len(m))+m
            else:
                m = str(abs(self.steps_z))
                z = '0'+'0'*(4-len(m))+m
            encoded_string = t+x+y+z
            # print(encoded_string, len(encoded_string))
            encoded_number = int(encoded_string)
            
            angles_to_pub = Int64()
            angles_to_pub.data = encoded_number
            # angles_to_pub.data = self.task
            print('hello')
            
            self.motor_angles_pub.publish(angles_to_pub)
                
        elif (self.task == 4):
            # angles = [None]*4
            # angles[0] = self.task
            # angles[1] = int(self.steps_x)
            # angles[2] = int(self.steps_y)
            # angles[3] = int(self.steps_z)
            # print(angles)
            # angles_to_pub = Int64MultiArray()
            # angles_to_pub.data = angles
            # for i in range(5):
            #     self.motor_angles_pub.publish(angles_to_pub)
            #     print("publish kiya")
            
            if (self.steps_x != None):
                print("executing task 4")
                # angles = [None]*4
                # angles[0] = self.task
                # angles[1] = int(self.steps_x)
                # angles[2] = int(self.steps_y)
                # angles[3] = int(self.steps_z)
                
                # angles_to_pub = Int64MultiArray()
                # angles_to_pub.data = angles
                # packed_data = (int(self.steps_z) << 27) | (int(self.steps_y) << 15) | (int(self.steps_x) << 3) | self.task  
                
                x,y,z = '','',''
                t = str(self.task)
                if(self.steps_x < 0):
                    m = str(abs(self.steps_x))
                    x = '1'+'0'*(4-len(m))+m
                else:
                    m = str(abs(self.steps_x))
                    x = '0'+'0'*(4-len(m))+m
                if(self.steps_y < 0):
                    m = str(abs(self.steps_y))
                    y = '1'+'0'*(4-len(m))+m
                else:
                    m = str(abs(self.steps_y))
                    y = '0'+'0'*(4-len(m))+m
                if(self.steps_z < 0):
                    m = str(abs(self.steps_z))
                    z = '1'+'0'*(4-len(m))+m
                else:
                    m = str(abs(self.steps_z))
                    z = '0'+'0'*(4-len(m))+m
                encoded_string = t+x+y+z
                # print(encoded_string, len(encoded_string))
                encoded_number = int(encoded_string)
                
                angles_to_pub = Int64()
                angles_to_pub.data = encoded_number

                self.motor_angles_pub.publish(angles_to_pub)
            else:
                print('No position from cv')

        elif (self.task == 3):
            # angles = [None]*4
            # angles[0] = self.task
            # angles[1] = int(self.steps_x)
            # angles[2] = int(self.steps_y)
            # angles[3] = int(self.steps_z)
            # print(angles)
            # angles_to_pub = Int64MultiArray()
            # angles_to_pub.data = angles
            # for i in range(5):
            #     self.motor_angles_pub.publish(angles_to_pub)
            #     print("publish kiya")
            
            if (self.steps_x != None):
                print("executing task 3")
                # angles = [None]*4
                # angles[0] = self.task
                # angles[1] = int(self.steps_x)
                # angles[2] = int(self.steps_y)
                # angles[3] = int(self.steps_z)
                
                # angles_to_pub = Int64MultiArray()
                # angles_to_pub.data = angles
                # packed_data = (int(self.steps_z) << 27) | (int(self.steps_y) << 15) | (int(self.steps_x) << 3) | self.task  
                
                x,y,z = '','',''
                t = str(self.task)
                if(self.steps_x < 0):
                    m = str(abs(self.steps_x))
                    x = '1'+'0'*(4-len(m))+m
                else:
                    m = str(abs(self.steps_x))
                    x = '0'+'0'*(4-len(m))+m
                if(self.steps_y < 0):
                    m = str(abs(self.steps_y))
                    y = '1'+'0'*(4-len(m))+m
                else:
                    m = str(abs(self.steps_y))
                    y = '0'+'0'*(4-len(m))+m
                if(self.steps_z < 0):
                    m = str(abs(self.steps_z))
                    z = '1'+'0'*(4-len(m))+m
                else:
                    m = str(abs(self.steps_z))
                    z = '0'+'0'*(4-len(m))+m
                encoded_string = t+x+y+z
                # print(encoded_string, len(encoded_string))
                encoded_number = int(encoded_string)
                
                angles_to_pub = Int64()
                angles_to_pub.data = encoded_number

                self.motor_angles_pub.publish(angles_to_pub)
            else:
                print('No position from cv')
                                
    def info_callback(self, data):
        self.confirmation = data.data
        
        if (self.task == 1 and self.confirmation == 1):
            self.pub_to_dm.publish(1)
            self.confirmation = 0
        elif (self.task == 4  and self.confirmation == 1):
            self.steps_x = None
            self.steps_y = None
            self.steps_z = None
            
            self.pub_to_dm.publish(0)
            self.confirmation = 0
        elif (self.task == 3 and self.confirmation == 1):
            self.steps_x = None
            self.steps_y = None
            self.steps_z = None
                
            self.pub_to_dm.publish(0)
            self.confirmation = 0
        elif (self.confirmation == 0):
            return

    def calculate_polar_coordinates(self, x_wrt_centre,y_wrt_centre):

        # theta_angle = math.atan2(y_wrt_centre, self.curr_poss + x_wrt_centre)
        # theta_deg = (180.0 / 3.1415926) * theta_angle
        # r = ((x_wrt_centre + self.curr_poss) / math.cos(theta_angle)) - self.curr_poss
        # return theta_deg, r
        a = self.curr_poss + x_wrt_centre
        if y_wrt_centre >= 0 :
            b = y_wrt_centre + 5.4   # distance of suction centre to horizontal member centre #3.5 before
            theta_angle = math.acos(4.6 / math.sqrt (a*a + b*b)) - math.acos(b / math.sqrt (a*a + b*b))
            theta_deg = (180.0 / 3.1415926) * theta_angle
            r = (a + 5.4* math.sin(theta_angle))/math.cos(theta_angle) - self.curr_poss          
            return theta_deg, r
    
        if y_wrt_centre <0 :
            y_wrt_centre = -(y_wrt_centre)
            b = y_wrt_centre - 5.4     # distance of suction centre to horizontal member centre
            theta_angle = math.asin((10.8*a + math.sqrt (b*b*a*a + b**4 - 29.16 *b*b))/ (2*(a*a + b*b))) 
            theta_deg = (180.0 / 3.1415926) * theta_angle
            theta_deg = -(theta_deg)
            r = (a - 5.4* math.sin(theta_angle))/math.cos(theta_angle) - self.curr_poss
            if theta_deg > 0 :
                theta_deg = 0
                r = 0
            return theta_deg, r
        
    def motor_target_callback(self, data):
        x_wrt_centre = data.x + 8    # distance of left sensor to suction centre in x
        y_wrt_centre = data.y + 10.5     # distance of left sensor to suction centre in y
        z = data.z - 23.5 + 1.5   # distance of suction to zed camera and 0.5 add to ensure suction gets stuck to box 
        
        theta, r = self.calculate_polar_coordinates(x_wrt_centre,y_wrt_centre)
        
        # self.steps_x = math.floor(r / 0.0384) # for 400 steps 
        self.steps_x = math.floor(r / 0.01863933) # for 800 steps 
        # self.steps_x = math.floor(r / 0.0768)   # for 200 steps 
        # self.steps_x = math.floor(r / 0.0024) # for 6400 steps
        
        self.steps_y = math.ceil(theta * 2.222)   # for 800 steps
        
        # self.steps_z = math.floor((z - 20.32) / 0.018) # for 800 steps
        # self.steps_z = math.floor((z - 20 + 1) / 0.036) # for 400 steps          # -33 

        # if self.steps_x >0 and self.steps_y<0:
        #     if z > 75 :
        #         z+=0.5
        #     self.steps_x += 25
        # elif self.steps_x <0 and self.steps_y>0:
        #     self.steps_x += 250
        #     self.steps_y += 26
        #     z -= 1.1
        # elif self.steps_x >0 and self.steps_y>0:
        #     self.steps_x += 120
        #     self.steps_y += 16
        #     z-= 1.3
        # elif self.steps_x >0 and self.steps_y<0:
        #     z+=0.5
        #     self.steps_x += 25

        # self.steps_z = math.floor((z - 20.32) / 0.036) # for 400 steps          # -33
        self.steps_z = math.floor((z) / 0.018) # for 800 steps          # -33 
        # # self.steps_z = math.floor((z - 20 + 1.1) / 0.072) # for 200 steps        # -33 
        # # self.steps_z = math.floor((z - 20.32) / 0.00225)  # for 6400 steps
        

if __name__ == '__main__':
    rospy.init_node('motor_control_node')
    motor_control_node = MotorControlNode()
    rospy.spin()
    
