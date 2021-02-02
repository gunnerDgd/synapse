#include "tls.hpp"

network::tls::tls(const char* ip, 
				  uint16_t    port,
				  crypto_type ssl_type)
	: network::socket_base(ip, port)
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
	ssl_ctx.ctx  = SSL_CTX_new(ssl_ctx.meth);
	
	if(!ssl_ctx.ctx)   return;
	
	socket_fd        = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket_fd <= 0) return;
		
	ssl_ctx.ssl = SSL_new (ssl_ctx.ctx);
	SSL_set_fd			  (ssl_ctx.ssl, socket_fd);
}
	
bool network::tls::connect   ()
{
	if(::connect(socket_fd,
				(sockaddr*)&socket_address,
				 sizeof(sockaddr_in)) != 0) return false;
	
	return (SSL_connect(ssl_ctx.ssl) == 1)
		  ? true
		  : false;
}

void network::tls::disconnect()
{
	SSL_free	(ssl_ctx.ssl);
	close		(socket_fd)  ;
	SSL_CTX_free(ssl_ctx.ctx);
}

size_t 	   network::tls::send(uint8_t* ctx, size_t len)
{
	int 	 s_bytes = 0;
	return ((s_bytes = SSL_write(ssl_ctx.ssl, ctx, len)) > 0)
		  ? (size_t)s_bytes
		  : 0;
}

size_t 	   network::tls::recv(uint8_t* ctx, size_t len)
{
	int 	 r_bytes = 0;
	return ((r_bytes = SSL_read(ssl_ctx.ssl, ctx, len)) > 0)
		  ? (size_t)r_bytes
		  : 0;
}