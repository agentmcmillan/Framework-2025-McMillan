#pragma once
#include <Arduino.h>

#ifndef CHARM_W
#define CHARM_W 9
#endif
#ifndef CHARM_H
#define CHARM_H 7
#endif

extern const uint16_t PROGMEM charms565[][CHARM_W * CHARM_H];
extern const uint8_t  CHARM_COUNT;
