
default:
	@echo "Please specify a target"

umps:
	$(MAKE) -f uMPS_make
	
umps2:
	$(MAKE) -f uMPS_make

uarm:
	$(MAKE) -f uARM_make

clean:
	$(MAKE) -f uARM_make clean
	$(MAKE) -f uMPS_make clean

.PHONY: umps umps2 uarm clean default