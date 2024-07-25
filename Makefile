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

array_mini.o: array_mini.c
	gcc -c array_mini.c

#******************************************************************************

.PHONY: clean
clean:
	$(call RM,array_mini.o)
	$(call RM,libarray-mini.a)
	$(call RM,libarray-mini.lib)