ifndef OS # linux
LIBRARY_EXTENSION=.a
RM=rm -f $(1)
else ifeq ($(OS), Windows_NT) # windows
LIBRARY_EXTENSION=.lib
RM=if exist $(1) del $(1)
else
$(error os not supported)
endif

libarray-mini$(LIBRARY_EXTENSION): array_mini.o
	$(call RM,libarray-mini$(LIBRARY_EXTENSION))
	ar rcs libarray-mini$(LIBRARY_EXTENSION) array_mini.o

array_mini.o: array_mini.c ../check-mini/check_mini.h
	gcc -c array_mini.c -I../check-mini/

test$(EXECUTABLE_EXTENSION): test.o libarray-mini$(LIBRARY_EXTENSION) ../test-mini/libtest-mini$(LIBRARY_EXTENSION)
	gcc -Wl,--wrap=malloc,--wrap=free,--wrap=main -o test$(EXECUTABLE_EXTENSION) test.o -L./ -larray-mini -L../test-mini/ -ltest-mini

test.o: test.c array_mini.h ../test-mini/test_mini.h
	gcc -c test.c -I../test-mini/

#******************************************************************************

.PHONY: release
release: test$(EXECUTABLE_EXTENSION)
	./test$(EXECUTABLE_EXTENSION)

.PHONY: clean
clean:
	$(call RM,array_mini.o)
	$(call RM,libarray-mini.a)
	$(call RM,libarray-mini.lib)