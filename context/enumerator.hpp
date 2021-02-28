#include <synapse/context/switch.hpp>
#include <map>

namespace frame
{
    class enumerator
    {
    public:
        enumerator ();
        ~enumerator();

        template <typename T, typename... Args>
        void start    (T&& en_fp, Args&&... en_args);

        template <typename T>
        void start    (T&& en_fp)   ;
        void start    (frame& en_fr);

        template <typename T>
        void end      (T&& en_fp);

        template <typename T>
        void switch_to(T&& en_fp);
        void switch_to()         { switch_to(0); }

        template <typename T>
        friend void migrate  (T fp, enumerator& e_curr, enumerator& e_next);
        
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

frame::enumerator::~enumerator()
{
    for(auto& fr_it : en_queue)
        delete fr_it.second;
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

void frame::enumerator::start    (frame& en_fr)
{
    frame* en_prev = en_current;
    
}

template <typename T>
void frame::enumerator::end  (T&& en_fp) 
{
    auto fp_it  = en_queue.find((uint64_t)en_fp);
    if  (fp_it == en_queue.end()) return;
    
    delete         fp_it->second;
    en_queue.erase(fp_it);
}

template <typename T>
void frame::enumerator::switch_to(T&& en_fp)
{
    auto ce_it  = en_queue.find((uint64_t)en_fp);
    if  (ce_it == en_queue.end()) return;

    frame* en_prev = en_current;
    en_current     = ce_it->second;

    switch_frame(*en_prev, *en_current);
}

template <typename T>
void frame::enumerator::migrate  (T fp, enumerator& e_curr, enumerator& e_next)
{
    auto mig_it  = e_curr.en_queue.find((uint64_t)fp);
    if  (mig_it == e_curr.en_queue.end()) return;

    frame* mig_fr = mig_it->second;
    e_curr.en_queue.erase (mig_it);
    
    e_next.en_queue.insert(std::make_pair((uint64_t)fp, mig_fr));
}