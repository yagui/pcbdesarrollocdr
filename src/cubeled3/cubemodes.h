#include <util/delay.h>
#include "cubeled3.h"
#include <stdbool.h>

#define TOTAL_MODES  6


extern volatile bool modeEnded;

void cubeModes(uint8_t mode);
