program_NAME := app
program_C_SRCS := $(wildcard *.c)
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_INCLUDE_DIRS := 
program_LIBRARY_DIRS :=
program_LIBRARIES := 

CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -O3 `pkg-config --cflags gtk+-3.0`
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))
LDFLAGS += `pkg-config --libs gtk+-3.0`

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_C_OBJS)
	$(LINK.C) $(program_C_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_C_OBJS)

distclean: clean
