#ifndef MODEL_DATA_H
#define MODEL_DATA_H

extern const unsigned char copy_tflite[];
extern const unsigned int copy_tflite_len;

// Alias expected by Eloquent API
#define model_data        copy_tflite
#define model_data_len    copy_tflite_len

#endif  // MODEL_DATA_H