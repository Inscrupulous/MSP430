################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-msp430_20.2.2.LTS/bin/cl430" -vmsp --code_model=small --data_model=small -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccs1010/ccs/ccs_base/msp430/include" --include_path="C:/Users/tyler/OneDrive/Desktop/CPE-325/lab8/Lab008" --include_path="C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-msp430_20.2.2.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --c89 --c++14 --relaxed_ansi --float_operations_allowed=all --plain_char=unsigned --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --asm_listing --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


