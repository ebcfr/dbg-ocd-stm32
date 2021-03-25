# STM32F401/STM32F411 project

* [About the sample project](#about)
* [Configuration](#cfg)
* [Building and running the project](#run)


<a id="about"></a>
## About the sample project

## Project tree

```
 +- config/            linker script, openocd script
 +- docs/              some doc
 +- include/
 |   +- cmsis/         ARM CMSIS core
 |   +- board.h        global peripheral pointers
 |   +- config.h       global project IO pin configuration
 |   +- stm32f411xe.h  peripheral register structure definition
 +- lib/               microcontroller peripheral drivers
 +- src/               Application src
 +- startup/           Startup code ()
 +- Makefile
 +- README.md
```

<a id="cfg"></a>
## Project configuration (if needed)

The project is managed with a Makefile. The developper needs only to focus on
the first part of the Makefile.

* Choose the project to compile
	
	```
	##############################################################################################
	# Start of user section
	#
	PROJ = main_test
	#PROJ = main_test_dma
	#PROJ = main_test_malloc
	#PROJ = main_test_svc
	#PROJ = main_test_systick
	```

* Choose your target
	
	```
	LDSCRIPT = config/STM32F411RE_FLASH.lds
	
	UDEFS = -DSTM32F411xE
	```

	The first line defines the linker script used for memory mapping, the second
	defines a symbol for conditionnal compilation in the startup code.
	
* Choose the source files to compile
	
	They are set in the SRC symbol
	
	```
	SRC  = startup/stm32f411_periph.c startup/sys_handlers.c startup/rcc.c \
	       startup/system_stm32f4xx.c startup/alloc.c \
	       lib/io.c \
	       src/${PROJ}.c 
	```

* Choose additionnal libraies by setting `ULIBS` symbol.

All pathes must be relative to the project root directory.


<a id="run"></a>
## Build an run the project

### Connecting to the board

	ocd &
	
then choose the stm32 target in the list.

### Building the project:

	make

will produce the executable `main.elf`, a mapping file `main.elf.map` and object files in the different directories where there is source code.

### Cleaning the project:

	make clean

### Uploading the program to the microcontroller:

	dbg main.elf &

uploads the program to the microcontroller FLASH memory, and 
executes the code up to the entry `main()` function.

`dbg` has a restart feature (see second icon from the left) which enable to
restart the program without re-flashing (interesting while debugging).

`dbg` has an autreload feature that tracks for executable change. If on, it 
reloads automatically the new executable so that you don't need to relaunch
it each time.

	# query autoreload
	set autoreload
	
	# set autoreload
	set autoreload 1
	
	# unset autoreload
	set autoreload 0
