/*
	Copyright 2011, Michal Stawinski <michal.stawinski@gmail.com>

	This file is part of libsockets- C++ wrapper over OpenSSL and raw sockets.

	libsockets is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libsockets is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with libsockets.  If not, see <http://www.gnu.org/licenses/>.
*/

#define DEBUG_WANTED

#include <Utilities/SSLWrap.h>
#include <Utilities/Logger.h>
#include <defines.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>

namespace SSLWrap
{
	SSL_CTX *SSL_CTX_new(SSL_METHOD *meth)
	{
		SSL_CTX* ctx;
		ctx = ::SSL_CTX_new(meth);
		DBG_FUNC_NOLINE <<  "( meth=" << (void*) meth << " ) : " << (void*)ctx << std::endl;
		return ctx;
	}

	void SSL_CTX_free(SSL_CTX *ctx)
	{
		::SSL_CTX_free(ctx);
		DBG_FUNC_NOLINE <<  "( ctx=" << (void*)ctx << " ) : void" << std::endl;
	}

	void SSL_CTX_set_default_passwd_cb(SSL_CTX *ctx, pem_password_cb *cb)
	{
		::SSL_CTX_set_default_passwd_cb(ctx, cb);
		DBG_FUNC_NOLINE <<  "( ctx=" << (void*)ctx << ", cb=" << (void*)cb << " ) : void" << std::endl;
	}

	void SSL_CTX_set_default_passwd_cb_userdata(SSL_CTX *ctx, void *u)
	{
		::SSL_CTX_set_default_passwd_cb_userdata(ctx, u);
		DBG_FUNC_NOLINE <<  "( ctx=" << (void*) ctx << ", u=" << u << " ) : void" << std::endl;
	}

	BIO *BIO_new_ssl(SSL_CTX *ctx,int client)
	{
		BIO* b;
		b = ::BIO_new_ssl(ctx, client);
		DBG_FUNC_NOLINE <<  "( ctx=" << (void*) ctx << ", client=" << client <<" ) : " << (void*)b << std::endl;
		return b;
	}

	BIO *BIO_new_accept(char *host_port)
	{
		BIO* b;
		b = ::BIO_new_accept(host_port);
		DBG_FUNC_NOLINE <<  "( host_port=" << host_port << " ) : " << (void*)b << std::endl;
		return b;
	}

	BIO *BIO_pop(BIO *b)
	{
		BIO* bret;
		bret = ::BIO_pop(b);
		DBG_FUNC_NOLINE <<  "( b=" << b << " ) : " << (void*)bret << std::endl;
		return bret;
	}

	long BIO_set_close_(BIO* b, int f)
	{
		long l;
		if (f == BIO_NOCLOSE)
			l = BIO_set_close(b, BIO_NOCLOSE);
		else
			l = BIO_set_close(b, BIO_CLOSE);

		DBG_FUNC_NOLINE <<  "( b=" << b << ", f=" << f << " ) : " << l << std::endl;
		return l;
	}

	long BIO_do_handshake_(BIO* b)
	{
		long l = ::BIO_do_handshake(b);
		DBG_FUNC_NOLINE <<  "( b=" << b << " ) : " << l << std::endl;
		return l;
	}

	long BIO_do_accept_(BIO* b)
	{
		long l = ::BIO_do_accept(b);
		DBG_FUNC_NOLINE <<  "( b=" << b << " ) : " << l << std::endl;
		return l;
	}

	long BIO_set_accept_bios_(BIO* b, BIO* bio)
	{
		long l = ::BIO_set_accept_bios(b, bio);
		DBG_FUNC_NOLINE <<  "( b=" << b << ", bio=" << bio << " ) : " << l << std::endl;
		return l;
	}

	long BIO_get_ssl_(BIO* b, SSL** ssl)
	{
		long l = ::BIO_get_ssl(b, ssl);
		DBG_FUNC_NOLINE <<  "( b=" << b << ", *ssl=" << *ssl << " ) : " << l << std::endl;
		return l;
	}

	SSL_METHOD *TLSv1_server_method(void)
	{
		SSL_METHOD *m;
		m = ::TLSv1_server_method();
		DBG_FUNC_NOLINE <<  "() : " << (void*)m << std::endl;
		return m;
	}

	SSL_METHOD *SSLv2_server_method(void)
	{
		SSL_METHOD *m;
		m = ::SSLv2_server_method();
		DBG_FUNC_NOLINE <<  "() : " << (void*)m << std::endl;
		return m;
	}

	SSL_METHOD *SSLv3_server_method(void)
	{
		SSL_METHOD *m;
		m = ::SSLv3_server_method();
		DBG_FUNC_NOLINE << "SSLv3_server_method() : " << (void*)m << std::endl;
		return m;
	}

	SSL_METHOD *SSLv23_server_method(void)
	{
		SSL_METHOD *m;
		m = ::SSLv23_server_method();
		DBG_FUNC_NOLINE <<  "() : " << (void*)m << std::endl;
		return m;
	}

	int BIO_should_retry_(BIO* b)
	{
		int l = ::BIO_should_retry(b);
		DBG_FUNC_NOLINE <<  "( b=" << b << " ) : " << l << std::endl;
		return l;
	}

	void BIO_free_all(BIO *b)
	{
		::BIO_free_all(b);
		DBG_FUNC_NOLINE << "( b=" << b << " ) : void" << std::endl;
	}

	int	BIO_free(BIO *b)
	{
		int i = ::BIO_free(b);
		DBG_FUNC_NOLINE << "( b=" << b << " ) : " << i << std::endl;
		return i;
	}

	void BIO_vfree(BIO *b)
	{
		::BIO_vfree(b);
		DBG_FUNC_NOLINE << "( b=" << b << " ) : void" << std::endl;
	}

	void SSL_free(SSL *ssl)
	{
		::SSL_free(ssl);
		DBG_FUNC_NOLINE << "( ssl=" << ssl << " ) : void" << std::endl;
	}

} //namespace SSL
