#include <synapse/context/switch.hpp>
#include <map>

namespace frame
{
    class enumerator
    {
    public:
        enumerator();

        template <typename T, typename... Args>
        void start    (T&& en_fp, Args&&... en_args);

        template <typename T>
        void start    (T&& en_fp);

        template <typename T>
        void end      (T&& en_fp);

        template <typename T>
        void switch_to(T&& en_fp);
        
    private:
        std::map<uint64_t, frame*> en_queue               ;
        frame*                     en_current    = nullptr;
    };
}

frame::enumerator::enumerator()
{
    frame* main_fr = new frame;
    en_current     = main_fr;

    en_queue.insert (std::make_pair(0, main_fr));
}

template <typename T, typename... Args>
void frame::enumerator::start(T&& en_fp, Args&&... en_args)
{
    frame* en_fr   = new frame(64 * 1024),
         * en_prev = en_current;
    en_current     = en_fr;
    
    en_queue.insert (std::make_pair((uint64_t)en_fp, en_fr));
    switch_frame    (*en_prev,
                     *en_current,
                      en_fp, *this, en_args...);
}

template <typename T>
void frame::enumerator::start(T&& en_fp)
{
    frame* en_fr   = new frame(64 * 1024),
         * en_prev = en_current;
    en_current     = en_fr;

    en_queue.insert (std::make_pair((uint64_t)en_fp, en_fr));
    switch_frame    (*en_prev, *en_current, en_fp, *this);
}

template <typename T>
void frame::enumerator::end  (T&& en_fp) { en_queue.erase(en_fp); }

template <typename T>
void frame::enumerator::switch_to(T&& en_fp)
{
    auto ce_it  = en_queue.find((uint64_t)en_fp);
    if  (ce_it == en_queue.end()) return;

    frame* en_prev = en_current;
    en_current     = ce_it->second;

    switch_frame(*en_prev, *en_current);
}