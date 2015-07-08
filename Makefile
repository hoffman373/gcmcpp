CC = g++
CFLAGS=-std=c++11 -I. -I/usr/local/include
OBJ = jsoncpp.o GcmStanza.o AckMessage.o NackMessage.o ReceiptMessage.o ConnectionDrainingMessage.o UpstreamMessage.o NotificationMessage.o DownstreamMessage.o GlooxGcmManager.o IGcmManager.o IdMessageBase.o DownstreamMessageBase.o
LFLAGS = -L/usr/local/lib -pthread -lgloox -std=c++11


All: stage/libgcmcpp.so.1.0
	cp src/*.hpp stage/include
	cp src/json/*.h stage/include/json
	ln -s stage/libgcmcpp.so.1.0 libgcmcpp.so
	ln -s stage/libgcmcpp.so.1 libgcmcpp.so

buildFolders:
	mkdir -p build
	mkdir -p stage/include/json

build/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

stage/libgcmcpp.so.1.0: buildFolders $(OBJ:%.o=build/%.o)
	$(CC) -shared -Wl,-soname,libgcmcpp.so.1 -o stage/libgcmcpp.so.1.0 $(OBJ:%.o=build/%.o) $(LFLAGS)

clean:	
	rm -fr build/
	rm -fr stage/
	rm -f src/*~
	rm -f *~

cleanCrud:
	rm -f src/*~
	rm -f *~
