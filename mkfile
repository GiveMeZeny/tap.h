P := $(addprefix bin/, $(basename $(TESTS)))
R := $(addprefix src/, $(TESTS))

all: bin/ $(P)

bin/:
	@mkdir bin

$(P): $(R)
	$(eval F := $(firstword $(R)))
	$(eval R := $(R:$(F)=))
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(F)
	$(eval tfile := $(basename $(notdir $@)).t)
	@echo "#!/usr/bin/perl"                     > $(tfile)
	@echo "\$$0 =~ /(.*)(\/|\\\\)(.*)\.t\$$/;" >> $(tfile)
	@echo "exec(\"\$$1\$$2bin\$$2\$$3\");"     >> $(tfile)

clean:
	@$(RM) -rf bin *.t
