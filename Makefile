
default:
	@echo "Please specify a target"

umps:
	$(MAKE) -f uMPS_make.mk
	
umps2:
	$(MAKE) -f uMPS_make.mk

uarm:
	$(MAKE) -f uARM_make.mk

clean:
	$(MAKE) -f uARM_make.mk clean
	$(MAKE) -f uMPS_make.mk clean

.PHONY: umps umps2 uarm clean default