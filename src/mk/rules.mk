$(OBJECTS): | $(OBJ_DIR)
$(OBJ_DIR):
	@[ ! -d $(OBJ_DIR) ] && mkdir $(OBJ_DIR) 

# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
%.d: %.c
	@set -e; rm -f $@; \
	$(TOOLCHAIN)gcc -MM -MT '$*.o' $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

%.bin: %.elf
	@$(TOOLCHAIN)objcopy -O binary $< $@
	+@printf "copying '$(notdir $<)' -> '$(notdir $@)' ("
	@stat --printf="%s bytes)\n" $@

$(OBJ_DIR)/%.elf: app/%.o $(OBJECTS)
	$(TOOLCHAIN)gcc $(CFLAGS) $^ $(LFLAGS) -Wl,-Map=$(OBJ_DIR)/$*.map -o $@

%.dump: %.elf
	$(TOOLCHAIN)objdump -D $< > $@

%-dump: $(OBJ_DIR)/%.dump
	vim $<

%-debug: $(OBJ_DIR)/%.bin

JLINK_FILE = ./temp.jlink
JLINK_ARGS = -device STM32F303RE -if SWD -speed 4000 -autoconnect 1 -CommanderScript $(JLINK_FILE)
%-flash: $(OBJ_DIR)/%.bin
	@echo "loadbin $<, 0x08000000" > $(JLINK_FILE)
	@echo "r"                     >> $(JLINK_FILE)
	@echo "exit"                  >> $(JLINK_FILE)
	JLinkExe $(JLINK_ARGS)
	@rm $(JLINK_FILE)

# phony targets

clean:
	@find . -name '*.o' -delete
	@find . -name '*.d' -delete
	@find . -name '*.d.*' -delete
	@rm -rf $(OBJ_DIR)

main: $(OBJ_DIR)/main.bin
