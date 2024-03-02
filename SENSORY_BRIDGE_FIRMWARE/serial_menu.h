/*----------------------------------------
  Sensory Bridge UART COMMAND LINE
  ----------------------------------------*/

// These functions watch the Serial port for incoming commands,
// and perform actions based on whatis recieved.

extern void check_current_function();  // system.h
extern void reboot();                  // system.h

void tx_begin(bool error = false) {
  if (error == false) {
    printf("sbr{{\n");
  } else {
    printf("sberr[[\n");
  }
}

void tx_end(bool error = false) {
  if (error == false) {
    printf("}}\n");
  } else {
    printf("]]\n");
  }
}

void ack() {
  printf("SBOK\n");
}

void bad_command(char* command_type, char* command_data) {
  tx_begin(true);
  printf("Bad command: ");
  printf(command_type);
  if (command_data[0] != 0) {
    printf("=");
    printf(command_data);
  }

  printf("\n");
  tx_end(true);
}

void stop_streams() {
  stream_audio = false;
  stream_fps = false;
  stream_max_mags = false;
  stream_max_mags_followers = false;
  stream_magnitudes = false;
  stream_spectrogram = false;
  stream_chromagram = false;
}

void init_serial(uint32_t baud_rate) {
  USBSerial.begin(baud_rate);  // Default 500,000 baud
  bool timeout = false;
  bool serial_started = true;
  uint32_t t_start = millis();
  uint32_t t_timeout = t_start + 250;

  while (!Serial && timeout == false) {
    if (millis() >= t_timeout) {
      timeout = true;  // Must not be connected to PC
      serial_started = false;
    } else {
      yield();
    }
  }

  // Print welcome message
  printf("---------------------------\n");
  printf("SENSORY BRIDGE | VER: %d\n", FIRMWARE_VERSION);
  printf("---------------------------\n\n");
  printf("INIT_SERIAL: %s\n", serial_started == true ? "PASS" : "FAIL");
}

