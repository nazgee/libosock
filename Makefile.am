## libosock:
## C++ wrapper of OpenSSL and standard sockets- server/client convinience class

include $(top_srcdir)/aminclude.am

## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
## -I m4 will make sure, that all macros in m4 directory will be available
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

## Define name of the binaries that should be built along with our library.
## In the end this should be conditional, but for now we are good with building
## it every time
EXTRA_PROGRAMS = unittest
## Define the source file list for the "unittestlibosock" target
unittest_SOURCES  = $(TST_SRC_DIR)/main.cpp
unittest_SOURCES += $(TST_SRC_DIR)/Unit/Message/StringMessageTest.cpp
unittest_SOURCES += $(TST_SRC_DIR)/Unit/Message/MessageTest.cpp
## Prepare linker flags
unittest_LDFLAGS  = $(OPENSSL_LIBS)
unittest_LDFLAGS += $(CPPUNIT_LIBS)
unittest_LDFLAGS += -L$(top_srcdir) -losock-$(OSOCK_API_VERSION)
## Prepare compiler flags
unittest_CXXFLAGS  = -I$(LIB_INC_DIR) -I$(TST_INC_DIR) -Wall 
unittest_CXXFLAGS += $(OPENSSL_CFLAGS)
unittest_CXXFLAGS += $(CPPUNIT_CFLAGS) 

## Define a libtool archive target "libosock-@OSOCK_API_VERSION@.la", with
## @OSOCK_API_VERSION@ substituted into the generated Makefile at configure
## time.
## The libtool archive file (.la) will be installed into the directory named
## by the predefined variable $(bindir), along with the actual shared library
## file (.so).
lib_LTLIBRARIES = libosock-@OSOCK_API_VERSION@.la

## Define the source file list for the "libosock-@OSOCK_API_VERSION@.la"
## target.  Note that @OSOCK_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## The file extension .cc is recognized by Automake, and makes it produce
## rules which invoke the C++ compiler to produce a libtool object file (.lo)
## from each source file.  Note that it is not necessary to list header files
## which are already listed elsewhere in a _HEADERS variable assignment.
libosock_@OSOCK_API_VERSION@_la_SOURCES = $(LIB_SRC_DIR)/Message/Message.cpp \
	$(LIB_SRC_DIR)/Message/StringMessage.cpp \
	$(LIB_SRC_DIR)/Message/ChainedMessage.cpp \
	$(LIB_SRC_DIR)/Message/http/Path.cpp \
	$(LIB_SRC_DIR)/Message/http/Command.cpp \
	$(LIB_SRC_DIR)/Message/http/Protocole.cpp \
	$(LIB_SRC_DIR)/Message/http/Header.cpp \
	$(LIB_SRC_DIR)/Message/http/Status.cpp \
	$(LIB_SRC_DIR)/Message/http/Request.cpp \
	$(LIB_SRC_DIR)/Message/http/Response.cpp \
	$(LIB_SRC_DIR)/Message/http/HttpRequest.cpp \
	$(LIB_SRC_DIR)/Message/http/HttpResponse.cpp \
	$(LIB_SRC_DIR)/Message/http/HttpResponseFile.cpp \
	$(LIB_SRC_DIR)/Security/SecurityServerUnsafe.cpp \
	$(LIB_SRC_DIR)/Security/Trust/Trust.cpp \
	$(LIB_SRC_DIR)/Security/SecurityServer.cpp \
	$(LIB_SRC_DIR)/Security/SecurityClientSSL.cpp \
	$(LIB_SRC_DIR)/Security/Key/Key.cpp \
	$(LIB_SRC_DIR)/Security/SecurityServerSSL.cpp \
	$(LIB_SRC_DIR)/Security/Security.cpp \
	$(LIB_SRC_DIR)/Security/Certificate/Certificate.cpp \
	$(LIB_SRC_DIR)/Security/SecurityClientUnsafe.cpp \
	$(LIB_SRC_DIR)/Security/SecurityClient.cpp \
	$(LIB_SRC_DIR)/Socket/SocketClient.cpp \
	$(LIB_SRC_DIR)/Socket/SocketClientSSL.cpp \
	$(LIB_SRC_DIR)/Socket/SocketServer.cpp \
	$(LIB_SRC_DIR)/Socket/SocketClientUnsafe.cpp \
	$(LIB_SRC_DIR)/Socket/SocketServerSSL.cpp \
	$(LIB_SRC_DIR)/Socket/SocketServerUnsafe.cpp \
	$(LIB_SRC_DIR)/Socket/Socket.cpp \
	$(LIB_SRC_DIR)/Address/Address.cpp \
	$(LIB_SRC_DIR)/Exception/Exception.cpp \
	$(LIB_SRC_DIR)/Utilities/Logger.cpp \
	$(LIB_SRC_DIR)/Utilities/SSLWrap.cpp \
	$(LIB_SRC_DIR)/Utilities/Utils.cpp \
	$(LIB_SRC_DIR)/Server/Server.cpp

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libosock_@OSOCK_API_VERSION@_la_LDFLAGS = -version-info $(OSOCK_SO_VERSION)
libosock_@OSOCK_API_VERSION@_la_LDFLAGS += $(OPENSSL_LIBS)
libosock_@OSOCK_API_VERSION@_la_CXXFLAGS = -I$(top_srcdir)/include $(OPENSSL_CFLAGS) -Wall

