// This file defines global variables in a single place for convenience.
// It might be more advisable to split these out, especially if it gets to be large.
// To avoid duplicate declarations, we let main.c define these and have others consider them external
#ifdef MAIN_C
    #define EXT
#else
    #define EXT extern
#endif

// These variables can get modified by the ISR
EXT volatile uint8_t currently_logging;
EXT volatile uint8_t cmd_is_ready_flag;

EXT volatile uint32_t time_ms;

EXT volatile int32_t currently_running;
