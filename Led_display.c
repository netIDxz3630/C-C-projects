#include <stdio.h>
#include <wiringPi.h>
#include <lcd.h>
#include <dht.h>

#define DHT_PIN 4 // GPIO pin number for DHT11 data
#define LCD_RS  25 // GPIO pin number for LCD RS
#define LCD_EN  24 // GPIO pin number for LCD EN
#define LCD_D4  23 // GPIO pin number for LCD D4
#define LCD_D5  22 // GPIO pin number for LCD D5
#define LCD_D6  21 // GPIO pin number for LCD D6
#define LCD_D7  14 // GPIO pin number for LCD D7

#define DHT_TYPE DHT11 // Type of DHT sensor (DHT11 or DHT22)
#define MAX_RETRY 5    // Maximum number of retries for sensor reading

int main() {
    if (wiringPiSetupGpio() == -1) {
        printf("Failed to initialize wiringPi.\n");
        return 1;
    }

    int lcdHandle = lcdInit(2, 16, 4, LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
    if (lcdHandle < 0) {
        printf("Failed to initialize LCD.\n");
        return 1;
    }

    int dhtHandle = dhtSetup(DHT_PIN, DHT_TYPE);
    if (dhtHandle == -1) {
        printf("Failed to initialize DHT sensor.\n");
        return 1;
    }

    printf("Temperature and Humidity Monitoring\n");

    while (1) {
        int retryCount = 0;
        float temperature, humidity;

        while ((retryCount < MAX_RETRY) && dhtRead(dhtHandle, &temperature, &humidity) != 0) {
            retryCount++;
            delay(2000); // Delay 2 seconds before retrying
        }

        if (retryCount == MAX_RETRY) {
            printf("Failed to read sensor data.\n");
            break;
        }

        lcdHome(lcdHandle);
        lcdClear(lcdHandle);
        lcdPosition(lcdHandle, 0, 0);
        lcdPuts(lcdHandle, "Temp: ");
        lcdPrintf(lcdHandle, "%.1fC", temperature);
        lcdPosition(lcdHandle, 0, 1);
        lcdPuts(lcdHandle, "Humidity: ");
        lcdPrintf(lcdHandle, "%.1f%%", humidity);

        delay(5000); // Delay 5 seconds before the next reading
    }

    lcdClear(lcdHandle);
    lcdHome(lcdHandle);
    lcdPuts(lcdHandle, "Program stopped.");

    return 0;
}
