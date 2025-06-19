# 🌡️ Humidity & Temperature Anomaly Detection using TinyML

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

## 🧠 Model Summary

```text
Input: 2 features (temperature, humidity)
Dense(8, ReLU) → Dense(4, ReLU) → Dense(1, Sigmoid)
Size after quantization: ~2.5KB
