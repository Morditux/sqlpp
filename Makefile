CC=gcc
CXX=g++
CFLAGS=-W -O2 -fPIC
CXXFLAGS=-fPIC -W -std=c++11 -O2
EXEC=sqlpp.so
OBJS=Blob.o Cursor.o Database.o PreparedStatement.o SQLiteException.o \
	sqlite3.o
all: sqlitepp.so

clean:
	rm *.o -f
	rm *.*~ -f
	rm *~ -f
	rm *.so -f

Blob.o: Blob.cpp Blob.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

Cursor.o: Cursor.cpp Cursor.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

Database.o: Database.cpp Database.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

PreparedStatement.o: PreparedStatement.cpp PreparedStatement.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

SQLiteException.o: SQLiteException.cpp SQLiteException.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

sqlite3.o: sqlite/sqlite3.c sqlite/sqlite3.h
	$(CC) -o $@ -c $< $(CFLAGS)

sqlitepp.so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

