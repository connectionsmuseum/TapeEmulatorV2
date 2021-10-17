#include <atmel_start.h>
#include "usb_start.h"
#include "sd_mmc.h"
#include "fatFS/ff.h"
#include "block_transfer.h"
#include "tape_states.h"
#include "main.h"

static void tx_complete(struct _dma_resource *resource) {
    // gpio_set_pin_level(PB04GREEN, true);
    return;
}

static const uint8_t debug_buf[] = "some nonsense text";

static sd_mmc_err_t sd_err;
static sd_mmc_err_t sd_err_init;

FATFS FatFs;

bool d12_state = false;
bool d13_state = false;
bool pb04_state = false;
bool pb05_state = false;
int tape_state = STATE_IDLE;
int tape_position = 0;
int read_track = 0;
int write_track = 0;
bool init_flag = false;
bool in_transfer_block = false;
bool data_detect = false;

// Incremented by the tick interrupt handler,
// received in the main loop to trigger updates and
// reset to zero.
volatile int tick_flag = 0;
volatile int debug_tick_flag = 0;


// Ticks are every 2.5ms
// 1600 bit per inch tape, 300ft long.
// ~358 blocks on tracks A and B
// 1668 bytes per block incl preamble
// Read at 30 inches per second. FF/FR is 90 inches per second
// 1.55 inch interblock gap (IBG)
// A block is 8.34 inches, 0.28 seconds.
//
// So:
// Use bytes as position variable.
// Maximum byte position is 720 000.
// 30 inch per second = 6000 bytes per second (slow)
//                    = 6000/400 = 15 bytes per tick
//                    = 48 000 bits per second
// 90 inch per second = 45 bytes per tick
//
// IBG is 1.55 * 1600/8 = 310 bytes
#define FAST_SPEED 45
#define SLOW_SPEED 16
// "Correct" IBG length is 310
#define IBG_BYTES 275
#define BLOCK_BYTES 1668
#define MAX_TRACK_POSITION 740000 // Added some extra padding
#define TAPE_START_POSITION -14400
#define TAPE_BOT_POSITION (-14400 + 3200)
#define TAPE_EOT_POSITION (740000 - 3200)

// #define TAPE_START_POSITION -7200

// Define the "current block" as including the IBG that preceeds the data.
// Calculate the current block with:
// floor( tape_position / (IBG_BYTES + BLOCK_BYTES))
// Start sending data when (tape_position - block_start) > IBG_BYTES, where
// block_start = current_block * (IBG_BYTES + BLOCK_BYTES)


// Set these to zero for normal operation.
#define DEBUG_CONSTANT_FORWARD 0
#define DEBUG_ALWAYS_TRACK_ZERO 0
#define DEBUG_NEVER_CARTWE0    1

static struct timer_task TIMER_1_task1;
static struct timer_task TIMER_1_task2;

static void init_interrupt(void);

static inline bool get_pin_active_low(const uint8_t pin) {
    return !gpio_get_pin_level(pin);
}

static inline void set_pin_active_low(const uint8_t pin, const bool level) {
    gpio_set_pin_level(pin, !level);
}

