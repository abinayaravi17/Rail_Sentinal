# 🧠 Railway Sentinel Algorithm

## Objective

To detect animal movement on railway tracks using piezoelectric sensors and immediately notify railway authorities through IoT platforms.

---

## Algorithm

### Step 1

Initialize all hardware components.

- ESP32
- Piezoelectric Sensors
- LCD Display
- LEDs
- Buzzer
- Wi-Fi Connection
- Blynk Platform
- Telegram Bot

---

### Step 2

Continuously read vibration values from the piezoelectric sensor.

---

### Step 3

Compare the sensor value with the predefined threshold.

---

### Step 4

If the vibration value is below the threshold:

- Green LED remains ON
- Display "Track Safe"
- Continue monitoring

---

### Step 5

If the vibration value exceeds the threshold:

- Identify possible animal movement
- Turn ON Red LED
- Activate Buzzer
- Display warning message on LCD
- Send notification to Blynk
- Send alert through Telegram Bot

---

### Step 6

Wait for the predefined alert duration.

---

### Step 7

Reset the warning system.

- Turn OFF Red LED
- Stop Buzzer
- Turn ON Green LED
- Resume monitoring

---

## Flow Summary

```
Start

↓

Initialize ESP32 & Sensors

↓

Read Piezo Sensor

↓

Compare Threshold

↓

Safe?

↓           ↓

YES          NO

↓            ↓

Continue     Trigger Alert

Monitoring

↓

LCD

↓

LED

↓

Buzzer

↓

Telegram

↓

Blynk

↓

Reset

↓

Repeat
```

---

## End of Algorithm