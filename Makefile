# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -I./input_downloader

# Name of the static library
LIB_NAME := libdownloader.a

.PHONY: all
all: $(LIB_NAME)

$(LIB_NAME): input_downloader/Downloader.cpp input_downloader/Downloader.h
	$(CXX) $(CXXFLAGS) -c input_downloader/Downloader.cpp -o Downloader.o
	ar rcs $(LIB_NAME) Downloader.o

.PHONY: clean
clean:
	rm -f Downloader.o $(LIB_NAME)

# create-day: creates a directory Day_$(DAY) and places sol.cpp inside
.PHONY: create-day
create-day:
	@test "$(DAY)" != "" || (echo "Please specify DAY. e.g. make create-day DAY=1"; exit 1)
	mkdir -p Day_$(DAY)
	sed 's/%DAY%/$(DAY)/g' templates/sol_template.cpp > Day_$(DAY)/sol.cpp
	cat templates/Make_template.mk > Day_$(DAY)/Makefile
	echo "Day_$(DAY) directory with sol.cpp created from template."