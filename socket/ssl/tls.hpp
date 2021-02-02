#include <iostream>
#include <synapse/socket/tcp/tcp.hpp>

#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace  network
{
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
	
	class  tls : public network::socket_base
	{
	public:
		tls			   (const char* ip, uint16_t port, 
					    crypto_type ssl_type = crypto_type::tls_v1_2);
		
		bool connect   ();
		void disconnect();
		
		size_t 	   send(uint8_t* ctx, size_t len) override;
		size_t 	   recv(uint8_t* ctx, size_t len) override;
	
	private:
		tls_context   ssl_ctx;
	};
}