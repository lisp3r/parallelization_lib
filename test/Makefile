PRG_NAME	=	thread_pool_test
CPP_SRCS	:=	$(wildcard *.cpp)
OBJS		:=	${CPP_SRCS:.cpp=.o}

INCLUDE_DIR = ../include

CPPFLAGS += $(foreach inc_dir,$(INCLUDE_DIR),-I$(inc_dir))
LIBFLAGS := -pthread -luuid

all: $(PRG_NAME)

$(PRG_NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(PRG_NAME) $(LDFLAGS) $(LIBFLAGS)

%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<