void update_state() {
    int normal_speed = SLOW_SPEED;
    int fast_speed = FAST_SPEED;
    int previous_tape_state = tape_state;

    int current_block;
    int intrablock_position;
    char usb_printbuf[200];

    // Use this to time the update duration.
    // gpio_set_pin_level(PB05RED, true);
    flash_pin(PB05RED, &pb05_state);

    if(init_flag) {
        // Not sure if we're supposed to display rewinding during init?
        set_pin_active_low(RWDINGA0, true);
        set_pin_active_low(TIMA0, true);

        // TTRDY0 has already been taken inactive during interrupt

        if (cdcdf_acm_is_enabled()) {
            snprintf(usb_printbuf, 99, "*** Initializing *** \n\r");
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
        }

        cancel_transfer();
        tape_position = TAPE_BOT_POSITION + 3000;
        set_pin_active_low(DATDET0, false);

        // Might as well load a block while we're here.
        current_block = find_block(tape_position);
        load_next_block(read_track, current_block);

        delay_ms(1000);

        if (cdcdf_acm_is_enabled()) {
            snprintf(usb_printbuf, 99, "Done Initializing \n\r");
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
        }

        set_pin_active_low(RWDINGA0, false);
        set_pin_active_low(TIMA0, false);
        set_pin_active_low(TTRDY0, true);

        init_flag = false;
        ext_irq_register(TTINIT0, NULL);
        ext_irq_register(TTINIT0, init_interrupt);

        // D11 for IRQ timing
        // gpio_set_pin_level(D11, false);

        // update_state() timing
        // gpio_set_pin_level(D12, false);

        return;
    }


    // TODO:
    //   RWDINGA0 -- CTT is performing a tape rewind sequence
    //   LPEW0 -- Status pulse from CTT to indiciate the CTT
    //            has sensed the load point or early warning holes.
    //            (Implemented but totally a guess)

    // Motion control
    if (get_pin_active_low(TTSF0) || DEBUG_CONSTANT_FORWARD) {
        tape_state = STATE_FORWARD;
        if(previous_tape_state == STATE_FORWARD) {
            tape_position += normal_speed;
        }
    } else if (get_pin_active_low(TTFF0)) {
        tape_state = STATE_FF;
        if(previous_tape_state == STATE_FF) {
            tape_position += fast_speed;
        }
    } else if (get_pin_active_low(TTSR0)) {
        tape_state = STATE_REV;
        if(previous_tape_state == STATE_REV) {
            tape_position -= normal_speed;
        }
    } else if (get_pin_active_low(TTFR0)) {
        tape_state = STATE_FR;
        if(previous_tape_state == STATE_FR) {
            tape_position -= fast_speed;
        }
    } else {
        tape_state = STATE_IDLE;
    }

    // "Tape is Moving" status lead
    if(tape_state != STATE_IDLE) {
        set_pin_active_low(TIMA0, true);
    } else {
        set_pin_active_low(TIMA0, false);
    }

    // Beginning of Tape
    if(tape_position <= TAPE_START_POSITION) {
        tape_position = TAPE_START_POSITION;
        set_pin_active_low(TTBOTA0, true);
    } else {
        set_pin_active_low(TTBOTA0, false);

    }

    // End of Tape
    if(tape_position >= MAX_TRACK_POSITION) {
        tape_position = MAX_TRACK_POSITION;
        set_pin_active_low(TTEOTA0, true);
    } else {
        set_pin_active_low(TTEOTA0, false);
    }

    // Early warning holes
    // This is a total guess. We might need to add some extra space
    // on the tape before the first block.
    if((tape_position <= TAPE_BOT_POSITION)
            || (tape_position >= (MAX_TRACK_POSITION - 3200) )) {
        set_pin_active_low(LPEW0, true);
    } else {
        set_pin_active_low(LPEW0, false);
    }

    // Right now we're not sending early warning holes; need to get the logic
    // right here before re-enabling.
    // set_pin_active_low(LPEW0, false);

    // Not thought-out yet
    /*
    if((tape_position > (MAX_TRACK_POSITION - 400))) {
        set_pin_active_low(LPEW0, true);
    } else {
        set_pin_active_low(LPEW0, false);
    }
    */

    // These are both used for multiple purposes in this function,
    // and should use the post-update position.
    if(tape_position >= 0) {
        current_block = find_block(tape_position);
        intrablock_position = tape_position - current_block*(IBG_BYTES + BLOCK_BYTES);
    } else {
        current_block = 0;
        intrablock_position = 0;
    }

    // Data Detect
    //
    // We do data detect here *only* for reverse moves;
    // during actual sending of data, DATDET is handled in the
    // DMA setup/callback for more precise timing.
    if((tape_state == STATE_FF) || (tape_state == STATE_REV)
        || (tape_state == STATE_FR)) {

        if((intrablock_position > IBG_BYTES) &&
                (current_block > 0 || (current_block == 0 && read_track == 0))) {
            // data_detect variable only for console reporting.
            data_detect = true;
            set_pin_active_low(DATDET0, true);
        } else {
            data_detect = false;
            set_pin_active_low(DATDET0, false);
        }

    }

    // Get the track settings, only if we're not sending data.
    if(!dma_running()) {
        read_track = (get_pin_active_low(RTA10)*2 +
                      get_pin_active_low(RTA00)*1);

        write_track = (get_pin_active_low(WTA10)*2 +
                       get_pin_active_low(WTA00)*1);
    }

    if((get_pin_active_low(RTA10)*2 + get_pin_active_low(RTA00)*1) != read_track) {
        set_pin_active_low(PA02, true);
    } else {
        set_pin_active_low(PA02, false);
    }

#if DEBUG_ALWAYS_TRACK_ZERO == 1
     read_track = 0;
#endif

     /*
    if(read_track == 0) {
        set_pin_active_low(CARTWE0, false);
    } else {
        set_pin_active_low(CARTWE0, true);
    }
    */

    /*
     * Start transfers
     */
    if(tape_state == STATE_FORWARD) {
        // Check if DMA is active, else start
        if(!dma_running()) {
            if(intrablock_position > IBG_BYTES) {

                int block_end_pos = (current_block + 1) * (IBG_BYTES + BLOCK_BYTES);
                send_block(read_track, current_block, block_end_pos);

            }
        }
    } else {
        // Disable transfers if we're not in normal-forward.
        cancel_transfer();
    }


    /*
     * Load the next block; potentially expensive so might
     * need to find a way to do this carefully. Returns quickly
     * if next block is already loaded into buffer.
     * Don't load data if we're moving fast or in reverse.
     */
    if((tape_state == STATE_FORWARD) ||
       (tape_state == STATE_IDLE)) {
        load_next_block(read_track, current_block);
    }

    // Use this to time the update duration.
    // gpio_set_pin_level(D12, false);

}

