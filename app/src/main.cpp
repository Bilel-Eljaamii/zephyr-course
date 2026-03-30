#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/random/random.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define SLEEP_TIME_MS 1000
#define STRIP_NODE    DT_ALIAS(led_strip)
#define STRIP_NUM_LEDS DT_PROP(STRIP_NODE, chain-length)

static const struct device *strip = DEVICE_DT_GET(STRIP_NODE);
static struct led_rgb pixels[STRIP_NUM_LEDS];

int main(void)
{
    if (!device_is_ready(strip)) {
        LOG_ERR("LED strip device not ready");
        return 0;
    }

    LOG_INF("Found LED strip with %d LEDs", STRIP_NUM_LEDS);

    while (1) {
        /* Generate a random color */
        uint32_t random_val = sys_rand32_get();
        uint8_t r = (uint8_t)(random_val >> 16);
        uint8_t g = (uint8_t)(random_val >> 8);
        uint8_t b = (uint8_t)(random_val);

        /* Apply the same random color to all LEDs in the strip */
        for (int i = 0; i < STRIP_NUM_LEDS; i++) {
            pixels[i].r = r;
            pixels[i].g = g;
            pixels[i].b = b;
        }

        /* Send data to the strip */
        if (led_strip_update_rgb(strip, pixels, STRIP_NUM_LEDS) < 0) {
            LOG_ERR("Failed to update LED strip");
        }

        LOG_INF("Color changed to R:%d G:%d B:%d", r, g, b);
        
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
