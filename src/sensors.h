#pragma once

void checkButton(int b);
void updateSensors();
void IRAM_ATTR left_tape_sensor_interrupt();
void IRAM_ATTR right_tape_sensor_interrupt();