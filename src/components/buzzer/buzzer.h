#pragma once

typedef enum {
    BUZZER_NONE = 0,
    BUZZER_MID,
    BUZZER_LONG
} buzzer_event_type_t;

void buzzer_init(void);
void buzzer_send(buzzer_event_type_t type);  // Use this for triggering buzz pattern
