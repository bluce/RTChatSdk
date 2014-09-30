.PHONY: all clean

OutTarget = command.a

OutObject: $(OutTarget)

public.pb.cc: public.proto
	protoc --cpp_out=. public.proto

game.pb.cc: game.proto
	protoc --cpp_out=. game.proto

internal.pb.cc: internal.proto
	protoc --cpp_out=. internal.proto

public.o: public.pb.cc public.pb.h
	c++ -c public.pb.cc -o public.o `pkg-config --cflags --libs protobuf`

game.o: game.pb.cc game.pb.h
	c++ -c game.pb.cc -o game.o `pkg-config --cflags --libs protobuf`

internal.o: internal.pb.cc internal.pb.h
	c++ -c internal.pb.cc -o internal.o `pkg-config --cflags --libs protobuf`

$(OutTarget): public.o game.o internal.o
	@rm -rf $(OutTarget)
	@ar cru $(OutTarget) public.o game.o internal.o
	@ranlib $(OutTarget)

clean:
	@rm -f $(OutTarget) *.o *.pb.cc *.pb.h