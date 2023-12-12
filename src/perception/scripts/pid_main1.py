#!/usr/bin/env python3

import rospy
import math
from std_msgs.msg import Float64,String, Int64
from geometry_msgs.msg import Point
from std_msgs.msg import Int32MultiArray


#use dynamic_reconfig with cfg file in src to tune pid coeff

class IDE_code:
    def __init__(self):

        self.dm_task_manager_sub = rospy.Subscriber('/task', Int64, self.task_callback)
        self.current_task = None


    def task_callback(self, task):
        self.current_task = task.data


class PIDMotorControlNode:
    def __init__(self):
        

        self.r_pid = PIDController(0.1, 0.01, 0.01)
        self.theta_pid = PIDController(0.1, 0.01, 0.01)  #pidfunction 
        self.z_pid = PIDController(0.1, 0.01, 0.01)
        
        self.motor_angles_pub = rospy.Publisher('/motor/angle_rotations', Point, queue_size=10) 
       
        # self.forward_job = rospy.Publisher('/dm/info_from_controls', String, queue_size=10)   #task 0:forward done 

        self.target_coordinate_sub = rospy.Subscriber('/motor/target_coordinates', Point, self.coordinates_callback)
        
        self.rate = rospy.Rate(10) 
        self.is_all_motors_ready = False

        self.is_all_motors_ready = False
        self.current_task = None


    def coordinates_callback(self, target_point):
        x_error = target_point.x
        y_error = target_point.y
        z_error = target_point.z

        r_error = math.sqrt(x_error**2 + y_error**2)
        theta_error = math.atan2(y_error, x_error)

        z_rotation = self.z_pid.calculate_output(z_error)
        r_rotation = self.r_pid.calculate_output(r_error)
        theta_rotation = self.theta_pid.calculate_output(theta_error) 

        if self.current_task == 1:
        
            motor_angles = Point()
            
            motor_angles.x = r_rotation * 0.0426    # belt movs 0.0426cm per 1 degree motor angle.    
            motor_angles.y = theta_rotation * 1
            motor_angles.z = z_rotation * 0.04
            
            rospy.loginfo("Motor Angles - R: %.2f, Theta: %.2f, Z: %.2f",
                        motor_angles.x, motor_angles.y, motor_angles.z)  # Print on the console

            self.motor_angles_pub.publish(motor_angles)
            pass
        
       
        
    def run(self):
        while not rospy.is_shutdown():
            rospy.spin()
            self.rate.sleep(self.rate)



class PIDController:                        #pid coeff initialisation for all motors
    def __init__(self, kp, ki, kd):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.prev_error = 0.0
        self.integral = 0.0

    def calculate_output(self, error):
        self.integral += error
        derivative = error - self.prev_error

        output = self.kp * error + self.ki * self.integral + self.kd * derivative
        self.prev_error = error
        return output

if __name__ == '__main__':
        rospy.init_node('pid_motor_control_node', anonymous=True)
        IDE_code_instance = IDE_code()
        Node = PIDMotorControlNode()
        Node.run()
        

# rostopic pub /motor/target_coordinates geometry_msgs/Point "{x: 1000, y: 1000, z: 1000}" 
# use this command to publish the (x,y,z)

#go to mw workspace
# sudo chmod 666 /dev/ttyACM0 
# rosrun rosserial_python serial_node.py /dev/ttyACM0
