################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), DARWIN)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
samd51a/gcc/gcc \
usb/class/cdc/device \
hpl/ramecc \
hpl/evsys \
sd_mmc \
examples \
hpl/oscctrl \
hpl/sdhc \
hpl/tc \
hpl/osc32kctrl \
usb \
hpl/dmac \
hal/src \
samd51a/gcc \
hal/utils/src \
hpl/rtc \
hpl/usb \
hpl/pm \
hpl/cmcc \
hpl/gclk \
hpl/mclk \
usb/device \
hpl/sercom \
hpl/eic \
hpl/core \
fatFS

# List the object files
OBJS +=  \
hal/src/hal_io.o \
sd_mmc_start.o \
hal/src/hal_spi_m_dma.o \
samd51a/gcc/system_samd51.o \
hpl/eic/hpl_eic.o \
hpl/evsys/hpl_evsys.o \
hpl/core/hpl_core_m4.o \
hal/src/hal_cache.o \
usb/class/cdc/device/cdcdf_acm.o \
hal/utils/src/utils_syscalls.o \
sd_mmc/sd_mmc.o \
usb_start.o \
hal/src/hal_timer.o \
hal/src/hal_mci_sync.o \
hpl/usb/hpl_usb.o \
hal/src/hal_delay.o \
hpl/pm/hpl_pm.o \
hpl/core/hpl_init.o \
hpl/ramecc/hpl_ramecc.o \
hal/utils/src/utils_list.o \
hal/utils/src/utils_assert.o \
hpl/oscctrl/hpl_oscctrl.o \
hpl/sdhc/hpl_sdhc.o \
hpl/mclk/hpl_mclk.o \
hpl/sercom/hpl_sercom.o \
hpl/gclk/hpl_gclk.o \
usb/usb_protocol.o \
hal/src/hal_init.o \
hal/src/hal_usb_device.o \
main.o \
hpl/osc32kctrl/hpl_osc32kctrl.o \
hpl/rtc/hpl_rtc.o \
examples/driver_examples.o \
driver_init.o \
hpl/tc/tc_lite.o \
hal/src/hal_ext_irq.o \
hal/src/hal_atomic.o \
hal/src/hal_gpio.o \
hal/src/hal_evsys.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
hpl/dmac/hpl_dmac.o \
samd51a/gcc/gcc/startup_samd51.o \
atmel_start.o \
usb/device/usbdc.o \
hpl/tc/hpl_tc.o \
hpl/cmcc/hpl_cmcc.o \
fatFS/diskio.o \
fatFS/ff.o \
fatFS/ffsystem.o \
fatFS/ffunicode.o \
block_transfer.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"sd_mmc_start.o" \
"hal/src/hal_spi_m_dma.o" \
"samd51a/gcc/system_samd51.o" \
"hpl/eic/hpl_eic.o" \
"hpl/evsys/hpl_evsys.o" \
"hpl/core/hpl_core_m4.o" \
"hal/src/hal_cache.o" \
"usb/class/cdc/device/cdcdf_acm.o" \
"hal/utils/src/utils_syscalls.o" \
"sd_mmc/sd_mmc.o" \
"usb_start.o" \
"hal/src/hal_timer.o" \
"hal/src/hal_mci_sync.o" \
"hpl/usb/hpl_usb.o" \
"hal/src/hal_delay.o" \
"hpl/pm/hpl_pm.o" \
"hpl/core/hpl_init.o" \
"hpl/ramecc/hpl_ramecc.o" \
"hal/utils/src/utils_list.o" \
"hal/utils/src/utils_assert.o" \
"hpl/oscctrl/hpl_oscctrl.o" \
"hpl/sdhc/hpl_sdhc.o" \
"hpl/mclk/hpl_mclk.o" \
"hpl/sercom/hpl_sercom.o" \
"hpl/gclk/hpl_gclk.o" \
"usb/usb_protocol.o" \
"hal/src/hal_init.o" \
"hal/src/hal_usb_device.o" \
"main.o" \
"hpl/osc32kctrl/hpl_osc32kctrl.o" \
"hpl/rtc/hpl_rtc.o" \
"examples/driver_examples.o" \
"driver_init.o" \
"hpl/tc/tc_lite.o" \
"hal/src/hal_ext_irq.o" \
"hal/src/hal_atomic.o" \
"hal/src/hal_gpio.o" \
"hal/src/hal_evsys.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"hpl/dmac/hpl_dmac.o" \
"samd51a/gcc/gcc/startup_samd51.o" \
"atmel_start.o" \
"usb/device/usbdc.o" \
"hpl/tc/hpl_tc.o" \
"hpl/cmcc/hpl_cmcc.o" \
"fatFS/diskio.o" \
"fatFS/ff.o" \
"fatFS/ffsystem.o" \
"fatFS/ffunicode.o" \
"block_transfer.o"

