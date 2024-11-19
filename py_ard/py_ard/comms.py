import rclpy
from rclpy.node import Node
import serial
from time import sleep
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy
from nav_msgs.msg import Odometry

import time, threading

class printser():
    def write(self, message):
        print(message)
    def __getattr__(self, name):
        pass
    def reset_input_buffer(self):
        pass
    def reset_output_buffer(self):
        pass


class VelocityComm(Node):
    cmd_x = 0
    cmd_y = 0
    cmd_z = 0

    wheelToAdjust = 4
    adjustAmount = 0

    ConveyorButton = 0
    DeployButton = 0
    DigLinButton = 0
    DigBeltButton = 0
    BucketConveyor = 0
    digActivate = 0

    _ConveyorButton = 0
    _DeployButton = 0
    _DigLinButton = 0
    _DigBeltButton = 0
    _BucketConveyor = 0
    _digActivate = 0

    firstTime = True

    x = 0
    y = 0
    z= 0

    x_offset = -.877 #check before comp and update
    y_offset = .296 #

    cmd_vel_from_cmd_vel = True
    dig = 0
    prevDig = 0

    xBerm = 6.10
    yBerm = 4.57-1.43
    lengthLARI = 1.44

    UWB_pose = Odometry()

    state = 'deploy'
    keep_dig = False

    ArenaLength = 8.14
    ArenaHeight = 4.57
    plunge = True

    plungeTime = 60 #120 for 11in sus

    def __init__(self):
        super().__init__('Comm')
        # set up serial connection to arduino
        try:
            self.ser = serial.Serial('/dev/ttyACM0', baudrate=115200, timeout=0)
            self.ser.reset_input_buffer()
        except Exception as e:
            print("Could not connect to arduino :(")
            print(e)
            self.ser = printser() 

        self.subscription2 = self.create_subscription(
            Joy,
            'joy',
            self.CallMe,
            10)
        self.subscription = self.create_subscription(
            Twist,
            'cmd_vel',
            self.listener_callback,
            10)
        self.globalOdom_sub = self.create_subscription(Odometry, '/globalOdom', self.globalOdom_callback, 10) #TODO change odom call back to globalOdom
        self.comms()

        self.subscription
        self.subscription2   
    def listener_callback(self, msg):
        #self.get_logger().info('I heard: "%s"' % msg.linear.x)
        self.cmd_x = msg.linear.x
        self.cmd_y = msg.linear.y
        self.cmd_z = msg.angular.z
            

    def globalOdom_callback(self, msg):
        """Callback function for odometry subscriber."""
        self.UWB_pose = msg
        self.get_logger().info('I heard: "%s"' % self.UWB_pose.pose.pose.position.x)


    def CallMe(self, msg):
        self._ConveyorButton = (msg.buttons[2]-msg.buttons[3])*255
        self._DeployButton = float(msg.buttons[6]-msg.buttons[7])*255
        self._DigLinButton += float(-1*(msg.axes[7])*30) #int(abs(msg.axes[2]-1)*255/2)
        self._DigBeltButton = float(abs(msg.axes[5]-1)*255/2)
        self._BucketConveyor = float(msg.buttons[5])*255
        self._digActivate = float(msg.buttons[4])
        self.dig = msg.buttons[8]
        self.wheelToAdjust += msg.axes[6]
        self.adjustAmount += msg.axes[0]
        if self.wheelToAdjust > 3 or self.wheelToAdjust < 3:
            self.wheelToAdjust = 4
        else:
            print("controlling wheel: " + str(self.wheelToAdjust) + " by: " + str(self.adjustAmount))
        if self.adjustAmount > 255:
            self.adjustAmount = 255
        elif self.adjustAmount < -255:
            self.adjustAmount = -255
            
        if self.dig and self.dig != self.prevDig:
            self.dig_cycle()
            self.prevDig = self.dig

    def comms(self):
        if self.firstTime == True:
            self.get_logger().info('waiting for arduino')
            sleep(5)
            self.get_logger().info('arduino ready')
            self.firstTime = False  

        if self.cmd_vel_from_cmd_vel:
            self.x = self.cmd_x
            self.y = self.cmd_y
            self.z = self.cmd_z
            self.ConveyorButton = self._ConveyorButton
            self.DeployButton = self._DeployButton
            self.DigLinButton = self._DigLinButton
            self.DigBeltButton = self._DigBeltButton
            self.BucketConveyor = self._BucketConveyor
            self.digActivate = self._digActivate
            

        if self.ser != None:

            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()
            start = "<"
            finish = str(">")
            print(self.x, self.y, self.z)
            self.ser.write(start.encode())
            self.ser.write(str(float(self.x)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.y)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.z)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.digActivate)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.ConveyorButton)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.DeployButton)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.DigLinButton)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.DigBeltButton)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.BucketConveyor)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.wheelToAdjust)).encode())
            self.ser.write(str(',').encode())
            self.ser.write(str(float(self.adjustAmount)).encode())
            self.ser.write(finish.encode())
        
        threading.Timer(.1, self.comms).start()


    def dig_cycle(self):
        print("Dig Cycle")
        if self.state == 'deploy':
            self.cmd_vel_from_cmd_vel = False
            self.digActivate = 1 #wheels go 90
            self.ConveyorButton = -254 #slide out moves forward
            self.x = 0 # speed is 0
            self.y = 0
            self.z = 0
            sleep(20) #TODO adjusself.cmd_vel_from_cmd_vel = Falset time or add feedback from PI
            self.ConveyorButton = 0 #stop the slide outs
            self.DeployButton = 254
            sleep(7)
            self.DeployButton = 0

            #self.BucketConveyor = 254
            currTime = time.time()
            
            self.plunge = True
            self.state = 'dig'
            self.cmd_vel_from_cmd_vel = True
            return
        
        elif self.state == 'dig':
            self.cmd_vel_from_cmd_vel = False
            self.get_logger().info('DIG')   
            self.DigLinButton = 254
            sleep(3)
            self.DigLinButton = 0

            self.digActivate = 1
            self.DigBeltButton = 254 
            currTime = time.time()


            while time.time() - currTime < self.plungeTime: #TODO change to more accurate timing?

                if self.plunge == True:
                    self.get_logger().info('self.Plunge On')
                    self.DigLinButton = 60
                    self.BucketConveyor = 254
                    self.plunge = False
                else:
                    self.get_logger().info('self.Plunge Off')
                    self.DigLinButton = 0
                    self.BucketConveyor = 0
                    self.plunge = True
                time.sleep(3)  # Wait for 3 seconds between state changes

            self.DigLinButton = -254
            self.DigBeltButton = 0 
            self.BucketConveyor = 0

            time.sleep(int((self.plungeTime*60)/254)) #TODO TIME to retract self.plunger
            self.x = .8
            time.sleep(1)
            self.x = 0.0

            self.cmd_vel_from_cmd_vel = True
            return
        

def main(args=None):
    rclpy.init(args=args)

    velocityComm = VelocityComm()
    #buttonPressComm = ButtonPressComm()
    #minimal_subscriber.create_rate(10)
    rclpy.spin(velocityComm)
    #rclpy.spin(buttonPressComm)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    if self.ser != None:
        self.ser.close()
    velocityComm.destroy_node()
    #buttonPressComm.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
