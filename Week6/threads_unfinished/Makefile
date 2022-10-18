# SOURCES = containers.cpp ref_pointers_templates.cpp assorted.cpp
CPPC = g++
CPPFLAGS = -Wall -O2 -lpthread
TARGET =  basicthread workdivison kinglear 

all: basicthread workdivison kinglear 

%.o : %.c



basicthread: basicthread.cpp
	$(CPPC) $(CPPFLAGS) -o $@ basicthread.cpp

workdivison: workdivison.cpp
	$(CPPC) $(CPPFLAGS) -o $@ workdivison.cpp

kinglear: kinglear.cpp
	$(CPPC) $(CPPFLAGS) -o $@ kinglear.cpp

# ref_pointers_templates: ref_pointers_templates.cpp
# 	$(CPPC) $(CPPFLAGS) -o $@ ref_pointers_templates.cpp

# assorted: assorted.cpp
# 	$(CPPC) $(CPPFLAGS) -o $@ assorted.cpp


.PHONY: clean
clean:
	rm -f *~ *.o $(TARGET) 
