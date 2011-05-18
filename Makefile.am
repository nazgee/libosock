## libosock:
## C++ wrapper of OpenSSL and standard sockets- server/client convinience class

## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

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
libosock_@OSOCK_API_VERSION@_la_SOURCES = src/Message/Message.cpp \
	src/Message/StringMessage.cpp \
	src/Message/ChainedMessage.cpp \
	src/Message/http/Path.cpp \
	src/Message/http/Request.cpp \
	src/Message/http/Protocole.cpp \
	src/Message/http/RequestLine.cpp \
	src/Security/SecurityServerUnsafe.cpp \
	src/Security/Trust/Trust.cpp \
	src/Security/SecurityServer.cpp \
	src/Security/SecurityClientSSL.cpp \
	src/Security/Key/Key.cpp \
	src/Security/SecurityServerSSL.cpp \
	src/Security/Security.cpp \
	src/Security/Certificate/Certificate.cpp \
	src/Security/SecurityClientUnsafe.cpp \
	src/Security/SecurityClient.cpp \
	src/Socket/SocketClient.cpp \
	src/Socket/SocketClientSSL.cpp \
	src/Socket/SocketServer.cpp \
	src/Socket/SocketClientUnsafe.cpp \
	src/Socket/SocketServerSSL.cpp \
	src/Socket/SocketServerUnsafe.cpp \
	src/Socket/Socket.cpp \
	src/Address/Address.cpp \
	src/Exception/Exception.cpp \
	src/Utilities/Logger.cpp \
	src/Utilities/SSLWrap.cpp \
	src/Server/Server.cpp

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
osockmessage_include_HEADERS = include/Message/StringMessage.h \
	include/Message/ChainedMessage.h \
	include/Message/http/Path.h \
	include/Message/http/Request.h \
	include/Message/http/Protocole.h \
	include/Message/http/RequestLine.h \
	include/Message/Message.h
	
osocksecurity_includedir = $(osock_includedir)/Security
osocksecurity_include_HEADERS = include/Security/Trust/Trust.h \
	include/Security/SecurityServerSSL.h \
	include/Security/Key/Key.h \
	include/Security/SecurityClientUnsafe.h \
	include/Security/SecurityServer.h \
	include/Security/Security.h \
	include/Security/Certificate/Certificate.h \
	include/Security/SecurityClient.h \
	include/Security/SecurityServerUnsafe.h \
	include/Security/SecurityClientSSL.h

osocksocket_includedir = $(osock_includedir)/Socket
osocksocket_include_HEADERS = include/Socket/SocketClientUnsafe.h \
	include/Socket/SocketServerUnsafe.h \
	include/Socket/SocketClient.h \
	include/Socket/Socket.h \
	include/Socket/SocketServer.h \
	include/Socket/SocketClientSSL.h \
	include/Socket/SocketServerSSL.h
	
osockaddress_includedir = $(osock_includedir)/Address
osockaddress_include_HEADERS = include/Address/Address.h

osockexception_includedir = $(osock_includedir)/Exception
osockexception_include_HEADERS = include/Exception/Exception.h			

osockloose_includedir = $(osock_includedir)
osockloose_include_HEADERS = include/osock.h \
	include/defines.h

## The generated configuration header is installed in its own subdirectory of
## $(libdir).  The reason for this is that the configuration information put
## into this header file describes the target platform the installed library
## has been built for.  Thus the file must not be installed into a location
## intended for architecture-independent files, as defined by the Filesystem
## Hierarchy Standard (FHS).
## The nodist_ prefix instructs Automake to not generate rules for including
## the listed files in the distribution on 'make dist'.  Files that are listed
## in _HEADERS variables are normally included in the distribution, but the
## configuration header file (osockconfig.h) is generated at configure time and 
## should not be shipped with the source tarball.
osock_libincludedir = $(libdir)/osock-$(OSOCK_API_VERSION)/include
nodist_osock_libinclude_HEADERS = osockconfig.h

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
