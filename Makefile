CC=g++
SUBDIR=threads

.PHONY: clean, all

all:
	cd $(SUBDIR) && $(MAKE) all

clean:
	@echo "clean"
	cd $(SUBDIR) && $(MAKE) clean
	
