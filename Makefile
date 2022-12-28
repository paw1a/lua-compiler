CC = gcc

CFLAGS = -std=c99 -O2
DEPFLAGS = -MT $@ -MMD -MP -MF $(OUT)/$*.d
LDFLAGS = -lm

TARGET = clua
OUT = build
SRC = src

$(shell mkdir -p $(OUT))

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	CC = gcc-12
endif

SOURCES = $(wildcard $(SRC)/*.c)
HEADERS = $(wildcard $(SRC)/*.h)
OBJ = $(SOURCES:$(SRC)/%.c=$(OUT)/%.o)

-include $(SOURCES:$(SRC)/%.c=$(OUT)/%.d)

$(OUT)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(OUT) $(TARGET)

all: $(TARGET)

.PHONY: clean $(TARGET)

.DEFAULT_GOAL := $(TARGET)
