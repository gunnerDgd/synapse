#include <iostream>
#include <synapse/format/string.hpp>

namespace http
{
    class request
    {
        public:
        request(std::string _method,
                std::string _path,
                std::string _httpversion = "HTTP/1.1") : r_msg_line( _method + " " + _path + " " + _httpversion ),
                                                         r_method  (_method),
                                                         r_path    (_path),
                                                         r_version (_httpversion)
                { }

        request(const char* _raw_msg);

        public:
            std::string     r_msg_line,
                            r_method,
                            r_path,
                            r_version;
    };

    class response
    {
        public:
            enum code
            {
                ok              = 200,
                create          = 201,
                permanent_moved = 301,
                found           = 302,

                bad_request     = 400,
                forbidden       = 403,
                not_found       = 404,
                server_error    = 500
            };

        public:
            response(uint16_t    _code,
                     std::string _version,
                     std::string _message) : r_code    (_code), 
                                             r_version (_version), 
                                             r_msg     (_message), 
                                             r_msg_line(r_version + " " + std::to_string(_code) + " " + _message)
                     {}
            
            response(const char* _raw_msg);

        private:
            uint16_t r_code;
            std::string r_version,
                        r_msg,
                        r_msg_line;
    };

    class header
    {
        public:
            header(std::string _req,
                   std::string _ctx) : h_request(std:move(_req)), h_context(_ctx), h_msg_line(_req + ": " + _ctx)
                   {}

        private:
            std::string h_request,
                        h_context,
                        h_msg_line;
    };

    class message
    {
        public:
            message (const char* _msg);
            ~message() { delete msg_request; }

            request                      *msg_request;
            std::map<std::string, header> msg_header;
    };
}