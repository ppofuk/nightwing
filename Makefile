.DEFAULT_GOAL := bin/debug/nightwing

include build/nightwing.mk


make: 
	lbtcli -m .lbt
	
clean: 
	rm -f build/*.o build/release/*.o build/debug/*.o bin/debug/nightwing bin/nightwing
	
release: bin/nightwing
	
	
