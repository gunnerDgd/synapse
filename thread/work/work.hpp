#include <synapse/sync/event/message_event.hpp>

namespace thread
{
	class work_base
	{
	public:
		virtual synchronous::event& run() = 0;
	private:
		synchronous::signal			wk_signal;
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