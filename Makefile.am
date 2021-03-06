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
unittest_SOURCES  = src/Testing/main.cpp
unittest_SOURCES += src/Testing/Unit/Message/StringMessageTest.cpp
unittest_SOURCES += src/Testing/Unit/Message/MessageTest.cpp
unittest_SOURCES += src/Testing/Unit/Server/EchoServerTest.cpp
unittest_SOURCES += src/Testing/Unit/BIO/BIOTest.cpp
## Prepare linker flags
unittest_LDFLAGS  = $(OPENSSL_LIBS)
unittest_LDFLAGS += $(CPPUNIT_LIBS)
unittest_LDFLAGS += $(BOOST_THREAD_LDFLAGS)
unittest_LDFLAGS += $(BOOST_THREAD_LIBS)
unittest_LDFLAGS += -L$(top_srcdir) -lpthread -losock-$(OSOCK_API_VERSION)
## Prepare compiler flags
unittest_CXXFLAGS  = -Iinclude -Iinclude/Testing -Wall 
unittest_CXXFLAGS += $(OPENSSL_CFLAGS)
unittest_CXXFLAGS += $(CPPUNIT_CFLAGS)
unittest_CXXFLAGS += $(BOOST_CPPFLAGS)

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
	src/Message/http/Command.cpp \
	src/Message/http/Protocole.cpp \
	src/Message/http/Header.cpp \
	src/Message/http/Status.cpp \
	src/Message/http/Request.cpp \
	src/Message/http/Response.cpp \
	src/Message/http/HttpRequest.cpp \
	src/Message/http/HttpResponse.cpp \
	src/Message/http/HttpResponseFile.cpp \
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
	src/Address/Address.cpp \
	src/Exception/Exception.cpp \
	src/Utilities/Logger.cpp \
	src/Utilities/SSLWrap.cpp \
	src/Utilities/Utils.cpp \
	src/Utilities/sd-daemon.cpp \
	src/Server/Server.cpp \
	src/Server/ServerEcho.cpp \
	src/Security/AuthSSL.cpp \
	src/Security/AuthNone.cpp \
	src/Security/SSLContext.cpp \
	src/Message/Parser.cpp \
	src/BIO/BIOSocket.cpp \
	src/BIO/BIOSSL.cpp \
	src/BIO/BIO.cpp

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libosock_@OSOCK_API_VERSION@_la_LDFLAGS = -fPIC -version-info $(OSOCK_SO_VERSION)
libosock_@OSOCK_API_VERSION@_la_LDFLAGS += $(OPENSSL_LIBS)
libosock_@OSOCK_API_VERSION@_la_LDFLAGS += $(BOOST_THREAD_LDFLAGS)
libosock_@OSOCK_API_VERSION@_la_LDFLAGS += $(BOOST_THREAD_LIBS)
libosock_@OSOCK_API_VERSION@_la_CXXFLAGS = -fPIC -I$(top_srcdir)/include $(OPENSSL_CFLAGS) -Wall
libosock_@OSOCK_API_VERSION@_la_CXXFLAGS += $(BOOST_CPPFLAGS)

#Add include dirs needed for a successfull compilation
#AM_XXFLAGS = -I$(top_srcdir)/include/ -I./include/
AM_CFLAGS = -I$(top_srcdir)/include/

## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
osock_includedir = $(includedir)/osock-$(OSOCK_API_VERSION)

osockmessage_includedir = $(osock_includedir)/Message
osockmessage_include_HEADERS = include/Message/StringMessage.h \
	include/Message/ChainedMessage.h \
	include/Message/Message.h \
	include/Message/Parser.h

osockmessagehttp_includedir = $(osock_includedir)/Message/http
osockmessagehttp_include_HEADERS = include/Message/http/Path.h \
	include/Message/http/Command.h \
	include/Message/http/Protocole.h \
	include/Message/http/Header.h \
	include/Message/http/Status.h \
	include/Message/http/Request.h \
	include/Message/http/Response.h \
	include/Message/http/HttpRequest.h \
	include/Message/http/HttpResponse.h \
	include/Message/http/HttpResponseFile.h

osocksecurity_includedir = $(osock_includedir)/Security
osocksecurity_include_HEADERS = include/Security/Trust/Trust.h \
	include/Security/Certificate/Certificate.h \
	include/Security/Key/Key.h \
	include/Security/SSLContext.h \
	include/Security/Auth.h \
	include/Security/AuthNone.h \
	include/Security/AuthSSL.h \
	include/Security/Security.h \
	include/Security/SecurityServer.h \
	include/Security/SecurityServerSSL.h \
	include/Security/SecurityServerUnsafe.h \
	include/Security/SecurityClient.h \
	include/Security/SecurityClientUnsafe.h \
	include/Security/SecurityClientSSL.h

osockserver_includedir = $(osock_includedir)/Server
osockserver_include_HEADERS = include/Server/Server.h \
	include/Server/ServerEcho.h

osockutilities_includedir = $(osock_includedir)/Utilities
osockutilities_include_HEADERS = include/Utilities/Logger.h

osockbio_includedir = $(osock_includedir)/BIO
osockbio_include_HEADERS = include/BIO/BIO.h \
	include/BIO/BIOSocket.h \
	include/BIO/BIOSSL.h
	
osockaddress_includedir = $(osock_includedir)/Address
osockaddress_include_HEADERS = include/Address/Address.h

osockexception_includedir = $(osock_includedir)/Exception
osockexception_include_HEADERS = include/Exception/Exception.h

osockloose_includedir = $(osock_includedir)
osockloose_include_HEADERS = include/osock.h \
	include/defines.h \
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
