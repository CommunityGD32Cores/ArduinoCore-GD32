.PHONY: astyle

default:
	# The default target does nothing
	@exit 0

astyle:
	astyle --options=.astylerc \
		--suffix=none \
		--exclude=cores/arduino/api \
		--recursive "cores/*.c,*.cpp,*.h" 
	astyle --options=.astylerc \
		--suffix=none \
		--recursive "libraries/*.c,*.cpp,*.h" 