void flash_pin(const uint8_t pin, bool *state_variable) {
    if(*state_variable) {
	gpio_set_pin_level(pin, true);
        *state_variable = false;
    } else {
	gpio_set_pin_level(pin, false);
        *state_variable = true;
    }
}

void tick(const struct timer_task *const timer_task) {

    // Only to debug ticking.
    // flash_pin(D12, &d12_state);

    tick_flag += 1;

}

void debug_tick(const struct timer_task *const timer_task) {

    debug_tick_flag += 1;

}

// For handling INIT0 signal, which is too fast for polling.
static void init_interrupt() {
    init_flag = true;
    set_pin_active_low(TTRDY0, false);

    // Can set D11 for timing IRQ response.
    // gpio_set_pin_level(D11, true);
}

int find_block(int tape_position) {
    return floor( tape_position / (IBG_BYTES + BLOCK_BYTES));
}


int main(void)
{
    char usb_printbuf[300];
    FRESULT result;

    //
    //
    // debugging_main();
    //
    //
    //

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    // TIMER_LED_init();


    // interval is in terms of 100 microseconds, see CONF_TC0_TIMER_TICK
    TIMER_1_task1.interval = 25;
    TIMER_1_task1.cb       = tick;
    TIMER_1_task1.mode     = TIMER_TASK_REPEAT;
    timer_add_task(&TIMER_1, &TIMER_1_task1);

    // TIMER_1_task2.interval = 10000;
    TIMER_1_task2.interval = 2000;
    TIMER_1_task2.cb       = debug_tick;
    TIMER_1_task2.mode     = TIMER_TASK_REPEAT;
    timer_add_task(&TIMER_1, &TIMER_1_task2);

    timer_start(&TIMER_1);


    result = f_mount(&FatFs, "", 0);

    init_block_buffer(&SPI_0);

    ext_irq_register(TTINIT0, init_interrupt);

    // Transport Ready
    set_pin_active_low(TTRDY0, true);
    // Cartridge is write enabled (from CTT to CTTC)
    set_pin_active_low(CARTWE0, true);
    // Tape OFF reel status (from CTT)
    set_pin_active_low(TOR0, false);
    // Not rewinding
    set_pin_active_low(RWDINGA0, false);

    set_pin_active_low(DATDET0, false);

    while(1) {

        if(tick_flag > 0) {
            tick_flag = 0;
            update_state();
        }

        if(debug_tick_flag > 0) {
            debug_tick_flag = 0;
            // flash_pin(D13, &d13_state);
            // flash_pin(D35, &d35_state);
            flash_pin(PB04GREEN, &pb04_state);
            // Print some status to USB.
            if (cdcdf_acm_is_enabled()) {
                int block = find_block(tape_position);

                snprintf(usb_printbuf, 200, "State: %i, Track %i, DMA: %i, Position: %i, Block: %i\n\r",
                         tape_state, read_track, (int) dma_running(), tape_position, block);
                cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));

                /*
                snprintf(usb_printbuf, 99, "MANEN0 %i, WRENAB0 %i, WTA10 %i, WTA00 %i, RTA10 %i, RTA00 %i \n\r",
                        get_pin_active_low(MANEN0), get_pin_active_low(WRENAB0), get_pin_active_low(WTA10), get_pin_active_low(WTA00),
                        get_pin_active_low(RTA10), get_pin_active_low(RTA00));
                cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));

                snprintf(usb_printbuf, 99, "TTMSPT0 %i, TTREWC0 %i, TTSEL0 %i, TTFR0 %i, TTSR0 %i, TTFF0 %i, TTSF0 %i, WRDATA %i \n\r",
                        get_pin_active_low(TTMSPT0), get_pin_active_low(TTREWC0), get_pin_active_low(TTSEL0), get_pin_active_low(TTFR0),
                        get_pin_active_low(TTSR0), get_pin_active_low(TTFF0), get_pin_active_low(TTSF0), get_pin_active_low(WRDATA));
                cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
                */


            }
        }
    }

}


