import dearpygui.dearpygui as dpg
import serial
import collections
import time

ser = serial.Serial(
    port='/dev/cu.usbserial-0001', 
    baudrate=115200,                
    timeout=1
)
Buffer = 300

data_buffers = {
    "MX": collections.deque(maxlen=Buffer),
    "MY": collections.deque(maxlen=Buffer),
    "MZ": collections.deque(maxlen=Buffer),
    
    "GX": collections.deque(maxlen=Buffer),
    "GY": collections.deque(maxlen=Buffer),
    "GZ": collections.deque(maxlen=Buffer)
}

TIME_WINDOW = 5

time_buffer = collections.deque(maxlen=Buffer)
start_time = time.time()

latest_value = [0.0]

def read_serial_data():
    while ser.in_waiting > 0:
        line = ser.readline().decode(errors='ignore').strip()
        

        if ':' in line:
            #Base setup
            ID, val_str = line.split(':', 1)
            ID = ID.strip()
            val_str = val_str.strip()
            try:
                value = float(val_str)
            except ValueError:
                print(f"Ignored non-numeric line: {line}")
                return

            #Data type split
            if ID in data_buffers:
                data_buffers[ID].append(value)
                
                current_time = time.time() - start_time  # Time since start            
                time_buffer.append(current_time)
        
dpg.create_context()

with dpg.window(label="Mango Serial Plot", width=450, height=450, pos=(0,0)):
    dpg.add_text("Reading serial data...")
    dpg.add_separator()
    
    with dpg.plot(label="Axis Data (MX, MY, MZ)", height=400, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="x_axis_M")
        dpg.add_plot_axis(dpg.mvYAxis, label="Value", tag="y_axis_M")
        
        line_series = dpg.add_line_series([], [], label="MX", parent="y_axis_M", tag="MX_data_series")        
        line_series = dpg.add_line_series([], [], label="MZ", parent="y_axis_M", tag="MZ_data_series")        
        line_series = dpg.add_line_series([], [], label="MY", parent="y_axis_M", tag="MY_data_series")        

        dpg.set_axis_limits("y_axis_M", -200, 200)


with dpg.window(label="Gyro Serial Plot", width=450, height=450, pos=(450,0)):
    dpg.add_text("Reading serial data...")
    dpg.add_separator()
    
    with dpg.plot(label="Axis Data (GX, GY, GZ)", height=400, width=-1):
        dpg.add_plot_legend()
        dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="x_axis_G")
        dpg.add_plot_axis(dpg.mvYAxis, label="Value", tag="y_axis_G")
        
        line_series = dpg.add_line_series([], [], label="GX", parent="y_axis_G", tag="GX_data_series")        
        line_series = dpg.add_line_series([], [], label="GZ", parent="y_axis_G", tag="GZ_data_series")        
        line_series = dpg.add_line_series([], [], label="GY", parent="y_axis_G", tag="GY_data_series")
        
        dpg.set_axis_limits("y_axis_G", -200, 200)


def update_display():
    read_serial_data()
    
    if len(time_buffer) > 0:
        current_time = time_buffer[-1]
        min_time = max(0, current_time - TIME_WINDOW)
        
        # Filter to only show recent data within the time window
        recent_indices = [i for i, t in enumerate(time_buffer) if t >= min_time]
        recent_times = [time_buffer[i] for i in recent_indices]
        
        for axis in ["MX", "MY", "MZ", "GX", "GY", "GZ"]:
            # REMOVE the synchronization check - it's too strict
            # Just check if we have any data at all
            if len(data_buffers[axis]) > 0:
                # Make sure we don't try to access out-of-range indices
                valid_indices = [i for i in recent_indices if i < len(data_buffers[axis])]
                valid_times = [time_buffer[i] for i in valid_indices]
                recent_values = [data_buffers[axis][i] for i in valid_indices]
                
                if len(valid_times) > 0 and len(recent_values) > 0:
                    dpg.set_value(f"{axis}_data_series", [valid_times, recent_values])
        
        # Update X-axis limits to create scrolling effect
        dpg.set_axis_limits("x_axis_M", min_time, current_time)
        dpg.set_axis_limits("x_axis_G", min_time, current_time)




dpg.create_viewport(title='Experimental ground station', width=1200, height=1200)
dpg.setup_dearpygui()
dpg.show_viewport()


while dpg.is_dearpygui_running():
    update_display()
    dpg.render_dearpygui_frame()
    time.sleep(0.001)
    
dpg.destroy_context()


    
