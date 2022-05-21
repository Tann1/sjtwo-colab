#include "volume_treble.h"
#include "mp3.h"
#include "mp3_buttons.h"
#include "mp3_decoder.h"
#include "stdio.h"

static double volume_value = 0.0;
static int bass_value = 0;
static int treble_value = 0;
static int previous_rotary_index = 0;
static int current_volume = 50;
static int current_bass = 15;
static int current_treble = 0;
extern bool currently_playing;
extern bool menu_button;
volatile int volume_menu;

void volume_control(void) {
  if (previous_rotary_index == get_rotary_position()) {
    return;
  }
  volume_value = get_volume_value();

  uint8_t volume_v = 254 * (1 - ((volume_value * 0.50) + 0.5));

  write_register(0x0B, volume_v, volume_v);
  char volume_string[20];
  if (menu_button) {
    sprintf(volume_string, "1. Volume: %d", current_volume);
    lcd__send_row("                       ", 0);
    lcd__send_row(volume_string, 0);
  }

  if (currently_playing) {
    sprintf(volume_string, "Volume: %d", current_volume);
    lcd__send_row("                       ", 2);
    lcd__send_row(volume_string, 2);
  }
}

double get_volume_value(void) {
  int get_current_index = get_rotary_position();

  if (current_volume == 100) {
    if (get_current_index > previous_rotary_index) {
      previous_rotary_index = get_current_index;
      return 1;

    } else {
      current_volume--;
    }
  }

  else if (current_volume == 0) {
    if (get_current_index > previous_rotary_index) {
      current_volume++;

    } else {
      return 0;
    }
  }

  else {
    if (get_current_index > previous_rotary_index) {
      previous_rotary_index = get_current_index;
      current_volume++;
    } else {
      current_volume--;
    }
  }

  previous_rotary_index = get_current_index;

  return (current_volume / 100.0);
}

void bass_control(void) {
  if (previous_rotary_index == get_rotary_position()) {
    return;
  }
  bass_value = get_bass_value();
  uint8_t temp_bass = 0;
  temp_bass |= (bass_value << 4);
  temp_bass |= 15 << 0;

  write_register(0x02, 0, temp_bass);
  char bass_string[20];

  sprintf(bass_string, "2. Bass: %d", bass_value);
  lcd__send_row("                       ", 1);
  lcd__send_row(bass_string, 1);
}

double get_bass_value(void) {
  int get_current_index = get_rotary_position();

  if (current_bass == 15) {
    if (get_current_index > previous_rotary_index) {
      previous_rotary_index = get_current_index;
      return current_bass;

    } else {
      current_bass--;
    }
  }

  else if (current_bass == 0) {
    if (get_current_index > previous_rotary_index) {
      current_bass++;

    } else {
      return 0;
    }
  }

  else {
    if (get_current_index > previous_rotary_index) {
      previous_rotary_index = get_current_index;
      current_bass++;
    } else {
      current_bass--;
    }
  }

  previous_rotary_index = get_current_index;

  return current_bass;
}

void treble_control(void) {
  if (previous_rotary_index == get_rotary_position()) {
    return;
  }
  treble_value = get_treble_value();
  uint8_t temp_treble = 0;
  temp_treble |= (treble_value << 4);
  temp_treble |= 3 << 0;

  write_register(0x02, temp_treble, 0);
  char treble_string[20];

  uint8_t print_treble = treble_value + 8;

  sprintf(treble_string, "3. Treble: %d", print_treble);
  lcd__send_row("                       ", 2);
  lcd__send_row(treble_string, 2);
}

double get_treble_value(void) {
  int get_current_index = get_rotary_position();

  if (current_treble == 7) {
    if (get_current_index > previous_rotary_index) {
      previous_rotary_index = get_current_index;
      return current_treble;

    } else {
      current_treble--;
    }
  }

  else if (current_treble == -8) {
    if (get_current_index > previous_rotary_index) {
      current_treble++;

    } else {
      return -8;
    }
  }

  else {
    if (get_current_index > previous_rotary_index) {
      previous_rotary_index = get_current_index;
      current_treble++;
    } else {
      current_treble--;
    }
  }

  previous_rotary_index = get_current_index;

  return current_treble;
}