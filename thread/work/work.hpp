#include <iostream>
#include <synapse/sync/event.hpp>

namespace thread
{
	class work_base
	{
	public:
		virtual synchronous::event& wk_launch() = 0;
	private:
		synchronous::event			wk_event;
	};
	
	template <typename wk_out, typename... wk_in>
	class work
	{
	public:
		work(std::function<wk_out(wk_in...)> _fn);
		
		
		
	private:
		std::function<wk_out(wk_in...)> wk_func;
		wk_launch();
	}
}