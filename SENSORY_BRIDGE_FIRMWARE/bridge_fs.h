/*----------------------------------------
  Sensory Bridge FILESYSTEM ACCESS
  ----------------------------------------*/
#include <M5Unified.h>

extern void reboot(); // system.h

void update_config_filename(uint32_t input) {
  snprintf(config_filename, 24, "/CONFIG_%05lu.BIN", input);
}

// Restore all defaults defined in globals.h by removing saved data and rebooting
void factory_reset() {
  M5.Log(ESP_LOG_INFO   , "Deleting %s: ", config_filename);

  if (LittleFS.remove(config_filename)) {
    printf("file deleted\n");
  } else {
    printf("delete failed\n");
  }

  printf("Deleting noise_cal.bin: ");
  if (LittleFS.remove("/noise_cal.bin")) {
    printf("file deleted\n");
  } else {
    printf("delete failed\n");
  }

  reboot();
}

// Restore only configuration defaults
void restore_defaults() {
  M5.Log(ESP_LOG_INFO   , "Deleting %s: ", config_filename);

  if (LittleFS.remove(config_filename)) {
    M5.Log(ESP_LOG_INFO   , "file deleted");
  } else {
    M5.Log(ESP_LOG_ERROR   , "delete failed");
  }

  reboot();
}

// Save configuration to LittleFS
void save_config() {
  if (debug_mode) {
    printf("LITTLEFS: ");
  }
  File file = LittleFS.open(config_filename, FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      printf("Failed to open ");
      printf(config_filename);
      printf(" for writing!");
    }
    return;
  } else {
    file.seek(0);
    uint8_t config_buffer[512];
    memcpy(config_buffer, &CONFIG, sizeof(CONFIG));

    for (uint16_t i = 0; i < 512; i++) {
      file.write(config_buffer[i]);
    }

    if (debug_mode) {
      printf("WROTE ");
      printf(config_filename);
      printf(" SUCCESSFULLY");
    }
  }
  file.close();
}

// Save configuration to LittleFS 10 seconds from now
void save_config_delayed() {
  if(debug_mode == true){
    printf("CONFIG SAVE QUEUED");
  }
  next_save_time = millis()+5000;
  settings_updated = true;
}

// Load configuration from LittleFS
void load_config() {
  if (debug_mode) {
    M5.Log(ESP_LOG_INFO   , "LITTLEFS: ");
  }

  bool queue_factory_reset = false;
  File file = LittleFS.open(config_filename, FILE_READ);
  if (!file) {
    if (debug_mode) {
      M5.Log(ESP_LOG_ERROR   ,"Failed to open %s for reading!", config_filename);
    }
    return;
  } else {
    file.seek(0);
    uint8_t config_buffer[512];
    for (uint16_t i = 0; i < sizeof(CONFIG); i++) {
      config_buffer[i] = file.read();
    }

    memcpy(&CONFIG, config_buffer, sizeof(CONFIG));

    if (debug_mode) {
      M5.Log(ESP_LOG_ERROR   , "READ CONFIG SUCCESSFULLY");
    }
  }
  file.close();

  if (queue_factory_reset == true) {
    factory_reset();
  }
}

// Save noise calibration to LittleFS
void save_ambient_noise_calibration() {
  if (debug_mode) {
    printf("SAVING AMBIENT_NOISE PROFILE... ");
  }
  File file = LittleFS.open("/noise_cal.bin", FILE_WRITE);
  if (!file) {
    if (debug_mode) {
      printf("Failed to open file for writing!");
    }
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    float in_val = float(noise_samples[i]);

    temp.long_val_float = in_val;

    file.write(temp.bytes[0]);
    file.write(temp.bytes[1]);
    file.write(temp.bytes[2]);
    file.write(temp.bytes[3]);
  }

  file.close();
  if (debug_mode) {
    printf("SAVE COMPLETE\n");
  }
}

// Load noise calibration from LittleFS
void load_ambient_noise_calibration() {
  if (debug_mode) {
    M5.Log(ESP_LOG_INFO   ,"LOADING AMBIENT_NOISE PROFILE... ");
  }
  File file = LittleFS.open("/noise_cal.bin", FILE_READ);
  if (!file) {
    if (debug_mode) {
      M5.Log(ESP_LOG_ERROR  ,"Failed to open file for reading!");
    }
    return;
  }

  bytes_32 temp;

  file.seek(0);
  for (uint16_t i = 0; i < NUM_FREQS; i++) {
    temp.bytes[0] = file.read();
    temp.bytes[1] = file.read();
    temp.bytes[2] = file.read();
    temp.bytes[3] = file.read();

    noise_samples[i] = SQ15x16(temp.long_val_float);
  }

  file.close();
  if (debug_mode) {
    M5.Log(ESP_LOG_INFO    , "LOAD COMPLETE");
  }
}

// Initialize LittleFS
void init_fs() {
  M5.Log(ESP_LOG_INFO    ,  "INIT FILESYSTEM: %s", LittleFS.begin(true) == true ? PASS : FAIL);

  update_config_filename(FIRMWARE_VERSION);

  load_ambient_noise_calibration();
  load_config();
}
