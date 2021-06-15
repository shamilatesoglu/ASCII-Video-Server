OBJS	= src/sserver.o src/string_utils.o src/video.o
SOURCE	= src/server.c src/string_utils.c src/video.c
HEADER	= src/string_utils.h src/lang_utils.h src/video.h
INC=src/ ./
INC_PARAMS=$(foreach d, $(INC), -I$d)
OUT	= sserver
CC	= gcc
LFLAGS	 = -lm -lpthread -lrt

all: sserver

.PHONY: sserver
sserver: $(OBJS)
	@echo "[INFO] Linking C executable sserver..."
	@$(CC)  $^ $(LFLAGS) -o $@
	@echo "[OK]   Built C executable sserver"

%.o: %.c $(HEADER)
	@echo "[INFO] Compiling and assembling $<"
	@$(CC) $(INC_PARAMS) -c -o $@ $< $(LFLAGS)

.PHONY: clean_obj_files
clean_obj_files:
	@echo "[OK]   Cleaning object files..."
	@rm -f $(OBJS)
	@echo "[OK]   Object files are cleaned."

.PHONY: clean
clean:
	@echo "[INFO] Cleaning project..."
	@rm -f $(OBJS) $(OUT)
	@echo "[OK]   Project cleaned."
