# --- 1. Imports ---
from flask import Flask, render_template, request, redirect, url_for
import paho.mqtt.client as mqtt
import firebase_admin
from firebase_admin import credentials, db 
from dotenv import load_dotenv
import os


load_dotenv()

# --- 2. Configurations ---
MQTT_BROKER = os.getenv("BROKER")
MQTT_TOPIC_COMMAND = "house/livingroom/led/set" # topic
# Database URL
DATABASE_URL = os.getenv("DB_URL")

# --- 3. Initializations ---

# --- Firebase Admin Setup ---
cred = credentials.Certificate("ServiceAccountKey.json") # JSON key
# Khởi tạo với DATABASE_URL
firebase_admin.initialize_app(cred, {
    'databaseURL': DATABASE_URL
})

# --- Flask App Setup ---
app = Flask(__name__) # Tạo Web Server

# --- MQTT Client Setup ---
def on_connect(client, userdata, flags, return_code):
    if return_code == 0:
        print("Connected sucessfully")
    else:
        print(f"Failed to connect, return code: {return_code}")

mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.connect(MQTT_BROKER, 1883, 60)
mqtt_client.loop_start()

# --- 4. Helper Function (Hàm lấy data - ĐÃ VIẾT LẠI) ---
def get_latest_data_from_db():  #connect to RTDB and read data from "Sensor"
    data_default = {"temperature": "N/A", "humidity": "N/A"}
    try:
        # --- QUAN TRỌNG: Sửa 2 dòng này ---
        # 1. Tên đường dẫn (Path) đến data của bạn, dựa vào ảnh, đó là 'Sensor'
        path_name = "Sensor"
        
        # 2. Tạo một tham chiếu (reference) đến đường dẫn đó
        data_ref = db.reference(path_name)
        
        # 3. Lấy data (chỉ cần .get(), không cần query)
        data = data_ref.get() # data sẽ là: {'humid_data': 23, 'temp_data': 24.2}

        if data:
            # Sửa lại tên key cho khớp với file HTML
            return {
                "temperature": data.get("temp_data", "N/A"),
                "humidity": data.get("humid_data", "N/A")
            }
        else:
            return data_default

    except Exception as e:
        print(f"Loi khi doc Realtime DB: {e}")
        return data_default  #có lỗi hàm trả về giá trị default

# --- 5. Flask Routes (Các trang web) ---
# 
@app.route('/')
def index():
    current_data = get_latest_data_from_db()  #get data from RTDB
    return render_template('index.html', data=current_data) #send data to file html

@app.route('/control')
def control():
    command = request.args.get('cmd') 
    if command == "ON":
        mqtt_client.publish(MQTT_TOPIC_COMMAND, "ON")
    elif command == "OFF":
        mqtt_client.publish(MQTT_TOPIC_COMMAND, "OFF")
    
    return redirect(url_for('index'))

# --- 6. Run the Application ---
if __name__ == '__main__':
    app.run(host = '0.0.0.0', port = 5000, debug = True)

