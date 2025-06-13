#  Humidity Anomaly Detection using TinyML

This project uses a TinyML model to detect temperature and humidity anomalies in real-time using an ESP32 and a DHT11 sensor.

##  Features
- Real-time temperature & humidity monitoring
- Lightweight ML model (TFLite, quantized)
- Deployed on ESP32 for on-device inference
- Uses DHT11 sensor for data collection

## � Project Structure
- `model.ipynb`: Model training and evaluation
- `quant.tflite`: Quantized TFLite model
- `model_data.cc`: Converted model for deployment
- `arduino.cpp`: Inference logic for ESP32
- `requirements.txt`: Dependencies for model training

##  Hardware
- ESP32 DevKit V1
- DHT11 Sensor
- Breadboard, Jumper Wires

##  Deployment
1. Convert `.tflite` to `.cc` using `xxd`
2. Flash ESP32 with `arduino.cpp` code
3. View inference results via Serial Monitor

## 📊 Model Performance
| Model Type       | Accuracy | Size    |
|------------------|----------|---------|
| Baseline (.h5)   | 0.88     | 32.45 KB |
| TFLite           | 0.88     | 2.27 KB  |
| Quantized TFLite | 0.86     | 2.87 KB  |


