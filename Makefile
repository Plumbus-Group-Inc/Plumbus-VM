BUILD_DIR ?= build

.PHONY: all
all: build run

.PHONY: build
build:
	cmake -B $(PWD)/$(BUILD_DIR) -S $(PWD)
	cmake --build $(PWD)/$(BUILD_DIR)

.PHONY: run
run: build
	$(PWD)/$(BUILD_DIR)/plumbus-vm

.PHONY: clean
.SILENT: clean
clean:
	-rm -rf $(BUILD_DIR)
