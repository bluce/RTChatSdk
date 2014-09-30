Srcs = $(wildcard *.cpp)
Objs = $(Srcs:%.cpp=%.o)
Deps = $(Srcs:%.cpp=%.d)

OutTarget = cubelib.a

OutObject: $(OutTarget)

-include $(Deps)

$(Deps): %.d: %.cpp
	@$(CompilerTool) $< -MM $(DebugOption) $(CompilerOption) > $@.$$$$;\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

$(Objs): %.o: %.cpp
	$(CompilerTool) $< $(DebugOption) $(CompilerOption) -o $@

$(OutTarget): $(Objs)
	@rm -rf $(OutTarget)
	@ar cru $(OutTarget) $(Objs)
	@ranlib $(OutTarget)

clean:
	@rm -f $(OutTarget) *.o *.d core*~
