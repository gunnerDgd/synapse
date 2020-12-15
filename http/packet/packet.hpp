#include "header.hpp"

namespace http
{
    using header_vec = std::vector<header>;
    class packet
    {
        public:
            ~packet() { if(p_raw != nullptr) delete[] p_raw; }
            void      write_header   (header&     h_add);

        protected:
            header_vec  p_header;
            char*       p_context,
                *       p_raw     = nullptr;
    };

    void packet::write_header (header&     h_add) { p_header.push_back(h_add); }
}