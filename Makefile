CC=g++
SUBDIR=$(subdir)

.PHONY: clean, all

all:
	cd $(SUBDIR) && $(MAKE) all

clean:
	@echo "clean"
	cd $(SUBDIR) && $(MAKE) clean
	
