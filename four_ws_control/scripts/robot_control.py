#!/usr/bin/python3
import math
import threading
import rclpy
import numpy as np
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy

#GOAL USE THIS

vel_msg = Twist()  # robot velosity
mode_selection = 0 # 1:opposite phase, 2:in-phase, 3:pivot turn 4: none
# vel in m/s
# pos in rads
class Commander(Node):

    def __init__(self):
        super().__init__('commander')
        timer_period = 0.02
        self.wheel_seperation = .557784
        self.wheel_base = .808482
        self.wheel_radius = 0.155575
        self.wheel_steering_y_offset = 0.00 #CHANGE MAYBE?
        self.steering_track = self.wheel_seperation - 2*self.wheel_steering_y_offset

        self.pos = np.array([0,0,0,0], float)
        self.vel = np.array([0,0,0,0], float) #left_front, right_front, left_rear, right_rear

        self.pub_pos = self.create_publisher(Float64MultiArray, '/forward_position_controller/commands', 10) 
        self.pub_vel = self.create_publisher(Float64MultiArray, '/forward_velocity_controller/commands', 10)
        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        global vel_msg, mode_selection

        # opposite phase
        if(mode_selection == 1):
            
            vel_steerring_offset = vel_msg.angular.z * self.wheel_steering_y_offset
            sign = np.sign(vel_msg.linear.x)
            
            self.vel[0] = sign*math.hypot(vel_msg.linear.x - vel_msg.angular.z*self.steering_track/2, vel_msg.angular.z*self.wheel_base/2) - vel_steerring_offset
            self.vel[1] = sign*math.hypot(vel_msg.linear.x + vel_msg.angular.z*self.steering_track/2, vel_msg.angular.z*self.wheel_base/2) + vel_steerring_offset
            self.vel[2] = sign*math.hypot(vel_msg.linear.x - vel_msg.angular.z*self.steering_track/2, vel_msg.angular.z*self.wheel_base/2) - vel_steerring_offset
            self.vel[3] = sign*math.hypot(vel_msg.linear.x + vel_msg.angular.z*self.steering_track/2, vel_msg.angular.z*self.wheel_base/2) + vel_steerring_offset
            if (2*vel_msg.linear.x + vel_msg.angular.z*self.steering_track) == 0.0:
                self.pos[0] = 0.0 
                self.pos[1] = 0.0
                self.pos[2] = 0.0
                self.pos[3] = 0.0
            else:
                self.pos[0] = math.atan(vel_msg.angular.z*self.wheel_base/(2*vel_msg.linear.x + vel_msg.angular.z*self.steering_track))
                self.pos[1] = math.atan(vel_msg.angular.z*self.wheel_base/(2*vel_msg.linear.x - vel_msg.angular.z*self.steering_track))
                self.pos[2] = -self.pos[0]
                self.pos[3] = -self.pos[1]

        # in-phase
        elif(mode_selection == 2):

            V = math.hypot(vel_msg.linear.x, vel_msg.linear.y)
            sign = np.sign(vel_msg.linear.x)
            
            if(vel_msg.linear.x != 0):
                ang = vel_msg.linear.y / vel_msg.linear.x
            else:
                ang = 0
            
            self.pos[0] = math.atan(ang)
            self.pos[1] = math.atan(ang)
            self.pos[2] = self.pos[0]
            self.pos[3] = self.pos[1]
            
            self.vel[:] = sign*V
            
        # pivot turn
        elif(mode_selection == 3):

            self.pos[0] = -math.atan(self.wheel_base/self.steering_track)
            self.pos[1] = math.atan(self.wheel_base/self.steering_track)
            self.pos[2] = math.atan(self.wheel_base/self.steering_track)
            self.pos[3] = -math.atan(self.wheel_base/self.steering_track)
            
            self.vel[0] = -vel_msg.angular.z
            self.vel[1] = vel_msg.angular.z
            self.vel[2] = self.vel[0]
            self.vel[3] = self.vel[1]

        else:

            self.pos[:] = 0
            self.vel[:] = 0

        pos_array = Float64MultiArray(data=self.pos) 
        vel_array = Float64MultiArray(data=self.vel) 
        self.pub_pos.publish(pos_array)
        self.pub_vel.publish(vel_array)
        self.pos[:] = 0
        self.vel[:] = 0

class Joy_subscriber(Node):

    def __init__(self):
        super().__init__('joy_subscriber')
        self.subscription = self.create_subscription(
            Joy,
            'joy',
            self.listener_callback,
            10)
        self.subscription

    def listener_callback(self, data):
        global vel_msg, mode_selection
            #CHANGE 1. UPDATE TO UR BUTTONS 
        if(data.buttons[0] == 1):   # in-phase # A button of Xbox 360 controller
            mode_selection = 2
        elif(data.buttons[2] == 1): # opposite phase # X button of Xbox 360 controller
            mode_selection = 1
        elif(data.buttons[1] == 1): # pivot turn # B button of Xbox 360 controller
            mode_selection = 3
        else:
            mode_selection = 4

class Cmd_vel_subscriber(Node):

    def __init__(self):
        super().__init__('cmd_vel_subscriber')
        self.subscription = self.create_subscription(
            Twist,
            'cmd_vel',
            self.listener_callback,
            10)
        self.subscription

    def listener_callback(self, msg):
        global vel_msg, mode_selection

        vel_msg.linear.x = msg.linear.x
        vel_msg.linear.y = msg.linear.y
        vel_msg.angular.z = msg.angular.z


if __name__ == '__main__':
    rclpy.init(args=None)
    
    commander = Commander()
    joy_subscriber = Joy_subscriber()
    cmd_vel_subscriber = Cmd_vel_subscriber()

    executor = rclpy.executors.MultiThreadedExecutor()
    executor.add_node(commander)
    executor.add_node(joy_subscriber)
    executor.add_node(cmd_vel_subscriber)

    executor_thread = threading.Thread(target=executor.spin, daemon=True)
    executor_thread.start()
    rate = commander.create_rate(3)
    try:
        while rclpy.ok():
            rate.sleep()
    except KeyboardInterrupt:
        pass
    
    rclpy.shutdown()
    executor_thread.join()

