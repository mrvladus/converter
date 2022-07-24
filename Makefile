NAME=converter
LIBS=gtk4 libadwaita-1 vte-2.91-gtk4
BUILDDIR=./build
LOCALESDIR=$(BUILDDIR)/locales

all: compile

prepare:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(LOCALESDIR)/ru/LC_MESSAGES
	
translate:
	@xgettext ./data/ui/window.ui -o ./po/converter.pot
	@msgfmt -c ./po/ru.po -o $(LOCALESDIR)/ru/LC_MESSAGES/converter.mo

compile: prepare translate
	@glib-compile-resources ./data/converter.gresource.xml --target=./src/resources.c --generate-source
	gcc -Wall `pkg-config --cflags $(LIBS)` -o $(BUILDDIR)/$(NAME) ./src/main.c ./src/resources.c `pkg-config --libs $(LIBS)`
	@rm ./src/resources.c

clean:
	rm -rf $(BUILDDIR)