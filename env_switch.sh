#!/bin/bash
test -z $2 && echo "Not enough params" && exit 1
[ "$1" != 'get' ] && [ "$1" != 'put' ] && echo "Options: get / put" && exit 1
if [ "$1" = 'get' ]; then
		/bin/cp -f data/home.conf home.conf_$2
		/bin/cp -f src/thingProperties.h thingProperties.h_$2
		/bin/cp -f src/thingProperties.cpp thingProperties.cpp_$2
		echo "Files copied:"
		ls -l *_$2
fi
if [ "$1" = 'put' ]; then
	! [ -f home.conf_$2 ] && echo "File home.conf_$2 doesn't exist" && exit 1
	! [ -f thingProperties.h_$2 ] && echo "File thingProperties.h_$2 doesn't exist" && exit 1
	! [ -f thingProperties.cpp_$2 ] && echo "File thingProperties.cpp_$2 doesn't exist" && exit 1
	/bin/cp -f home.conf_$2 data/home.conf
	/bin/cp -f thingProperties.h_$2 src/thingProperties.h
	/bin/cp -f thingProperties.cpp_$2 src/thingProperties.cpp
	echo "Files copied:"
	ls -l *_$2
fi
