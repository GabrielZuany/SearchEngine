# Makefile que percorre toda a biblioteca, procurando os códigos fontes .c para compilar

LIBCONTAINERSDIR       = containerslib
LIBCONTAINERS          = containers
LIBCONTAINERSDIRRELEASE = $(LIBCONTAINERSDIR)/build/release
LIBCONTAINERSDIRDEBUG   = $(LIBCONTAINERSDIR)/build/debug

LIBPAGEINDEXERDIR       = page_indexerlib
LIBPAGEINDEXER          = page_indexer
LIBPAGEINDEXERDIRRELEASE = $(LIBPAGEINDEXERDIR)/build/release
LIBPAGEINDEXERDIRDEBUG   = $(LIBPAGEINDEXERDIR)/build/debug

LIBGOOGLEPAGERANKERDIR       = google_page_rankerlib
LIBGOOGLEPAGERANKER          = google_page_ranker
LIBGOOGLEPAGERANKERDIRRELEASE = $(LIBGOOGLEPAGERANKERDIR)/build/release
LIBGOOGLEPAGERANKERDIRDEBUG   = $(LIBGOOGLEPAGERANKERDIR)/build/debug

LIBENGINEDIR       = google_page_rankerlib
LIBENGINE          = google_page_ranker
LIBENGINEDIRRELEASE = $(LIBENGINEDIR)/build/release
LIBENGINEDIRDEBUG   = $(LIBENGINEDIR)/build/debug

OUT            = main

BUILDIR = build
DEBUGDIR = $(BUILDIR)/debug
RELEASEDIR = $(BUILDIR)/release

# CCompiler - é o compilador usado
# CFLAGS - são as flags dadas ao compilador durante a compilação
CC = gcc
override CFLAGS += -Wall -Wextra -I $(LIBENGINEDIR)/include -I $(LIBGOOGLEPAGERANKERDIR)/include -I $(LIBPAGEINDEXERDIR)/include -I $(LIBCONTAINERSDIR)/include -lm

DEBUGFLAGS = -ggdb3 -O0 -DDEBUG
RELEASEFLAGS = -O3 -DNDEBUG

MAKE = make

VALGRINDOUT = valgrind-out.txt

all : release

release : pre-build-release build-release

debug: pre-build-debug build-debug

# Preparativos para o build; Garante que o diretório build/ existe
pre-build:
	mkdir -p $(BUILDIR)/

pre-build-release: pre-build
	mkdir -p $(RELEASEDIR)/
	$(MAKE) -C $(LIBENGINEDIR)/
	$(MAKE) -C $(LIBGOOGLEPAGERANKERDIR)/
	$(MAKE) -C $(LIBPAGEINDEXERDIR)/
	$(MAKE) -C $(LIBCONTAINERSDIR)/

pre-build-debug: pre-build
	mkdir -p $(DEBUGDIR)/
	$(MAKE) -C $(LIBENGINEDIR)/ debug
	$(MAKE) -C $(LIBGOOGLEPAGERANKERDIR)/ debug
	$(MAKE) -C $(LIBPAGEINDEXERDIR)/ debug
	$(MAKE) -C $(LIBCONTAINERSDIR)/ debug

build-release : main.c
	$(CC) -o $(RELEASEDIR)/$(OUT) $< -L $(LIBENGINEDIRRELEASE) -l$(LIBENGINE) -L $(LIBGOOGLEPAGERANKERDIRRELEASE) -l$(LIBGOOGLEPAGERANKER) -L $(LIBPAGEINDEXERDIRRELEASE) -l$(LIBPAGEINDEXER) -L $(LIBCONTAINERSDIRRELEASE) -l$(LIBCONTAINERS) $(CFLAGS) $(RELEASEFLAGS)

build-debug : main.c
	$(CC) -o $(DEBUGDIR)/$(OUT) $< -L $(LIBENGINEDIRDEBUG) -l$(LIBENGINE) -L $(LIBGOOGLEPAGERANKERDIRDEBUG) -l$(LIBGOOGLEPAGERANKER) -L $(LIBPAGEINDEXERDIRDEBUG) -l$(LIBPAGEINDEXER) -L $(LIBCONTAINERSDIRDEBUG) -l$(LIBCONTAINERS) $(CFLAGS) $(DEBUGFLAGS)

# Rode make clean para remover tudo que é gerado pela build
clean:
	rm -rf $(BUILDIR)/ $(VALGRINDOUT)
	$(MAKE) -C $(LIBENGINEDIR)/ clean
	$(MAKE) -C $(LIBGOOGLEPAGERANKERDIR)/ clean
	$(MAKE) -C $(LIBPAGEINDEXERDIR)/ clean
	$(MAKE) -C $(LIBCONTAINERSDIR)/ clean

valgrind: debug
	rm -f $(VALGRINDOUT)

	valgrind --leak-check=full \
			 --show-leak-kinds=all \
			 --track-origins=yes \
			 --verbose \
			 --log-file=$(VALGRINDOUT) \
			 ./$(DEBUGDIR)/$(OUT) $(VALARGS)

	@echo "Head to $(VALGRINDOUT)"
