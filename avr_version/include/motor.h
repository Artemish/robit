// diagnostics
int get_counts(uint8_t motor);
int get_target(uint8_t motor);

int16_t set_target(uint8_t motor, int8_t byte);

int find_direction(void);
int init_motor(void);

void go(void);
void stop(void);
