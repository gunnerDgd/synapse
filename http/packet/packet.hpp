#include "header.hpp"

namespace http
{
    using header_map = std::map<std::string, header>;
    class packet
    {
        public:
            ~packet() { if(p_raw != nullptr) delete[] p_raw; }

            void       add_header   (header&     h_add);
            void       delete_header(std::string h_delete);

            header&    operator[]   (std::string h_find);

        protected:
            header_map  p_header;
            char*       p_context,
                *       p_raw     = nullptr;
    };

    void packet::add_header   (header&     h_add)
    {
        p_header.insert(std::make_pair(h_add.h_prefix, std::move(h_add)));
    }

    void packet::delete_header(std::string h_delete)
    {
        p_header.erase(h_delete);
    }

    header&    packet::operator[]   (std::string h_find)
    {
        auto hf = p_header.find(h_find);
        if(hf  != p_header.end()) return *hf;
        else                      return header();
    }
}