// This is for development purposes, and allows the user to dump
// the current values of many variables to the monitor at once
void dump_info() {
  printf("FIRMWARE_VERSION: %d\n", FIRMWARE_VERSION);

  printf("CHIP ID: ");
  print_chip_id();

  printf("noise_button.pressed: %s\n", noise_button.pressed ? "true" : "false");
  printf("noise_button.last_down: %lu\n", noise_button.last_down);
  printf("noise_button.last_up: %lu\n", noise_button.last_up);
  printf("noise_button.pin: %d\n", noise_button.pin);

  printf("mode_button.pressed: %s\n", mode_button.pressed ? "true" : "false");
  printf("mode_button.last_down: %lu\n", mode_button.last_down);
  printf("mode_button.last_up: %lu\n", mode_button.last_up);
  printf("mode_button.pin: %d\n", mode_button.pin);


  printf("CONFIG.PHOTONS: %.6f\n", CONFIG.PHOTONS);
  printf("CONFIG.CHROMA: %.6f\n", CONFIG.CHROMA);
  printf("CONFIG.MOOD: %.6f\n", CONFIG.MOOD);
  printf("CONFIG.LIGHTSHOW_MODE: %u\n", CONFIG.LIGHTSHOW_MODE);
  printf("CONFIG.MIRROR_ENABLED: %s\n", CONFIG.MIRROR_ENABLED ? "true" : "false");
  printf("CONFIG.CHROMAGRAM_RANGE: %u\n", CONFIG.CHROMAGRAM_RANGE);
  printf("CONFIG.SAMPLE_RATE: %lu\n", CONFIG.SAMPLE_RATE);
  printf("CONFIG.NOTE_OFFSET: %u\n", CONFIG.NOTE_OFFSET);
  printf("CONFIG.SQUARE_ITER: %u\n", CONFIG.SQUARE_ITER);
  printf("CONFIG.LED_TYPE: %u\n", CONFIG.LED_TYPE);
  printf("CONFIG.LED_COUNT: %u\n", CONFIG.LED_COUNT);
  printf("CONFIG.LED_COLOR_ORDER: %u\n", CONFIG.LED_COLOR_ORDER);
  printf("CONFIG.SAMPLES_PER_CHUNK: %u\n", CONFIG.SAMPLES_PER_CHUNK);
  printf("CONFIG.SENSITIVITY: %.6f\n", CONFIG.SENSITIVITY);
  printf("CONFIG.BOOT_ANIMATION: %s\n", CONFIG.BOOT_ANIMATION ? "true" : "false");
  printf("CONFIG.SWEET_SPOT_MIN_LEVEL: %lu\n", CONFIG.SWEET_SPOT_MIN_LEVEL);
  printf("CONFIG.SWEET_SPOT_MAX_LEVEL: %lu\n", CONFIG.SWEET_SPOT_MAX_LEVEL);
  printf("CONFIG.DC_OFFSET: %ld\n", CONFIG.DC_OFFSET);
  printf("CONFIG.STANDBY_DIMMING: %s\n", CONFIG.STANDBY_DIMMING ? "true" : "false");
  printf("CONFIG.REVERSE_ORDER: %s\n", CONFIG.REVERSE_ORDER ? "true" : "false");
  printf("CONFIG.IS_MAIN_UNIT: %s\n", CONFIG.IS_MAIN_UNIT ? "true" : "false");
  printf("CONFIG.MAX_CURRENT_MA: %lu\n", CONFIG.MAX_CURRENT_MA);
  printf("CONFIG.TEMPORAL_DITHERING: %s\n", CONFIG.TEMPORAL_DITHERING ? "true" : "false");
  printf("CONFIG.AUTO_COLOR_SHIFT: %s\n", CONFIG.AUTO_COLOR_SHIFT ? "true" : "false");
  printf("CONFIG.INCANDESCENT_FILTER: %.6f\n", CONFIG.INCANDESCENT_FILTER);
  printf("CONFIG.INCANDESCENT_MODE: %s\n", CONFIG.INCANDESCENT_MODE ? "true" : "false");
  printf("CONFIG.BULB_OPACITY: %.6f\n", CONFIG.BULB_OPACITY);
  printf("CONFIG.SATURATION: %.6f\n", CONFIG.SATURATION);
  printf("CONFIG.PRISM_COUNT: %u\n", CONFIG.PRISM_COUNT);
  printf("CONFIG.BASE_COAT: %s\n", CONFIG.BASE_COAT ? "true" : "false");

  printf("MASTER_BRIGHTNESS: %f\n", MASTER_BRIGHTNESS);
  printf("stream_audio: %s\n", stream_audio ? "true" : "false");
  printf("stream_fps: %s\n", stream_fps ? "true" : "false");
  printf("stream_max_mags: %s\n", stream_max_mags ? "true" : "false");
  printf("stream_max_mags_followers: %s\n", stream_max_mags_followers ? "true" : "false");
  printf("stream_magnitudes: %s\n", stream_magnitudes ? "true" : "false");
  printf("stream_spectrogram: %s\n", stream_spectrogram ? "true" : "false");
  printf("stream_chromagram: %s\n", stream_chromagram ? "true" : "false");
  printf("debug_mode: %s\n", debug_mode ? "true" : "false");
  printf("noise_complete: %s\n", noise_complete ? "true" : "false");
  printf("noise_iterations: %d\n", noise_iterations);
  printf("main_override: %s\n", main_override ? "true" : "false");
  printf("last_rx_time: %d\n", last_rx_time);
  printf("next_save_time: %d\n", next_save_time);
  printf("settings_updated: %s\n", settings_updated ? "true" : "false");
  printf("I2S_PORT: %d\n", I2S_PORT);
  printf("SAMPLE_HISTORY_LENGTH: %d\n", SAMPLE_HISTORY_LENGTH);
  printf("silence: %s\n", silence ? "true" : "false");
  printf("mode_destination: %d\n", mode_destination);
  printf("SYSTEM_FPS: %f\n", SYSTEM_FPS);
  printf("LED_FPS: %f\n", LED_FPS);
}

