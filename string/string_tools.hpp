#pragma once
#include <synapse/synapse.hpp>
#include <iostream>

#include <string_view>
#include <string>
#include <vector>

namespace synapse {
namespace string  {
	
	using str_vec = std::vector<std::string>	 ;
	using sv_vec  = std::vector<std::string_view>;
	using off_vec = std::vector<size_t>		     ;

	void   split	  (str_vec& res, std::string     & context, std::string token);
	void   split	  (sv_vec & res, std::string_view& context, std::string token);
	
	void   trim 	  (std::string& 	 context, std::string token);
	
	void   find_all   (off_vec& res, std::string	 & context, std::string token);
	void   find_all   (off_vec& res, std::string_view& context, std::string token);

	size_t token_count(std::string	   & context, std::string token);
	size_t token_count(std::string_view& context, std::string token);
}
}

size_t synapse::string::token_count(std::string	    & context, std::string token)
{
	size_t cnt_res = 0,
		   cnt_off = 0;
	size_t tk_len  = token.length();

	while((cnt_off = context.find(token, cnt_off)) != std::string::npos)
	{
		cnt_off += tk_len;
		cnt_res 	   ++;
	}

	return cnt_res;
}

size_t synapse::string::token_count(std::string_view& context, std::string token)
{
	size_t cnt_res = 0, 
		   cnt_off = 0;

	size_t tk_len  = token.length();

	while((cnt_off = context.find(token, cnt_off)) != std::string::npos)
	{
		cnt_off += tk_len;
		cnt_res 	   ++;
	}

	return cnt_res;
}

void synapse::string::split(synapse::string::str_vec& res, std::string& context, std::string token)
{
	size_t ptr_split = 0,
		   ptr_seek  = 0;

	size_t ctx_length = context.length(),
		   tk_length  = token  .length();

	for(size_t i = 0 ; (ptr_split = context.find(token, ptr_seek)) != std::string::npos ; i++)
	{
		synapse::debug_tool::runtime_timer dbg_timer;
		res		  .push_back(context.substr(ptr_seek, ptr_split - ptr_seek));
		ptr_seek = ptr_split + tk_length;
	}
	
	res.push_back	 (context.substr(ptr_seek, ctx_length - ptr_seek));
}

void synapse::string::split(synapse::string::sv_vec& res, std::string_view& context, std::string token)
{
	res				 .reserve(sizeof(std::string_view) * 16);
	size_t ptr_split = 0,
		   ptr_seek  = 0;

	size_t ctx_length = context.length(),
		   tk_length  = token  .length();

	for( ; (ptr_split = context.find(token, ptr_seek)) != std::string::npos ; )
	{
		res.push_back(context.substr(ptr_seek, ptr_split - ptr_seek));
		ptr_seek    = ptr_split + tk_length;
	}
	
	res.push_back	 (context.substr(ptr_seek, ctx_length - ptr_seek));
}

void						  synapse::string::trim 	  (std::string& context, std::string token)
{
	size_t ptr_seek = 0;
	while((ptr_seek = context.find(token, ptr_seek)) != std::string::npos)
		context.erase(ptr_seek, token.length());
}

void synapse::string::find_all(synapse::string::off_vec& res, std::string& context, std::string token)
{
	size_t ptr_seek = 0,
		   tk_len   = token.length();

	while((ptr_seek = context.find(token, ptr_seek)) != std::string::npos)
	{
		res	   	   .push_back(ptr_seek);
		ptr_seek += tk_len;
	}
}

void synapse::string::find_all(synapse::string::off_vec& res, std::string_view& context, std::string token)
{
	size_t 				ptr_seek = 0;
	while((ptr_seek = context.find(token, ptr_seek)) != std::string::npos)
	{
		res	   	   .push_back(ptr_seek);
		ptr_seek += token.length();
	}
}