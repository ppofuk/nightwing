.DEFAULT_GOAL := debug
prefix=/usr
.PHONY: install

include build/nightwing.mk


make: 
	lbtcli -m .lbt
	
debug: structure bin/debug/nightwing
	
	
install: bin/nightwing
	install -m 0755 bin/nightwing $(prefix)/bin
	install -m 0644 share/xsessions/nightwing.desktop $(prefix)/share/xsessions/nightwing.desktop
	
clean: 
	rm -f build/*.o build/release/*.o build/debug/*.o bin/debug/nightwing bin/nightwing
	
release: structure bin/nightwing
	
	
structure: 
	mkdir -p ./build/debug
	mkdir -p ./build/release
	