// This parses a completed command to decide how to handle it
void parse_command(char* command_buf) {

  // COMMANDS WITHOUT METADATA ###############################

  // Get firmware version -----------------------------------
  if (strcmp(command_buf, "v") == 0 || strcmp(command_buf, "V") == 0 || strcmp(command_buf, "version") == 0) {

    tx_begin();
    printf("VERSION: %d\n", FIRMWARE_VERSION);
    tx_end();

  }

  // Print help ---------------------------------------------
  else if (strcmp(command_buf, "h") == 0 || strcmp(command_buf, "H") == 0 || strcmp(command_buf, "help") == 0) {

    tx_begin();
    printf("SENSORY BRIDGE - Serial Menu ------------------------------------------------------------------------------------\n\n");
    printf("                                           v | Print firmware version number\n");
    printf("                                       reset | Reboot Sensory Bridge\n");
    printf("                               factory_reset | Delete configuration, including noise cal, reboot\n");
    printf("                            restore_defaults | Delete configuration, reboot\n");
    printf("                               get_main_unit | Print if this unit is set to MAIN for SensorySync\n");
    printf("                                        dump | Print tons of useful variables in realtime\n");
    printf("                                        stop | Stops the output of any enabled streams\n");
    printf("                                         fps | Return the system FPS\n");
    printf("                                     led_fps | Return the LED FPS\n");
    printf("                                     chip_id | Return the chip id (MAC) of the CPU\n");
    printf("                                    get_mode | Get lightshow mode's ID (index)\n");
    printf("                               get_num_modes | Return the number of modes available\n");
    printf("                             start_noise_cal | Remotely begin a noise calibration\n");
    printf("                             clear_noise_cal | Remotely clear the stored noise calibration\n");
    printf("                                    identify | Flashes the LEDs twice in yellow\n");
    printf("                              set_mode=[int] | Set the mode number\n");
    printf("         mirror_enabled=[true/false/default] | Remotely toggle lightshow mirroring\n");
    printf("          reverse_order=[true/false/default] | Toggle whether image is flipped upside down before final rendering\n");
    printf("                         get_mode_name=[int] | Get a mode's name by ID (index)\n");
    printf("                               stream=[type] | Stream live data to a Serial Plotter.\n");
    printf("                                               Options are: audio, fps, magnitudes, spectrogram, chromagram\n");
    printf("             led_type=['neopixel'/'dotstar'] | Sets which LED protocol to use, 3 wire or 4 wire\n");
    printf("                led_count=[int or 'default'] | Sets how many LEDs your display will use (native resolution is 128)\n");
    printf("       led_color_order=[GRB/RGB/BGR/default] | Sets LED color ordering, default GRB\n");
    printf("      led_interpolation=[true/false/default] | Toggles linear LED interpolation when running in a non-native resolution (slower)\n");
    printf("                          debug=[true/false] | Enables debug mode, where functions are timed\n");
    printf("               sample_rate=[hz or 'default'] | Sets the microphone sample rate\n");
    printf("             note_offset=[0-32 or 'default'] | Sets the lowest note, as a positive offset from A1 (55.0Hz)\n");
    printf("              square_iter=[int or 'default'] | Sets the number of times the LED output is squared (contrast)\n");
    printf("        samples_per_chunk=[int or 'default'] | Sets the number of samples collected every frame\n");
    printf("            sensitivity=[float or 'default'] | Sets the scaling of audio data (>1.0 is more sensitive, <1.0 is less sensitive)\n");
    printf("         boot_animation=[true/false/default] | Enable or disable the boot animation\n");
    printf("                  set_main_unit=[true/false] | Sets if this unit is MAIN or not for SensorySync\n");
    printf("           sweet_spot_min=[int or 'default'] | Sets the minimum amplitude to be inside the 'Sweet Spot'\n");
    printf("           sweet_spot_max=[int or 'default'] | Sets the maximum amplitude to be inside the 'Sweet Spot'\n");
    printf("        chromagram_range=[1-64 or 'default'] | Range between 1 and 64, how many notes at the bottom of the\n");
    printf("                                               spectrogram should be considered in chromagram sums\n");
    printf("        standby_dimming=[true/false/default] | Toggle dimming during detected silence\n");
    printf("                      bass_mode=[true/false] | Toggle bass-mode, which alters note_offset and chromagram_range for bass-y tunes\n");
    printf("           max_current_ma=[int or 'default'] | Sets the maximum current FastLED will attempt to limit the LED consumption to\n");
    printf("     temporal_dithering=[true/false/default] | Toggle per-LED temporal dithering that simulates higher bit-depths\n");
    printf("       auto_color_shift=[true/false/default] | Toggle automated color shifting based on positive spectral changes\n");
    printf("    incandescent_filter=[float or 'default'] | Set the intensity of the incandescent LUT (reduces harsh blues)\n");
    printf("      incandescent_mode=[true/false/default] | Force all output into monochrome and tint with 2700K incandescent color\n");
    printf("              base_coat=[true/false/default] | Enable a dim gray backdrop to the LEDs (approves appearance in most modes)\n");
    printf("           bulb_opacity=[float or 'default'] | Set opacity of a filter that portrays the output as 32 \"bulbs\" with separation and hot spots\n");
    printf("             saturation=[float or 'default'] | Sets the saturation of internal hues\n");
    printf("              prism_count=[int or 'default'] | Sets the number of times the \"prism\" effect is applied\n");
    printf("                        preset=[preset_name] | Sets multiple configuration options at once to match a preset theme\n");
    tx_end();
  }

  // So that software can automatically identify this device -
  else if (strcmp(command_buf, "SB?") == 0) {

    printf("SB!\n");

  }

  // Reset the micro ----------------------------------------
  else if (strcmp(command_buf, "reset") == 0) {

    ack();
    reboot();

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "factory_reset") == 0) {

    ack();
    factory_reset();

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "restore_defaults") == 0) {

    ack();
    restore_defaults();

  }

  // Return chip ID -----------------------------------------
  else if (strcmp(command_buf, "chip_id") == 0) {

    tx_begin();
    print_chip_id();
    tx_end();

  }

  // Identify unit via 2 yellow flashes ---------------------
  else if (strcmp(command_buf, "identify") == 0) {

    ack();
    CRGB16 col = {1.00, 0.25, 0.00};
    blocking_flash(col);

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "start_noise_cal") == 0) {

    ack();
    noise_transition_queued = true;

  }

  // Clear configs and reset micro --------------------------
  else if (strcmp(command_buf, "clear_noise_cal") == 0) {

    ack();
    clear_noise_cal();

  }

  // Returns the number of modes available ------------------
  else if (strcmp(command_buf, "get_num_modes") == 0) {

    tx_begin();
    printf("NUM_MODES: %d\n", NUM_MODES);
    tx_end();

  }

  // Returns the mode ID ------------------------------------
  else if (strcmp(command_buf, "get_mode") == 0) {

    tx_begin();
    printf("MODE: %d\n", CONFIG.LIGHTSHOW_MODE);
    tx_end();

  }

  // Returns whether or not this is a "MAIN" unit -----------
  else if (strcmp(command_buf, "get_main_unit") == 0) {

    tx_begin();
    printf("CONFIG.IS_MAIN_UNIT: %s\n", CONFIG.IS_MAIN_UNIT ? "true" : "false");
    tx_end();

  }

  // Returns the reason why the ESP32 last rebooted ---------
  else if (strcmp(command_buf, "reset_reason") == 0) {
    tx_begin();
    switch (esp_reset_reason()) {
      case ESP_RST_UNKNOWN:
        printf("UNKNOWN\n");
        break;
      case ESP_RST_POWERON:
        printf("POWERON\n");
        break;
      case ESP_RST_EXT:
        printf("EXTERNAL\n");
        break;
      case ESP_RST_SW:
        printf("SOFTWARE\n");
        break;
      case ESP_RST_PANIC:
        printf("PANIC\n");
        break;
      case ESP_RST_INT_WDT:
        printf("INTERNAL WATCHDOG\n");
        break;
      case ESP_RST_TASK_WDT:
        printf("TASK WATCHDOG\n");
        break;
      case ESP_RST_WDT:
        printf("WATCHDOG\n");
        break;
      case ESP_RST_DEEPSLEEP:
        printf("DEEPSLEEP\n");
        break;
      case ESP_RST_BROWNOUT:
        printf("BROWNOUT\n");
        break;
      case ESP_RST_SDIO:
        printf("SDIO\n");
        break;
    }
    tx_end();
  }

  // Dump tons of variables to the monitor ------------------
  else if (strcmp(command_buf, "dump") == 0) {

    tx_begin();
    dump_info();
    tx_end();

  }

  // If a streaming or plotting a variable, stop ------------
  else if (strcmp(command_buf, "stop") == 0) {

    stop_streams();
    ack();

  }

  // Print the average FPS ----------------------------------
  else if (strcmp(command_buf, "fps") == 0) {

    tx_begin();
    printf("SYSTEM_FPS: %f\n", SYSTEM_FPS);
    tx_end();

  }

  // Print the average FPS ----------------------------------
  else if (strcmp(command_buf, "led_fps") == 0) {

    tx_begin();
    printf("LED_FPS: %f\n", LED_FPS);
    tx_end();

  }

  // Print the knob values ----------------------------------
  else if (strcmp(command_buf, "get_knobs") == 0) {
    printf("{\"PHOTONS\":%f,", CONFIG.PHOTONS);
    printf("\"CHROMA\":%f,", CONFIG.CHROMA);
    printf("\"MOOD\":%f}\n", CONFIG.MOOD);
  }

  // Print the button values --------------------------------
  else if (strcmp(command_buf, "get_buttons") == 0) {
    printf("{\"NOISE\":%d,", digitalRead(noise_button.pin));
    printf("\"MODE\":%d}\n", digitalRead(mode_button.pin));
  }

  // COMMANDS WITH METADATA ##################################

  else {  // Commands with metadata are parsed here

    // PARSER #############################
    // Parse command type
    char command_type[32] = { 0 };
    uint8_t reading_index = 0;
    for (uint8_t i = 0; i < 32; i++) {
      reading_index++;
      if (command_buf[i] != '=') {
        command_type[i] = command_buf[i];
      } else {
        break;
      }
    }

    // Then parse command data
    char command_data[94] = { 0 };
    for (uint8_t i = 0; i < 94; i++) {
      if (command_buf[reading_index + i] != 0) {
        command_data[i] = command_buf[reading_index + i];
      } else {
        break;
      }
    }
    // PARSER #############################

    // Now react accordingly:

    // Set if this Sensory Bridge is a MAIN Unit --------------
    if (strcmp(command_type, "set_main_unit") == 0) {
      bool good = false;
      if (strcmp(command_data, "true") == 0) {
        good = true;
        CONFIG.IS_MAIN_UNIT = true;
      } else if (strcmp(command_data, "false") == 0) {
        good = true;
        CONFIG.IS_MAIN_UNIT = false;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();

        tx_begin();
        printf("CONFIG.IS_MAIN_UNIT: %s\n", CONFIG.IS_MAIN_UNIT ? "true" : "false");
        tx_end();

        reboot();
      }
    }

    // Toggle Debug Mode --------------------------------------
    else if (strcmp(command_type, "debug") == 0) {
      bool good = false;
      if (strcmp(command_data, "true") == 0) {
        good = true;
        debug_mode = true;
        cpu_usage.attach_ms(5, check_current_function);
      } else if (strcmp(command_data, "false") == 0) {
        good = true;
        debug_mode = false;
        cpu_usage.detach();
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        tx_begin();
        printf("debug_mode: %s\n", debug_mode ? "true" : "false");
        tx_end();
      }
    }

    // Set Sample Rate ----------------------------------------
    else if (strcmp(command_type, "sample_rate") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        good = true;
        CONFIG.SAMPLE_RATE = CONFIG_DEFAULTS.SAMPLE_RATE;
      } else {
        good = true;
        CONFIG.SAMPLE_RATE = constrain(atol(command_data), 6400, 44100);
      }

      if (good) {
        save_config();
        tx_begin();
        printf("CONFIG.SAMPLE_RATE: %d\n", CONFIG.SAMPLE_RATE);
        tx_end();
        reboot();
      }
    }

    // Set Mode Number ----------------------------------------
    else if (strcmp(command_type, "set_mode") == 0) {
      mode_transition_queued = true;
      mode_destination = constrain(atol(command_data), 0, NUM_MODES - 1);

      save_config_delayed();
      tx_begin();
      printf("CONFIG.LIGHTSHOW_MODE: %d\n", mode_destination);
      tx_end();
    }

    // Get Mode Name By ID ------------------------------------
    else if (strcmp(command_type, "get_mode_name") == 0) {
      uint16_t mode_id = atol(command_data);

      if (mode_id < NUM_MODES) {
        char buf[32] = { 0 };
        for (uint8_t i = 0; i < 32; i++) {
          char c = mode_names[32 * mode_id + i];
          if (c != 0) {
            buf[i] = c;
          } else {
            break;
          }
        }

        tx_begin();
        printf("MODE_NAME: %s\n", buf);
        tx_end();
      } else {
        bad_command(command_type, command_data);
      }
    }

    // Set Note Offset ----------------------------------------
    else if (strcmp(command_type, "note_offset") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.NOTE_OFFSET = CONFIG_DEFAULTS.NOTE_OFFSET;
      } else {
        CONFIG.NOTE_OFFSET = constrain(atol(command_data), 0, 32);
      }
      save_config();
      tx_begin();
      printf("CONFIG.NOTE_OFFSET: %d\n", CONFIG.NOTE_OFFSET);
      tx_end();
      reboot();
    }

    // Set Square Iterations ----------------------------------
    else if (strcmp(command_type, "square_iter") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SQUARE_ITER = CONFIG_DEFAULTS.SQUARE_ITER;
      } else {
        CONFIG.SQUARE_ITER = constrain(atol(command_data), 0, 10);
      }
      save_config_delayed();

      tx_begin();
      printf("CONFIG.SQUARE_ITER: %d\n", CONFIG.SQUARE_ITER);
      tx_end();
    }

    // Set LED Type ---------------------------------------
    else if (strcmp(command_type, "led_type") == 0) {
      bool good = false;
      if (strcmp(command_data, "neopixel") == 0) {
        CONFIG.LED_TYPE = LED_NEOPIXEL;
        CONFIG.LED_COLOR_ORDER = GRB;
        good = true;
      } else if (strcmp(command_data, "dotstar") == 0) {
        CONFIG.LED_TYPE = LED_DOTSTAR;
        CONFIG.LED_COLOR_ORDER = BGR;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        printf("CONFIG.LED_TYPE: %d\n", CONFIG.LED_TYPE);
        tx_end();
        reboot();
      }
    }

    // Set LED Count ------------------------------------
    else if (strcmp(command_type, "led_count") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_COUNT = CONFIG_DEFAULTS.LED_COUNT;
      } else {
        CONFIG.LED_COUNT = constrain(atol(command_data), 1, 10000);
      }

      save_config();
      tx_begin();
      printf("CONFIG.LED_COUNT: %d\n", CONFIG.LED_COUNT);
      tx_end();
      reboot();
    }

    // Set LED Interpolation ----------------------------
    else if (strcmp(command_type, "led_interpolation") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_INTERPOLATION = CONFIG_DEFAULTS.LED_INTERPOLATION;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.LED_INTERPOLATION = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.LED_INTERPOLATION = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.LED_INTERPOLATION: %s\n", CONFIG.LED_INTERPOLATION ? "true" : "false");
        tx_end();
      }
    }

    // Set Base Coat ----------------------------
    else if (strcmp(command_type, "base_coat") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.BASE_COAT = CONFIG_DEFAULTS.BASE_COAT;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.BASE_COAT = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.BASE_COAT = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.BASE_COAT: %s\n", CONFIG.BASE_COAT ? "true" : "false");
        tx_end();
      }
    }

    // Set LED Temporal Dithering ----------------------------
    else if (strcmp(command_type, "temporal_dithering") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.TEMPORAL_DITHERING = CONFIG_DEFAULTS.TEMPORAL_DITHERING;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.TEMPORAL_DITHERING = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.TEMPORAL_DITHERING = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.TEMPORAL_DITHERING: %s\n", CONFIG.TEMPORAL_DITHERING ? "true" : "false");
        tx_end();
      }
    }

    // Set LED Color Order ----------------------------
    else if (strcmp(command_type, "led_color_order") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.LED_COLOR_ORDER = CONFIG_DEFAULTS.LED_COLOR_ORDER;
        good = true;
      } else if (strcmp(command_data, "GRB") == 0) {
        CONFIG.LED_COLOR_ORDER = GRB;
        good = true;
      } else if (strcmp(command_data, "RGB") == 0) {
        CONFIG.LED_COLOR_ORDER = RGB;
        good = true;
      } else if (strcmp(command_data, "BGR") == 0) {
        CONFIG.LED_COLOR_ORDER = BGR;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        printf("CONFIG.LED_COLOR_ORDER: %d\n", CONFIG.LED_COLOR_ORDER);
        tx_end();
        reboot();
      }
    }

    // Set Samples Per Chunk ---------------------------
    else if (strcmp(command_type, "samples_per_chunk") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SAMPLES_PER_CHUNK = CONFIG_DEFAULTS.SAMPLES_PER_CHUNK;
      } else {
        CONFIG.SAMPLES_PER_CHUNK = constrain(atol(command_data), 0, SAMPLE_HISTORY_LENGTH);
      }

      save_config();
      tx_begin();
      printf("CONFIG.SAMPLES_PER_CHUNK: %d\n", CONFIG.SAMPLES_PER_CHUNK);
      tx_end();
      reboot();
    }

    // Set Audio Sensitivity ----------------------------
    else if (strcmp(command_type, "sensitivity") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SENSITIVITY = CONFIG_DEFAULTS.SENSITIVITY;
      } else {
        CONFIG.SENSITIVITY = atof(command_data);
      }

      save_config_delayed();
      tx_begin();
      printf("CONFIG.SENSITIVITY: %f\n", CONFIG.SENSITIVITY);
      tx_end();
    }

    // Toggle Boot Animation --------------------------
    else if (strcmp(command_type, "boot_animation") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.BOOT_ANIMATION = CONFIG_DEFAULTS.BOOT_ANIMATION;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.BOOT_ANIMATION = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.BOOT_ANIMATION = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        printf("CONFIG.BOOT_ANIMATION: %s\n", CONFIG.BOOT_ANIMATION ? "true" : "false");
        tx_end();
        reboot();
      }
    }

    // Toggle Lightshow Mirroring ---------------------
    else if (strcmp(command_type, "mirror_enabled") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.MIRROR_ENABLED = CONFIG_DEFAULTS.MIRROR_ENABLED;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.MIRROR_ENABLED = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.MIRROR_ENABLED = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.MIRROR_ENABLED: %s\n", CONFIG.MIRROR_ENABLED ? "true" : "false");
        tx_end();
      }
    }

    // Set Sweet Spot LOW threshold -------------------
    else if (strcmp(command_type, "sweet_spot_min") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SWEET_SPOT_MIN_LEVEL = CONFIG_DEFAULTS.SWEET_SPOT_MIN_LEVEL;
      } else {
        CONFIG.SWEET_SPOT_MIN_LEVEL = constrain(atof(command_data), 0, uint32_t(-1));
      }

      save_config_delayed();
      tx_begin();
      printf("CONFIG.SWEET_SPOT_MIN_LEVEL: %d\n", CONFIG.SWEET_SPOT_MIN_LEVEL);
      tx_end();
    }

    // Set Sweet Spot HIGH threshold ------------------
    else if (strcmp(command_type, "sweet_spot_max") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SWEET_SPOT_MAX_LEVEL = CONFIG_DEFAULTS.SWEET_SPOT_MAX_LEVEL;
      } else {
        CONFIG.SWEET_SPOT_MAX_LEVEL = constrain(atof(command_data), 0, uint32_t(-1));
      }

      save_config_delayed();
      tx_begin();
      printf("CONFIG.SWEET_SPOT_MAX_LEVEL: %d\n", CONFIG.SWEET_SPOT_MAX_LEVEL);
      tx_end();
    }

    // Set Chromagram Range ---------------
    else if (strcmp(command_type, "chromagram_range") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.CHROMAGRAM_RANGE = CONFIG_DEFAULTS.CHROMAGRAM_RANGE;
      } else {
        CONFIG.CHROMAGRAM_RANGE = constrain(atof(command_data), 1, 64);
      }

      save_config_delayed();
      tx_begin();
      printf("CONFIG.CHROMAGRAM_RANGE: %d\n", CONFIG.CHROMAGRAM_RANGE);
      tx_end();
    }

    // Set Standby Dimming behavior -------
    else if (strcmp(command_type, "standby_dimming") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.STANDBY_DIMMING = CONFIG_DEFAULTS.STANDBY_DIMMING;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.STANDBY_DIMMING = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.STANDBY_DIMMING = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.STANDBY_DIMMING: %d\n", CONFIG.STANDBY_DIMMING);
        tx_end();
      }
    }

    // Toggle bass mode -------------------
    else if (strcmp(command_type, "bass_mode") == 0) {
      bool good = false;
      if (strcmp(command_data, "true") == 0) {
        CONFIG.NOTE_OFFSET = 0;
        CONFIG.CHROMAGRAM_RANGE = 24;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.NOTE_OFFSET = CONFIG_DEFAULTS.NOTE_OFFSET;
        CONFIG.CHROMAGRAM_RANGE = CONFIG_DEFAULTS.CHROMAGRAM_RANGE;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config();
        tx_begin();
        printf("BASS MODE ENABLED\n");
        tx_end();
        reboot();
      }
    }

    // Set if image should be reversed ------------------------
    else if (strcmp(command_type, "reverse_order") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.REVERSE_ORDER = CONFIG_DEFAULTS.REVERSE_ORDER;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.REVERSE_ORDER = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.REVERSE_ORDER = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.REVERSE_ORDER: %d\n", CONFIG.REVERSE_ORDER);
        tx_end();
      }
    }

    // Set max LED current ----------------------------
    else if (strcmp(command_type, "max_current_ma") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.MAX_CURRENT_MA = CONFIG_DEFAULTS.MAX_CURRENT_MA;
      } else {
        CONFIG.MAX_CURRENT_MA = constrain(atof(command_data), 0, uint32_t(-1));
      }

      FastLED.setMaxPowerInVoltsAndMilliamps(5.0, CONFIG.MAX_CURRENT_MA);

      save_config_delayed();
      tx_begin();
      printf("CONFIG.MAX_CURRENT_MA: %d\n", CONFIG.MAX_CURRENT_MA);
      tx_end();
    }

    // Stream a given value over Serial -----------------
    else if (strcmp(command_type, "stream") == 0) {
      stop_streams();  // Stop any current streams
      if (strcmp(command_data, "audio") == 0) {
        stream_audio = true;
        ack();
      } else if (strcmp(command_data, "fps") == 0) {
        stream_fps = true;
        ack();
      } else if (strcmp(command_data, "max_mags") == 0) {
        stream_max_mags = true;
        ack();
      } else if (strcmp(command_data, "max_mags_followers") == 0) {
        stream_max_mags_followers = true;
        ack();
      } else if (strcmp(command_data, "magnitudes") == 0) {
        stream_magnitudes = true;
        ack();
      } else if (strcmp(command_data, "spectrogram") == 0) {
        stream_spectrogram = true;
        ack();
      } else if (strcmp(command_data, "chromagram") == 0) {
        stream_chromagram = true;
        ack();
      } else {
        bad_command(command_type, command_data);
      }
    }

    // Toggle Color Shift ---------------------------------
    else if (strcmp(command_type, "auto_color_shift") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.AUTO_COLOR_SHIFT = CONFIG_DEFAULTS.AUTO_COLOR_SHIFT;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.AUTO_COLOR_SHIFT = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.AUTO_COLOR_SHIFT = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.AUTO_COLOR_SHIFT: %s\n", CONFIG.AUTO_COLOR_SHIFT ? "true" : "false");
        tx_end();
      }
    }

    // Set Incandescent LUT intensity ----------------------------
    else if (strcmp(command_type, "incandescent_filter") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.INCANDESCENT_FILTER = CONFIG_DEFAULTS.INCANDESCENT_FILTER;
      } else {
        CONFIG.INCANDESCENT_FILTER = atof(command_data);
        if (CONFIG.INCANDESCENT_FILTER < 0.0) {
          CONFIG.INCANDESCENT_FILTER = 0.0;
        } else if (CONFIG.INCANDESCENT_FILTER > 1.0) {
          CONFIG.INCANDESCENT_FILTER = 1.0;
        }
      }

      save_config_delayed();
      tx_begin();
      printf("CONFIG.INCANDESCENT_FILTER: %f\n", CONFIG.INCANDESCENT_FILTER);
      tx_end();
    }

    // Toggle Incandescent Mode ----------------------------
    else if (strcmp(command_type, "incandescent_mode") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        CONFIG.INCANDESCENT_MODE = CONFIG_DEFAULTS.INCANDESCENT_MODE;
        good = true;
      } else if (strcmp(command_data, "true") == 0) {
        CONFIG.INCANDESCENT_MODE = true;
        good = true;
      } else if (strcmp(command_data, "false") == 0) {
        CONFIG.INCANDESCENT_MODE = false;
        good = true;
      } else {
        bad_command(command_type, command_data);
      }

      if (good) {
        save_config_delayed();
        tx_begin();
        printf("CONFIG.INCANDESCENT_MODE: %s\n", CONFIG.INCANDESCENT_MODE ? "true" : "false");
        tx_end();
      }
    }

    // Set Bulb Cover Opacity ----------------------------
    else if (strcmp(command_type, "bulb_opacity") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.BULB_OPACITY = CONFIG_DEFAULTS.BULB_OPACITY;
      } else {
        CONFIG.BULB_OPACITY = atof(command_data);
        if (CONFIG.BULB_OPACITY < 0.0) {
          CONFIG.BULB_OPACITY = 0.0;
        } else if (CONFIG.BULB_OPACITY > 1.0) {
          CONFIG.BULB_OPACITY = 1.0;
        }
      }

      save_config_delayed();
      tx_begin();
      printf("CONFIG.BULB_OPACITY: %f\n", CONFIG.BULB_OPACITY);
      tx_end();
    }

    // Set Saturation ----------------------------
    else if (strcmp(command_type, "saturation") == 0) {
      if (strcmp(command_data, "default") == 0) {
        CONFIG.SATURATION = CONFIG_DEFAULTS.SATURATION;
      } else {
        CONFIG.SATURATION = atof(command_data);
        if (CONFIG.SATURATION < 0.0) {
          CONFIG.SATURATION = 0.0;
        } else if (CONFIG.SATURATION > 1.0) {
          CONFIG.SATURATION = 1.0;
        }
      }

      save_config_delayed();
      tx_begin();
      printf("CONFIG.SATURATION: %f\n", CONFIG.SATURATION);
      tx_end();
    }

    // Set Prism Count ----------------------------------------
    else if (strcmp(command_type, "prism_count") == 0) {
      bool good = false;
      if (strcmp(command_data, "default") == 0) {
        good = true;
        CONFIG.PRISM_COUNT = CONFIG_DEFAULTS.PRISM_COUNT;
      } else {
        good = true;
        CONFIG.PRISM_COUNT = constrain(atol(command_data), 0, 10);
      }

      if (good) {
        save_config();
        tx_begin();
        printf("CONFIG.PRISM_COUNT: %d\n", CONFIG.PRISM_COUNT);
        tx_end();
      }
    }

    // Set CONFIG preset ----------------------------
    else if (strcmp(command_type, "preset") == 0) {
      bool good = false;

      if      (strcmp(command_data, "default")      == 0) { good = true; }
      else if (strcmp(command_data, "tinted_bulbs") == 0) { good = true; }
      else if (strcmp(command_data, "incandescent") == 0) { good = true; }
      else if (strcmp(command_data, "white")        == 0) { good = true; }
      else if (strcmp(command_data, "classic")      == 0) { good = true; }

      else { // Bad preset name
        bad_command(command_type, command_data);
      }

      if (good) {
        set_preset(command_data); // presets.h

        save_config_delayed();
        tx_begin();
        printf("ENABLED PRESET: %s\n", command_data);
        tx_end();
      }
    }

    // COMMAND NOT RECOGNISED #############################
    else {
      bad_command(command_type, command_data);
    }
    // COMMAND NOT RECOGNISED #############################
  }
}

// Called on every frame, collects incoming characters until
// potential commands are found
void check_serial(uint32_t t_now) {
  serial_iter++;
  if (USBSerial.available() > 0) {
    char c = USBSerial.read();
    if (c != '\n') {  // If normal character, add to buffer
      command_buf[command_buf_index] = c;
      command_buf_index++;

      // Avoid overflow at the cost of breaking a long command
      if (command_buf_index > 127) {
        command_buf_index = 127;
      }

    } else {  // If a newline character is received,
      // the command in the buffer should be parsed
      parse_command(command_buf);                   // Parse
      memset(&command_buf, 0, sizeof(char) * 128);  // Clear
      command_buf_index = 0;                        // Reset
    }
  }
}
