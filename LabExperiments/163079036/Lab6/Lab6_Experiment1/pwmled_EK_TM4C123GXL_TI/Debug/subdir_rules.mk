################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
EK_TM4C123GXL.obj: ../EK_TM4C123GXL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/Roboteq/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/user/workspace_v7.5/pwmled_EK_TM4C123GXL_TI" --include_path="C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b" --include_path="C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="C:/Roboteq/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="EK_TM4C123GXL.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pwmled.obj: ../pwmled.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/Roboteq/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/user/workspace_v7.5/pwmled_EK_TM4C123GXL_TI" --include_path="C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b" --include_path="C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix" --include_path="C:/Roboteq/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="pwmled.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

build-1056609153:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1056609153-inproc

build-1056609153-inproc: ../pwmled.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/Roboteq/ti/xdctools_3_32_00_06_core/xs" --xdcpath="C:/Roboteq/ti/tirtos_tivac_2_16_00_08/packages;C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/tidrivers_tivac_2_16_00_08/packages;C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages;C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/ndk_2_25_00_09/packages;C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/uia_2_00_05_50/packages;C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/ns_1_11_00_10/packages;C:/Roboteq/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.tiva:TM4C123GH6PM -r release -c "C:/Roboteq/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"C:/Users/user/workspace_v7.5/pwmled_EK_TM4C123GXL_TI\" --include_path=\"C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/TivaWare_C_Series-2.1.1.71b\" --include_path=\"C:/Roboteq/ti/tirtos_tivac_2_16_00_08/products/bios_6_45_01_29/packages/ti/sysbios/posix\" --include_path=\"C:/Roboteq/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include\" --define=ccs=\"ccs\" --define=PART_TM4C123GH6PM --define=ccs --define=TIVAWARE -g --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-1056609153
configPkg/compiler.opt: build-1056609153
configPkg/: build-1056609153


