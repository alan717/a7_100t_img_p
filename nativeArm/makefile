CC=arm-linux-androideabi-g++.exe
 

# a.out: main.cpp 
	#$(cc) -o $@ $^ -llog -fPIE -pie
	
	
 
	
	
	
	# 指令编译器和选项  
  
CFLAGS=-Wall  
# 宏定义  
DEFS = -fPIE -pie -llog 
CFLAGS += $(DEFS)  
# 目标文件  
      
TARGET=test  
# 源文件  
SRCS = main.cpp 
     
# 头文件查找路径  
INC = -I.  
# 目标文件  
OBJS = $(SRCS:.cpp=.o)  
# 链接为可执行文件  
$(TARGET):$(OBJS)  
#   @echo TARGET:$@  
#   @echo OBJECTS:$^  
	$(CC) $(CFLAGS) -o $@ $^  
clean:  
	rm -rf $(TARGET) $(OBJS)  
# 连续动作，请清除再编译链接，最后执行  
exec:clean $(TARGET)  
	@echo 开始执行  
	cp $(TARGET) a.out
	push.bat 
	@echo 执行结束  
# 编译规则 $@代表目标文件 $< 代表第一个依赖文件  
%.o:%.cpp  
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<  