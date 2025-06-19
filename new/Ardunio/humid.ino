#include <Arduino.h>
#include "model_data.h"  // Generated using xxd -i quant_model.tflite > model_data.cc
#include <DHT.h>

// ==== DHT11 Setup ====
#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ==== TensorFlow Lite Micro ====
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/version.h"

constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;

TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// ✅ Actual MinMax values from training dataset
const float TEMP_MIN = 5.13;
const float TEMP_MAX = 44.99;
const float HUM_MIN = 5.2;
const float HUM_MAX = 99.74;

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();

  // Load model
  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("❌ Model version mismatch!");
    while (1);
  }

  // Only needed ops
  static tflite::MicroMutableOpResolver<4> resolver;
  resolver.AddFullyConnected();
  resolver.AddLogistic();
  resolver.AddMul();
  resolver.AddRelu();

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize
  );
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("❌ Failed to allocate tensors!");
    while (1);
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("✅ Model initialized.");
}

void loop() {
  delay(2000);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    return;
  }

  // 🧮 MinMax scaling
  float temp_scaled = (temp - TEMP_MIN) / (TEMP_MAX - TEMP_MIN);
  float hum_scaled  = (hum - HUM_MIN) / (HUM_MAX - HUM_MIN);

  // Clamp values between 0 and 1
  temp_scaled = constrain(temp_scaled, 0.0, 1.0);
  hum_scaled  = constrain(hum_scaled, 0.0, 1.0);

  input->data.f[0] = temp_scaled;
  input->data.f[1] = hum_scaled;

  // Inference
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("❌ Inference failed!");
    return;
  }

  float prediction = output->data.f[0];

  // === Output ===
  Serial.print("🌡 Temp: "); Serial.print(temp); Serial.print(" °C, ");
  Serial.print("💧 Humidity: "); Serial.println(hum);
  Serial.print("🧮 Scaled Temp: "); Serial.println(temp_scaled, 4);
  Serial.print("🧮 Scaled Hum : "); Serial.println(hum_scaled, 4);
  Serial.print("🔎 Prediction  : "); Serial.println(prediction, 4);

  if (prediction > 0.75) {
    Serial.println("🚨 Anomaly Detected!");
  } else {
    Serial.println("✅ Normal Condition.");
  }

  Serial.println("------------------------------");
}
