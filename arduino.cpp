#include <TensorFlowLite.h>
#include "model_data.cc"
#include "DHT.h"

// DHT Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// TFLite includes
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// TensorArena
constexpr int kTensorArenaSize = 2048;
uint8_t tensor_arena[kTensorArenaSize];

// Globals
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input;
TfLiteTensor* output;

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Set up error reporter
  static tflite::MicroErrorReporter micro_error_reporter;
  tflite::ErrorReporter* error_reporter = &micro_error_reporter;

  // Load model
  const tflite::Model* model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    return;
  }

  // Resolver
  static tflite::AllOpsResolver resolver;

  // Interpreter
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate
  interpreter->AllocateTensors();

  // Get input/output
  input = interpreter->input(0);
  output = interpreter->output(0);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Sensor error.");
    delay(1000);
    return;
  }

  // Feed input (e.g., normalized to 0-1 range if trained that way)
  input->data.f[0] = temp / 100.0;
  input->data.f[1] = hum / 100.0;

  // Run inference
  interpreter->Invoke();

  // Get result
  float result = output->data.f[0];
  Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" | Hum: "); Serial.print(hum);
  Serial.print(" | Anomaly Score: "); Serial.println(result);

  if (result > 0.5) {
    Serial.println("⚠️ Anomaly Detected!");
  }

  delay(2000);  // Adjust as needed
}
