#include <synapse/stream/stream.hpp>
#include <thread>

namespace stream
{
	class adapter
	{
	public:
		adapter(stream::stream& _in, stream::stream& _out)
			: a_in 		 (&_in),
			  a_out		 (&_out),
			  a_th_filter(new std::thread(&adapter::a_fi_func, this)) {}
		
		~adapter()
		{
			a_fi_run = false;
			a_in       ->close();
			a_out      ->close();
			
			a_th_filter->join();
			delete a_th_filter ;
		}
		
		stream::stream* in (stream::stream& _in) 
		{
			stream::stream* p_in = a_in;
			a_in				 = _in ;
			
			return p_in;
		}
		
		stream::stream* out(stream::stream& _out)
		{
			stream::stream* p_out = a_out;
			a_in				  = _out ;
			
			return p_out;
		}
		
	private:
		stream::stream* a_in,
					  * a_out;
		
		std::thread   * a_th_filter;
		void			a_fi_func();
		bool			a_fi_run = true;
	};
}

void stream::adapter::a_fi_func()
{
	uint8_t b_fi[4096] = { 0x00, };
	
	while(a_fi_run) {
		int i_size = a_in ->read (b_fi, 4096);
		int o_size = a_out->write(b_fi, i_size);
	}
}