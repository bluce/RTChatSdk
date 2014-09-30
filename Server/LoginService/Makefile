Srcs = $(wildcard *.cpp)
Objs = $(Srcs:%.cpp=%.o)
Deps = $(Srcs:%.cpp=%.d)

.PHONY: clean

OutTarget = LoginService

OutObject: $(OutTarget)

-include $(Deps)

$(Deps): %.d: %.cpp
	@$(CompilerTool) $< -MM $(DebugOption) $(CompilerOption) > $@.$$$$;\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

$(Objs): %.o: %.cpp
	$(CompilerTool) $< $(DebugOption) $(CompilerOption) -o $@

$(OutTarget): $(Objs) $(CubeLib)
	$(LinkTool) $(Objs) $(DebugOption) $(CubeLib) $(LinkOption) -o $@

clean:
	@rm -f $(OutTarget) $(Objs) *.d core*~ 
