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

#ifndef SSLWRAP_H_
#define SSLWRAP_H_

#include <openssl/ssl.h>

namespace SSLWrap
{
	SSL_CTX *SSL_CTX_new(SSL_METHOD *meth);
	void	SSL_CTX_free(SSL_CTX *ctx);
	void SSL_CTX_set_default_passwd_cb(SSL_CTX *ctx, pem_password_cb *cb);
	void SSL_CTX_set_default_passwd_cb_userdata(SSL_CTX *ctx, void *u);
	BIO *BIO_new_ssl(SSL_CTX *ctx,int client);
	BIO *BIO_new_accept(char *host_port);
	BIO *BIO_pop(BIO *b);
	long BIO_set_close_(BIO* b, int flags);
	long BIO_do_handshake_(BIO* b);
	long BIO_do_accept_(BIO* b);
	long BIO_set_accept_bios_(BIO* b, BIO* bio);
	long BIO_get_ssl_(BIO* b, SSL** ssl);
	SSL_METHOD *TLSv1_server_method(void);
	SSL_METHOD *SSLv2_server_method(void);
	SSL_METHOD *SSLv3_server_method(void);
	SSL_METHOD *SSLv23_server_method(void);
	int BIO_should_retry_(BIO* b);
	void BIO_free_all(BIO *b);
	int	BIO_free(BIO *b);
	void BIO_vfree(BIO *b);
	void SSL_free(SSL *ssl);

} //namespace SSL
#endif /* SSLWRAP_H_ */
