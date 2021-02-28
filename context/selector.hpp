#include <synapse/context/switch.hpp>
#include <list>

namespace frame
{
    class selector
    {
    public:
        selector  ();

        template <typename T, typename... Args>
        void add  (T(&&sel_fp)(selector&, Args...), Args&&... sel_arg);

        template <typename T>
        void add  (T(&&sel_fp)(selector));

        template <typename T>
        void erase(T sel_fp);

        void run  () { sel_stop_flag = false; execute_loop(); }
        void stop () { sel_stop_flag = true ; }
        void next ();

    private:
        void                     execute_loop ();
        bool                     sel_stop_flag;

        std::list<frame::frame*> sel_queue  ;
        frame::frame*            sel_loop_fr, *sel_current;
    };
}

void frame::selector::execute_loop()
{
    while(true)
    {
        for(auto& sel_fr : sel_queue) 
        {
            fr_prev     = sel_current;
            sel_current = *sel_fr;

            switch_frame     (*sel_loop_fr, *sel_current);
            if(sel_stop_flag) break;
        }
    }
}