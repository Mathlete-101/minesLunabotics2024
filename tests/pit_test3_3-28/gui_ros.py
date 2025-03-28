import rospy
from std_msgs.msg import String
import tkinter as tk
import queue
import json 
def get_dt_state_text(state_code):
    """Converts the drive train state code to a human-readable string."""
    if state_code == 0:
        return "Speed Control"
    elif state_code == 1:
        return "Time Control"
    elif state_code == 2:
        return "Waiting"
    else:
        return "Unknown"  # Handle unexpected state codes

def get_a_state_text(state_code):
    """Converts the actuator state code to a human-readable string."""
    if state_code == 0:
        return "Extending"
    elif state_code == 1:
        return "Extended"
    elif state_code == 2:
        return "Retracting"
    elif state_code == 3:
        return "Retracted"
    elif state_code == -1:
        return "Unknown"
    else:
        return "Invalid State"  # Catch other unexpected states.

def get_db_state_text(state_code):
    """Converts the dig belt state code to a human-readable string."""
    if state_code == 0:
        return "Off"
    elif state_code == 1:
        return "Digging"
    else:
        return "Unknown"  # Handle unexpected state codes

def get_dump_state_text(state_code):
    """Converts the dump belt state code to a human-readable string."""
    if state_code == 0:
        return "Rotating for Loads"
    elif state_code == 1:
        return "Rotating for Time"
    elif state_code == 2:
        return "Dumping"
    elif state_code == 3:
        return "Waiting"
    else:
        return "Unknown"  # Handle unexpected state codes

class RoverGuiNode:
    def __init__(self):
        rospy.init_node('rover_gui_node', anonymous=True)
        self.gui_queue = queue.Queue()
        self.root = tk.Tk()
        self.root.title("Rover Status")
        self.root.geometry("600x500")  # Set window size

        # Dump Belt frame
        self.dump_belt_frame = tk.LabelFrame(self.main_frame, text="Dump Belt", font=("Arial", 16), padx=10, pady=10, bg="lightgray", borderwidth=4)
        self.dump_belt_frame.grid(row=115, column=0, columnspan=4, rowspan=3, pady=5, sticky="ew")

        self.dump_state_label = tk.Label(self.dump_belt_frame, text="State: Waiting", font=("Arial", 12), bg="SeaGreen3")
        self.dump_state_label.grid(row=0, column=0, padx=10, pady=2, sticky="w")

        self.dump_full_label = tk.Label(self.dump_belt_frame, text="Full: FALSE", font=("Arial", 10), bg="steelblue1")
        self.dump_full_label.grid(row=0, column=1, padx=10, pady=2, sticky="w")

        self.dump_loads_label = tk.Label(self.dump_belt_frame, text="Loads: 0", font=("Arial", 10), bg="yellow")
        self.dump_loads_label.grid(row=1, column=1, padx=10, pady=2, sticky="w")

        self.dump_pwm_label = tk.Label(self.dump_belt_frame, text="PWM: 0", font=("Arial", 10), bg="orange2")
        self.dump_pwm_label.grid(row=0, column=2, padx=10, pady=2, sticky="e")

        self.subscriber = rospy.Subscriber("rover_status", String, self.callback) #Subscribing to a topic
        self.update_gui()
        self.root.mainloop()

    def callback(self, data):
        """Callback function to process incoming messages."""
        try:
            message_dict = json.loads(data.data) #Parse JSON
            self.gui_queue.put(message_dict) #Put message in queue
        except json.JSONDecodeError:
            rospy.logerr("Received invalid JSON message")

    def update_gui(self):
        """Updates the GUI with data from the queue."""
        try:
            message = self.gui_queue.get_nowait()

            # update rover operation (remote control/automated)
            # self.status_label.config(text="Latest Update:")

            # Drive Train updates
            state_code = message.get('drive_train', {}).get('get_state', -1)  # using -1 as invalid state.
            self.dt_state_label.config(text=f"State: {get_dt_state_text(state_code)}")
            self.set_speed_label.config(text=f"Speed: {message.get('drive_train', {}).get('set_linearx_mps', 0.0):.3f} m/s")
            self.set_angular_speed_label.config(text=f"Angular Speed: {message.get('drive_train', {}).get('set_angularz_rps', 0.0):.3f} rad/s")
            self.actual_speed_label.config(text=f"Speed: {message.get('drive_train', {}).get('actual_linearx_mps', 0.0):.3f} m/s")
            self.actual_angular_speed_label.config(text=f"Angular Speed: {message.get('drive_train', {}).get('actual_angularz_rps', 0.0):.3f} rad/s")
            self.kp_label.config(text=f"Kp: {message.get('drive_train', {}).get('get_Kp', 0.0)}")
            self.ki_label.config(text=f"Ki: {message.get('drive_train', {}).get('get_Ki', 0.0)}")

            # Actuator updates
            state_code = message.get('actuator', {}).get('get_state', -2)  # using -2 as invalid state.
            self.a_state_label.config(text=f"State: {get_a_state_text(state_code)}")
            self.actuator_extend_pwm_label.config(text=f"Extend PWM: {message.get('actuator', {}).get('get_pwm_extend', 0)}")
            self.actuator_retract_pwm_label.config(text=f"Retract PWM: {message.get('actuator', {}).get('get_pwm_retract', 0)}")

            # Dig Belt updates
            state_code = message.get('dig_belt', {}).get('get_state', -1)  # using -1 as invalid state.
            self.db_state_label.config(text=f"State: {get_db_state_text(state_code)}")
            self.db_speed_label.config(text=f"Speed: {message.get('dig_belt', {}).get('get_speed', 0.0):.3f} RPM")
            self.db_pwm_label.config(text=f"PWM: {message.get('dig_belt', {}).get('get_pwm', 0)}")

            # Dump Belt updates
            state_code = message.get('dump_belt', {}).get('get_state', -1)  # using -1 as invalid state.
            self.dump_state_label.config(text=f"State: {get_dump_state_text(state_code)}")
            self.dump_pwm_label.config(text=f"PWM: {message.get('dump_belt', {}).get('get_pwm', 0)}")
            self.dump_full_label.config(text=f"Full: {message.get('dump_belt', {}).get('get_full', False)}")  # update to say TRUE or FALSE
            self.dump_loads_label.config(text=f"Loads: {message.get('dump_belt', {}).get('get_loads', 0)}")

        except queue.Empty:
            pass
        self.root.after(100, self.update_gui)

if __name__ == '__main__':
    try:
        rover_gui_node = RoverGuiNode()
    except rospy.ROSInterruptException:
        pass