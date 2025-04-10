import tkinter as tk
import queue


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
        return "Invalid State" #Catch other unexpected states.
    
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

# # GUI thread setup
# gui_queue = queue.Queue()

def init_gui(gui_queue):
    root = tk.Tk()
    root.title("Rover Status")
    root.geometry("600x500")  # Set window size

    # Main frame for organization
    main_frame = tk.Frame(root, bg="snow")
    main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

    # Status label
    status_label = tk.Label(main_frame, text="Remote Control", font=("Arial", 14), bg="snow")
    status_label.grid(row=0, column=0, columnspan=4, pady=10, sticky="ns")

    # Drive Train frame 
    drive_train_frame = tk.LabelFrame(main_frame, text="Drivetrain", font=("Arial", 16), padx=10, pady=10, bg="lightgray", borderwidth=4)
    drive_train_frame.grid(row=1, column=0, columnspan=4, rowspan=4, pady=5, sticky="we")

    dt_state_label = tk.Label(drive_train_frame, text="State: Waiting", font=("Arial", 12), bg="SeaGreen3")
    dt_state_label.grid(row=0, column=0, padx=10, pady=2, sticky="w")

    set_speed_label = tk.Label(drive_train_frame, text="Set Speed: 0.0 m/s", font=("Arial", 10), bg="steelblue1")
    set_speed_label.grid(row=0, column=1, padx=10, pady=2, sticky="w")

    actual_speed_label = tk.Label(drive_train_frame, text="Actual Speed: 0.0 m/s", font=("Arial", 10), bg="yellow")
    actual_speed_label.grid(row=2, column=1, padx=10, pady=2, sticky="w")

    set_angular_speed_label = tk.Label(drive_train_frame, text="Set Angular Speed: 0.0 rad/s", font=("Arial", 10), bg="steelblue1")
    set_angular_speed_label.grid(row=0, column=2, padx=10, pady=2, sticky="w")

    actual_angular_speed_label = tk.Label(drive_train_frame, text="Actual Angular Speed: 0.0 rad/s", font=("Arial", 10), bg="yellow")
    actual_angular_speed_label.grid(row=2, column=2, padx=10, pady=2, sticky="w")

    kp_label = tk.Label(drive_train_frame, text="Kp: 0", font=("Arial", 10), bg="magenta2")
    kp_label.grid(row=0, column=3, padx=10, pady=2, sticky="w")

    ki_label = tk.Label(drive_train_frame, text="Ki: 0", font=("Arial", 10), bg="magenta2")
    ki_label.grid(row=2, column=3, padx=10, pady=2, sticky="w")


    # Actuator frame
    actuator_frame = tk.LabelFrame(main_frame, text="Actuator", font=("Arial", 16), padx=10, pady=10, bg="lightgray", borderwidth=4)
    actuator_frame.grid(row=5, column=0, columnspan=4, rowspan=3, pady=5, sticky="ew")

    a_state_label = tk.Label(actuator_frame, text="State: Waiting", font=("Arial", 12), bg="SeaGreen3")
    a_state_label.grid(row=0, column=0, padx=10, pady=2, sticky="w")

    actuator_extend_pwm_label = tk.Label(actuator_frame, text="Extend PWM: 0", font=("Arial", 10), bg="orange2")
    actuator_extend_pwm_label.grid(row=0, column=1, padx=10, pady=2)

    actuator_retract_pwm_label = tk.Label(actuator_frame, text="Retract PWM: 0", font=("Arial", 10), bg="orange2")
    actuator_retract_pwm_label.grid(row=0, column=2, padx=10, pady=2, sticky="e")

    # Dig Belt frame
    dig_belt_frame = tk.LabelFrame(main_frame, text="Dig Belt", font=("Arial", 16), padx=10, pady=10, bg="lightgray", borderwidth=4)
    dig_belt_frame.grid(row=10, column=0, columnspan=4, rowspan=3, pady=5, sticky="ew")

    db_state_label = tk.Label(dig_belt_frame, text="State: Waiting", font=("Arial", 12), bg="SeaGreen3")
    db_state_label.grid(row=0, column=0, padx=10, pady=2, sticky="w")

    db_speed_label = tk.Label(dig_belt_frame, text="Speed: 0 RPM", font=("Arial", 10), bg="yellow")
    db_speed_label.grid(row=0, column=1, padx=10, pady=2, sticky="w")

    db_pwm_label = tk.Label(dig_belt_frame, text="PWM: 0", font=("Arial", 10), bg="orange2")
    db_pwm_label.grid(row=0, column=2, padx=10, pady=2, sticky="e")

    # Dump Belt frame
    dump_belt_frame = tk.LabelFrame(main_frame, text="Dump Belt", font=("Arial", 16), padx=10, pady=10, bg="lightgray", borderwidth=4)
    dump_belt_frame.grid(row=115, column=0, columnspan=4, rowspan=3, pady=5, sticky="ew")

    dump_state_label = tk.Label(dump_belt_frame, text="State: Waiting", font=("Arial", 12), bg="SeaGreen3")
    dump_state_label.grid(row=0, column=0, padx=10, pady=2, sticky="w")

    dump_full_label = tk.Label(dump_belt_frame, text="Full: FALSE", font=("Arial", 10), bg="steelblue1")
    dump_full_label.grid(row=0, column=1, padx=10, pady=2, sticky="w")

    dump_loads_label = tk.Label(dump_belt_frame, text="Loads: 0", font=("Arial", 10), bg="yellow")
    dump_loads_label.grid(row=1, column=1, padx=10, pady=2, sticky="w")

    dump_pwm_label = tk.Label(dump_belt_frame, text="PWM: 0", font=("Arial", 10), bg="orange2")
    dump_pwm_label.grid(row=0, column=2, padx=10, pady=2, sticky="e")

    def update_gui():
        try:
            message = gui_queue.get_nowait()

            # update rover operation (remote control/automated)
            # status_label.config(text="Latest Update:")

            # Drive Train updates
            state_code = message.get('drive_train', {}).get('get_state', -1) # using -1 as invalid state.
            dt_state_label.config(text=f"State: {get_dt_state_text(state_code)}")
            set_speed_label.config(text=f"Speed: {message.get('drive_train', {}).get('set_linearx_mps', 0.0):.3f} m/s")
            set_angular_speed_label.config(text=f"Angular Speed: {message.get('drive_train', {}).get('set_angularz_rps', 0.0):.3f} rad/s")
            actual_speed_label.config(text=f"Speed: {message.get('drive_train', {}).get('actual_linearx_mps', 0.0):.3f} m/s")
            actual_angular_speed_label.config(text=f"Angular Speed: {message.get('drive_train', {}).get('actual_angularz_rps', 0.0):.3f} rad/s")
            kp_label.config(text=f"Kp: {message.get('drive_train', {}).get('get_Kp', 0.0)}")
            ki_label.config(text=f"Ki: {message.get('drive_train', {}).get('get_Ki', 0.0)}")

            # Actuator updates
            state_code = message.get('actuator', {}).get('get_state', -2) # using -2 as invalid state.
            a_state_label.config(text=f"State: {get_a_state_text(state_code)}")
            actuator_extend_pwm_label.config(text=f"Extend PWM: {message.get('actuator', {}).get('get_pwm_extend', 0)}")
            actuator_retract_pwm_label.config(text=f"Retract PWM: {message.get('actuator', {}).get('get_pwm_retract', 0)}")

            # Dig Belt updates
            state_code = message.get('dig_belt', {}).get('get_state', -1) # using -1 as invalid state.
            db_state_label.config(text=f"State: {get_db_state_text(state_code)}")
            db_speed_label.config(text=f"Speed: {message.get('dig_belt', {}).get('get_speed', 0.0):.3f} RPM")
            db_pwm_label.config(text=f"PWM: {message.get('dig_belt', {}).get('get_pwm', 0)}")

            # Dump Belt updates
            state_code = message.get('dump_belt', {}).get('get_state', -1) # using -1 as invalid state.
            dump_state_label.config(text=f"State: {get_dump_state_text(state_code)}")
            dump_pwm_label.config(text=f"PWM: {message.get('dump_belt', {}).get('get_pwm', 0)}")
            dump_full_label.config(text=f"Full: {message.get('dump_belt', {}).get('get_full', False)}")  # update to say TRUE or FALSE
            dump_loads_label.config(text=f"Loads: {message.get('dump_belt', {}).get('get_loads', 0)}")

        except queue.Empty:
            pass
        root.after(100, update_gui)

    update_gui()
    root.mainloop()