#Add include dirs needed for a successfull compilation
#AM_CXXFLAGS = -I$(top_srcdir)/include -Wall $(OPENSSL_CFLAGS)

## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
osock_includedir = $(includedir)/osock-$(OSOCK_API_VERSION)

osockmessage_includedir = $(osock_includedir)/Message
osockmessage_include_HEADERS = $(LIB_INC_DIR)/Message/StringMessage.h \
	$(LIB_INC_DIR)/Message/ChainedMessage.h \
	$(LIB_INC_DIR)/Message/http/Path.h \
	$(LIB_INC_DIR)/Message/http/Command.h \
	$(LIB_INC_DIR)/Message/http/Protocole.h \
	$(LIB_INC_DIR)/Message/http/Header.h \
	$(LIB_INC_DIR)/Message/http/Status.h \
	$(LIB_INC_DIR)/Message/http/Request.h \
	$(LIB_INC_DIR)/Message/http/Response.h \
	$(LIB_INC_DIR)/Message/http/HttpRequest.h \
	$(LIB_INC_DIR)/Message/http/HttpResponse.h \
	$(LIB_INC_DIR)/Message/http/HttpResponseFile.h \
	$(LIB_INC_DIR)/Message/Message.h
	
osocksecurity_includedir = $(osock_includedir)/Security
osocksecurity_include_HEADERS = $(LIB_INC_DIR)/Security/Trust/Trust.h \
	$(LIB_INC_DIR)/Security/SecurityServerSSL.h \
	$(LIB_INC_DIR)/Security/Key/Key.h \
	$(LIB_INC_DIR)/Security/SecurityClientUnsafe.h \
	$(LIB_INC_DIR)/Security/SecurityServer.h \
	$(LIB_INC_DIR)/Security/Security.h \
	$(LIB_INC_DIR)/Security/Certificate/Certificate.h \
	$(LIB_INC_DIR)/Security/SecurityClient.h \
	$(LIB_INC_DIR)/Security/SecurityServerUnsafe.h \
	$(LIB_INC_DIR)/Security/SecurityClientSSL.h

osocksocket_includedir = $(osock_includedir)/Socket
osocksocket_include_HEADERS = $(LIB_INC_DIR)/Socket/SocketClientUnsafe.h \
	$(LIB_INC_DIR)/Socket/SocketServerUnsafe.h \
	$(LIB_INC_DIR)/Socket/SocketClient.h \
	$(LIB_INC_DIR)/Socket/Socket.h \
	$(LIB_INC_DIR)/Socket/SocketServer.h \
	$(LIB_INC_DIR)/Socket/SocketClientSSL.h \
	$(LIB_INC_DIR)/Socket/SocketServerSSL.h
	
osockaddress_includedir = $(osock_includedir)/Address
osockaddress_include_HEADERS = $(LIB_INC_DIR)/Address/Address.h

osockexception_includedir = $(osock_includedir)/Exception
osockexception_include_HEADERS = $(LIB_INC_DIR)/Exception/Exception.h			

osockloose_includedir = $(osock_includedir)
osockloose_include_HEADERS = $(LIB_INC_DIR)/osock.h \
	$(LIB_INC_DIR)/defines.h \
	osockconfig.h

## The generated configuration header is installed in its own subdirectory of
## $(libdir).  The reason for this is that the configuration information put
## into this header file describes the target platform the installed library
## has been built for.  Thus the file must not be installed into a location
## intended for architecture-independent files, as defined by the Filesystem
## Hierarchy Standard (FHS).
## The nodist_ prefix instructs Automake to not generate rules for including
## the listed files in the distribution on 'make dist'.  Files that are listed
## in _HEADERS variables are normally included in the distribution, but the
## configuration header file (config.h) is generated at configure time and 
## should not be shipped with the source tarball.
osock_libincludedir = $(libdir)/osock-$(OSOCK_API_VERSION)/include
nodist_osock_libinclude_HEADERS = config.h

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = osock-$(OSOCK_API_VERSION).pc

## Define an independent executable script for inclusion in the distribution
## archive.  However, it will not be installed on an end user's system due to
## the noinst_ prefix.
dist_noinst_SCRIPTS = autogen.sh