struct pin_name_pair {
    char name[20];
    uint8_t pin;
};

struct pin_name_pair input_pins[] = {

    {"WTA10", WTA10},
    {"WTA00", WTA00},
    {"RTA10", RTA10},
    {"RTA00", RTA00},
    {"WRENAB0", WRENAB0},
    {"888 WRDATA", WRDATA},
    {"777 TTMSPT0", TTMSPT0},
    {"yyyTTFF0", TTFF0},
    {"TTSF0", TTSF0},
    {"TTFR0", TTFR0},
    {"TTSR0", TTSR0},
    {"TTSEL0", TTSEL0},
    {"TTINIT0", TTINIT0},
    {"TTREWC0", TTREWC0},
    {"MANEN0", MANEN0},

};

//
// Debugging only
//
int debugging_main(void)
{
    char usb_printbuf[200];
    int buf_pos;
    uint32_t sd_cap;
    FRESULT fatfs_result;
    FILINFO file_info;
    DIR dir_obj;

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    int start_pin = 0;
    int end_pin = 6;

    while (1) {

        if (cdcdf_acm_is_enabled()) {
            usb_printbuf[0] = 0;
            for(int i = start_pin; i < end_pin; i++) {
                if(gpio_get_pin_level(input_pins[i].pin)) {
                    snprintf(usb_printbuf + strlen(usb_printbuf),
                            200 - strlen(usb_printbuf), "%s: HIGH ", input_pins[i].name);
                } else {
                    snprintf(usb_printbuf + strlen(usb_printbuf),
                            200 - strlen(usb_printbuf),  "%s: low  ", input_pins[i].name);
                }

                /*
                if(strlen(usb_printbuf) > 100) {
                    snprintf(usb_printbuf + strlen(usb_printbuf) - 2,
                            200 - strlen(usb_printbuf),  "xxxxx\n\r");
                    cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
                    usb_printbuf[0] = 0;
                }
                */
            }
            if(strlen(usb_printbuf) > 0) {
                snprintf(usb_printbuf + strlen(usb_printbuf) - 2,
                        200 - strlen(usb_printbuf),  "\n\r");
                cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
            }
        }


        delay_ms(30);
    }
}