# List the directories containing header files
DIR_INCLUDES +=  \
-I"../" \
-I"../config" \
-I"../examples" \
-I"../hal/include" \
-I"../hal/utils/include" \
-I"../hpl/cmcc" \
-I"../hpl/core" \
-I"../hpl/dmac" \
-I"../hpl/eic" \
-I"../hpl/evsys" \
-I"../hpl/gclk" \
-I"../hpl/mclk" \
-I"../hpl/osc32kctrl" \
-I"../hpl/oscctrl" \
-I"../hpl/pm" \
-I"../hpl/port" \
-I"../hpl/ramecc" \
-I"../hpl/rtc" \
-I"../hpl/sdhc" \
-I"../hpl/sercom" \
-I"../hpl/tc" \
-I"../hpl/usb" \
-I"../hri" \
-I"../" \
-I"../config" \
-I"../usb" \
-I"../usb/class/cdc" \
-I"../usb/class/cdc/device" \
-I"../usb/device" \
-I"../" \
-I"../config" \
-I"../sd_mmc" \
-I"../" \
-I"../CMSIS/Core/Include" \
-I"../samd51a/include" \
-I"../fatFS" \

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"hpl/ramecc/hpl_ramecc.d" \
"hpl/core/hpl_core_m4.d" \
"hpl/eic/hpl_eic.d" \
"hal/utils/src/utils_syscalls.d" \
"usb/class/cdc/device/cdcdf_acm.d" \
"sd_mmc_start.d" \
"samd51a/gcc/system_samd51.d" \
"hal/src/hal_spi_m_dma.d" \
"hpl/usb/hpl_usb.d" \
"sd_mmc/sd_mmc.d" \
"hal/src/hal_mci_sync.d" \
"hal/src/hal_timer.d" \
"hal/utils/src/utils_list.d" \
"hpl/sdhc/hpl_sdhc.d" \
"hpl/dmac/hpl_dmac.d" \
"hal/utils/src/utils_assert.d" \
"hpl/cmcc/hpl_cmcc.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/pm/hpl_pm.d" \
"usb/usb_protocol.d" \
"hpl/gclk/hpl_gclk.d" \
"hal/src/hal_usb_device.d" \
"usb_start.d" \
"hal/src/hal_init.d" \
"hpl/mclk/hpl_mclk.d" \
"driver_init.d" \
"hpl/tc/tc_lite.d" \
"hpl/osc32kctrl/hpl_osc32kctrl.d" \
"main.d" \
"examples/driver_examples.d" \
"hpl/rtc/hpl_rtc.d" \
"hal/src/hal_cache.d" \
"hal/src/hal_sleep.d" \
"hal/src/hal_ext_irq.d" \
"hal/src/hal_evsys.d" \
"hal/src/hal_gpio.d" \
"hpl/evsys/hpl_evsys.d" \
"atmel_start.d" \
"hal/src/hal_atomic.d" \
"hpl/tc/hpl_tc.d" \
"usb/device/usbdc.d" \
"hpl/oscctrl/hpl_oscctrl.d" \
"samd51a/gcc/gcc/startup_samd51.d" \
"hpl/sercom/hpl_sercom.d" \
"fatFS/diskio.d" \
"fatFS/ff.d" \
"fatFS/ffsystem.d" \
"fatFS/ffunicode.d" \
"block_transfer.d"

OUTPUT_FILE_NAME :=AtmelStart
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS) -Wl,--start-group -lm -Wl,--end-group -mthumb \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m4 \
 \
-T"../samd51a/gcc/gcc/samd51j20a_flash.ld" \
-L"../samd51a/gcc/gcc"
	@echo Finished building target: $@

	"arm-none-eabi-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"arm-none-eabi-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"arm-none-eabi-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"arm-none-eabi-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"arm-none-eabi-size" "$(OUTPUT_FILE_NAME).elf"

	

# Compiler targets




%.o: %.c
	@echo Building file: $<
	@echo ARM/GNU C Compiler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -O0 -mthumb -DDEBUG -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMD51J20A__ -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARM/GNU Assembler
	$(QUOTE)arm-none-eabi-as$(QUOTE) -x c -O0 -mthumb -DDEBUG -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMD51J20A__ -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -O0 -mthumb -DDEBUG -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAMD51J20A__ -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec
