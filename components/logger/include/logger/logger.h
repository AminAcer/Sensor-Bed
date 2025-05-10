#pragma once

#include "display/display.h"

/// @brief LOG at INFO level and print to LCD
///
/// @param tag: TAG for the ESP log macro
/// @param msg: Item to log
/// @param ...: Variadic args if needed for logging
#define D_LOGI(tag, msg, ...)                                   \
    do {                                                        \
        char msg_buf[256];                                      \
        snprintf(msg_buf, sizeof(msg_buf), msg, ##__VA_ARGS__); \
        ESP_LOGI(tag, msg, ##__VA_ARGS__);                      \
        display::display_text(msg_buf);                         \
    } while (0)

/// @brief LOG at INFO level and print to LCD
///
/// @param tag: TAG for the ESP log macro
/// @param msg: Item to log
/// @param ...: Variadic args if needed for logging
#define D_LOGW(tag, msg, ...)                                   \
    do {                                                        \
        char msg_buf[256];                                      \
        snprintf(msg_buf, sizeof(msg_buf), msg, ##__VA_ARGS__); \
        ESP_LOGW(tag, msg, ##__VA_ARGS__);                      \
        display::display_text(msg_buf);                         \
    } while (0)

/// @brief LOG at INFO level and print to LCD
///
/// @param tag: TAG for the ESP log macro
/// @param msg: Item to log
/// @param ...: Variadic args if needed for logging
#define D_LOGE(tag, msg, ...)                                   \
    do {                                                        \
        char msg_buf[256];                                      \
        snprintf(msg_buf, sizeof(msg_buf), msg, ##__VA_ARGS__); \
        ESP_LOGE(tag, msg, ##__VA_ARGS__);                      \
        display::display_text(msg_buf);                         \
    } while (0)

/// @brief LOG at INFO level and print to LCD
///
/// @param tag: TAG for the ESP log macro
/// @param msg: Item to log
/// @param ...: Variadic args if needed for logging
#define D_LOGD(tag, msg, ...)                                   \
    do {                                                        \
        char msg_buf[256];                                      \
        snprintf(msg_buf, sizeof(msg_buf), msg, ##__VA_ARGS__); \
        ESP_LOGD(tag, msg, ##__VA_ARGS__);                      \
        display::display_text(msg_buf);                         \
    } while (0)

/// @brief LOG a coordinate and print to LCD
///
/// @param tag: TAG for the ESP log macro
/// @param msg: Item to log
/// @param ...: Variadic args if needed for logging
#define D_LOG_CORD(entityId, cord)                                                              \
    do {                                                                                        \
        char msg_buf[256];                                                                      \
        snprintf(msg_buf, sizeof(msg_buf), "%s cords:\n|- x: %f\n|- y: %f\n|- z: %f", entityId, \
                 cord.getX(), cord.getY(), cord.getZ());                                        \
        ESP_LOGI("Cord", msg_buf);                                                              \
        display::display_text(msg_buf);                                                         \
    } while (0)
