exclude_dirs :=
dirs := $(shell find . -maxdepth 1 -type d)
dirs := $(basename $(patsubst ./%,%,$(dirs)))
dirs := $(filter-out $(exclude_dirs), $(dirs))
SUBDIRS := $(dirs)

all:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir all || exit 1; done

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean || exit 1; done

