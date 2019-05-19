CXX_DEFINES = 
CXX_INCLUDES = -I../3rdparty/pcapplusplus/Dist/header -I../build/gtest/src/googletest/googletest/include -I../include 
CXX_FLAGS = -std=gnu++11

LINK_FLAGS = -Wl,-search_paths_first -Wl,-headerpad_max_install_names  
LIB_PCAP =  -Wl,-rpath,../3rdparty/pcapplusplus/Dist -L../3rdparty/pcapplusplus/Dist  -lPacket++ -lPcap++ -lCommon++ -lpcap
LIB_GTEST = -Wl,-rpath,../build/gtest/src/googletest-build/lib -L../build/gtest/src/googletest-build/lib -lgtest -lgmock 

all:output/csv_export output/test_iex

output/iex_decoder.o:./iex_decoder.cpp 
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o output/iex_decoder.o -c ./iex_decoder.cpp

output/iex_messages.o:./iex_messages.cpp 
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o output/iex_messages.o -c ./iex_messages.cpp

output/csv_export.o:./csv_export.cpp 
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o output/csv_export.o -c ./csv_export.cpp

output/test.o:./test.cpp 
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o output/test.o -c ./test.cpp

output/csv_export:output/iex_decoder.o output/iex_messages.o output/csv_export.o
	g++ -o  output/csv_export  output/iex_decoder.o output/iex_messages.o output/csv_export.o $(LINK_FLAGS) $(LIB_PCAP) 

output/test_iex: output/iex_decoder.o output/iex_messages.o output/test.o
	g++ -o  output/test_iex output/iex_decoder.o output/iex_messages.o output/test.o $(LINK_FLAGS) $(LIB_PCAP) $(LIB_GTEST) -lpthread 

clean:
	rm -rf output/*

