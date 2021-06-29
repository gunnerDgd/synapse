#include <synapse/sync/lock.hpp>
#include <functional>

namespace synapse {
namespace thread  {
	
	class work_base
	{
	public:
		work_base(synapse::synchronize::lock& wb_lock)
			: work_notifier(wb_lock) { }
	
		virtual void 				run	 	() = 0;
		synapse::synchronize::lock& notifier() { return work_notifier; }

	protected:
		synapse::synchronize::lock work_notifier;
	};
	
	template <typename T>
	class work { };

	template <typename R, typename... Arg>
	class work<R(Arg...)> : public work_base
	{
	public:
		work(std::function<R(Arg...)> work_func, Arg... work_arg);

		void run		 ();
		R&	 return_value() { return work_return_value; }
		
	private:
		std::function<R(Arg...)> work_function;
		std::tuple<Arg...>		 work_argument;
		R						 work_return_value;
	};

}
}

template <typename R, typename... Arg>
synapse::thread::work<R(Arg...)>::work(std::function<R(Arg...)> work_func, Arg... work_arg)
	: work_function(work_func),
	  work_argument(std::make_tuple(work_arg...)) { }

template <typename R, typename... Arg>
void synapse::thread::work<R(Arg...)>::run()
{
	work_return_value = std::apply(work_function, work_argument);
	work_notifier	   .set();
}