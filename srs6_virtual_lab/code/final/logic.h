#pragma once

enum Zone { ZONE_NONE, ZONE_RED, ZONE_YELLOW, ZONE_GREEN };

struct Sample {
  bool valid;
  float cm;
};

inline float durationToCm(unsigned long durationUs) {
  return durationUs * 0.0343f / 2.0f;
}

inline Sample makeSample(unsigned long durationUs) {
  Sample s;
  s.valid = durationUs != 0;
  s.cm = s.valid ? durationToCm(durationUs) : 0.0f;
  return s;
}

inline Zone classify(bool valid, float cm) {
  if (!valid) return ZONE_NONE;
  if (cm < 20.0f) return ZONE_RED;
  if (cm <= 50.0f) return ZONE_YELLOW;
  return ZONE_GREEN;
}

inline const char* zoneName(Zone z) {
  switch (z) {
    case ZONE_RED: return "RED";
    case ZONE_YELLOW: return "YELLOW";
    case ZONE_GREEN: return "GREEN";
    default: return "NONE";
  }
}

inline bool medianOfValid(const Sample* samples, int n, float* out) {
  float v[16];
  int k = 0;
  for (int i = 0; i < n && k < 16; i++) {
    if (samples[i].valid) v[k++] = samples[i].cm;
  }
  if (k < n / 2 + 1) return false;
  for (int i = 1; i < k; i++) {
    float x = v[i];
    int j = i - 1;
    while (j >= 0 && v[j] > x) {
      v[j + 1] = v[j];
      j--;
    }
    v[j + 1] = x;
  }
  *out = (k % 2) ? v[k / 2] : (v[k / 2 - 1] + v[k / 2]) / 2.0f;
  return true;
}
