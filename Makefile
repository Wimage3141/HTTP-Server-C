CC = gcc
CFLAGS = -Wall
BUILD_DIR = build
SRC_DIR = src

.PHONY: server client clean

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

server: $(BUILD_DIR)/server_exe | $(BUILD_DIR)
	$(BUILD_DIR)/server_exe

client: $(BUILD_DIR)/client.o | $(BUILD_DIR)
	@echo "Client built, execute $(BUILD_DIR)/client <args, if any>"
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/client

$(BUILD_DIR)/server_exe: $(BUILD_DIR)/server.o | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# adding $(BUILD_DIR) as a prereq if the build dir does not exist
# it is set as an order-only prereq so that the make file does not look at it's time stamp
# if the make file were to look at $(BUILD_DIR) time stamp every time, then it would always think
# that the $(BUILD_DIR) prereq is newer than the target and always compile to make the target
# to avoid always compiling to get server.o even if no changes have been made, we use $(BUILD_DIR) as a
# order-only prereq
$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(BUILD_DIR)/*


