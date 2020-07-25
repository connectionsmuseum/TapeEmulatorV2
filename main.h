

void update_state();

void tick(const struct timer_task *const timer_task);

int find_block(int tape_position);

void flash_pin(const uint8_t pin, bool *state_variable);

