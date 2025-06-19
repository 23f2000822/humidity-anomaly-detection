# 🌡️ Humidity & Temperature Anomaly Detection using TinyML Models & ESP 32

This project implements a real-time anomaly detection system for monitoring temperature and humidity using **TinyML** deployed on **ESP32** with a **DHT11 sensor**.

## 🔍 Overview

- 🧠 Model: Lightweight Neural Network trained on environmental sensor data
- 🛠️ Platform: ESP32
- 🌡️ Sensor: DHT11 (temperature and humidity)
- 📦 Frameworks: TensorFlow Lite for Microcontrollers (TFLM)
- 🚀 Deployment: Model converted to `.tflite`, then compiled into C array for microcontroller

## 🧪 Features

- Detects anomalous environmental conditions (e.g., overheating, moisture drop)
- Inference fully runs on-device (offline operation)
- Minimal memory and power usage
- Real-time serial output

## 🔧 Hardware Required

- ESP32 (DevKit v1)
- DHT11 sensor
- USB cable
- Breadboard + jumper wires

# 🌡️ Humidity & Temperature Anomaly Detection using TinyML (ESP32 + DHT11)

This project implements a **real-time anomaly detection system** for temperature and humidity using **TinyML** deployed on an **ESP32 microcontroller** with a **DHT11 sensor**.

---

## 📁 Project Structure

humidity-anomaly-detection/
├── model_data.cc # Quantized model as C array
├── humid.ino # Arduino sketch for inference
├── DHT.h # Local DHT11 library (optional)
├── README.md # Project documentation
└── data.csv # Collected sensor dataset (optional)


---

## ⚙️ Process Flow

This section outlines the complete end-to-end workflow for designing, training, optimizing, and deploying the model.

---

### 1. 📥 Data Collection

- **Sensor**: DHT11 connected to ESP32 or Arduino
- **Features**:
  - `Temperature (°C)`
  - `Humidity (%)`
- **Labeling**:
  - `0` → Normal
  - `1` → Anomalous (e.g., unusual environment)
- **Tool Used**: Collected using Serial Monitor or `pySerial`
- **File Format**: CSV (`data.csv`)

---

### 2. 🧹 Data Preprocessing

- **Cleaned** the dataset: removed corrupt and noisy readings.
- **Normalized** the data using `MinMaxScaler` to scale between 0 and 1.
- **Balanced** class distribution to avoid bias.

---

### 3. 🧠 Model Definition

- **Framework**: TensorFlow / Keras
- **Architecture**:

```python
model = Sequential([
    Dense(8, activation='relu', input_shape=(2,)),
    Dense(4, activation='relu'),
    Dense(1, activation='sigmoid')
])
Loss: Binary Crossentropy
Optimizer: Adam
Metrics: Accuracy, Precision, Recall
4. 🏋️ Model Training & Testing
Train/Test Split: 80% training, 20% testing
Results:
Training Accuracy: ~97.3%
Validation Accuracy: ~98.6%
Model Exported As: .h5 file
5. 🔁 TFLite Conversion & Quantization
Converted .h5 to .tflite:
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()
Post-training quantization applied:
converter.optimizations = [tf.lite.Optimize.DEFAULT]
Final size: ~2.5 KB
6. 🔃 Convert to C Array (.cc file)
Used xxd to convert .tflite to a C array:

xxd -i model.tflite > model_data.cc
This file is used in the Arduino sketch to load the model into memory.
7. 💡 Arduino Integration
Main file: humid.ino
Libraries:
DHT.h – for DHT11 sensor
tensorflow/lite/micro/... – for TinyML inference
TFLM Components: Only essential ops included via MicroMutableOpResolver
8. 🚀 ESP32 Deployment
Uploaded the sketch using Arduino IDE
Serial Monitor Output:
0 → Normal condition
1 → Anomaly detected
Hardware Used:
ESP32 DevKit
DHT11 sensor
Breadboard + Jumper Wires
USB cable for upload
🔬 Model Summary

Input Shape: 2 (Temperature, Humidity)
Layer 1: Dense(8, ReLU)
Layer 2: Dense(4, ReLU)
Output Layer: Dense(1, Sigmoid)
Final Model Size: ~2.5 KB (after quantization)
🖼️ Demo

Add a screenshot or video link here showing live inference on Serial Monitor.
📊 Accuracy Metrics

Metric	Value
Train Acc	97.3%
Val Acc	98.6%
Model Size	~2.5 KB
Latency	~<20 ms
📦 Dependencies

Python:
tensorflow, numpy, pandas, sklearn
Arduino IDE:
ESP32 board support
DHT sensor library
TFLite Micro setup (tensorflow/lite/micro)
Tools:
xxd for model conversion
