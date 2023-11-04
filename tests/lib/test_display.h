#ifndef __TEST_DIPLAY_H__
#define __TEST_DIPLAY_H__
#include "display.h"
#include "definitions.h"
#include <stdint.h>
#include <stdlib.h>

display_t test_display();

void free_test_display(display_t* display);


void test_set_key();

void test_get_key();

#endif
