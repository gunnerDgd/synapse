#include <iostream>
#include <synapse/socket/tcp/tcp.hpp>

#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace synapse {
namespace network {
	
	enum   crypto_type
	{
		ssl_v23,
		tls_v1,
		tls_v1_2
	};
	
	struct tls_context
	{
		SSL_CTX    		 *ctx;
   		SSL    	   		 *ssl;
    	X509       		 *client_cert;
    	const SSL_METHOD *meth;
	};
	
	class tls : public network::socket_base
	{
	public:
		tls(synapse::network::ip::v4 tls_addr,
			crypto_type 			 tls_type = crypto_type::tls_v1_2);
		
		bool   connect   ();
		void   disconnect();
		
		size_t send(uint8_t* ctx, size_t len) override;
		size_t recv(uint8_t* ctx, size_t len) override;
	
	private:
		tls_context   ssl_ctx;
	};

}
}

#include "tls.hpp"

synapse::network::tls::tls(synapse::network::ip::v4      tls_addr,
				  		   synapse::network::crypto_type tls_type)
	: synapse::network::socket_base(tls_addr)
{
	SSL_library_init	  ();
    SSL_load_error_strings();
	
	switch(ssl_type)
	{
		case crypto_type::ssl_v23:
			ssl_ctx.meth = SSLv23_client_method ();
			break;
		
		case crypto_type::tls_v1:
			ssl_ctx.meth = TLSv1_client_method  ();
			break;
			
		case crypto_type::tls_v1_2:
			ssl_ctx.meth = TLSv1_2_client_method();
			break;
	}
	ssl_ctx.ctx      = SSL_CTX_new(ssl_ctx.meth);
	if(!ssl_ctx.ctx)   goto initializer_error;
	
	socket_fd        = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket_fd <= 0) goto initializer_error;
		
	ssl_ctx.ssl = SSL_new (ssl_ctx.ctx);
	SSL_set_fd			  (ssl_ctx.ssl, socket_fd);

initializer_error:
	this->set_state(synapse::io::io_state::internal_error);
	return;
}
	
bool synapse::network::tls::connect   ()
{
	if(::connect(socket_fd,
				(sockaddr*)&socket_address,
				 sizeof(sockaddr_in)) != 0) goto connection_error;
	
	if(SSL_connect(ssl_ctx.ssl) != 1)		goto connection_error;
	return true;

connection_error:
	this->set_state(synapse::io::io_state::internal_error);
	return false;
}

void synapse::network::tls::disconnect()
{
	SSL_free	(ssl_ctx.ssl);
	close		(socket_fd)  ;
	SSL_CTX_free(ssl_ctx.ctx);
}

size_t 	   synapse::network::tls::send(uint8_t* ctx, size_t len)
{
	int 	 s_bytes = 0;
	if(io_state_flag != synapse::io::io_state::normal) return 0;
	
	return ((s_bytes = SSL_write(ssl_ctx.ssl, ctx, len)) > 0)
		  ? (size_t)s_bytes
		  : 0;
}

size_t 	   synapse::network::tls::recv(uint8_t* ctx, size_t len)
{
	int 	 r_bytes = 0;
	return ((r_bytes = SSL_read(ssl_ctx.ssl, ctx, len)) > 0)
		  ? (size_t)r_bytes
		  : 0;
}