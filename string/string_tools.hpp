#pragma once

#include <iostream>
#include <vector>

#include <cstring>
#include <string>
#include <string_view>

namespace string
{
	std::vector<std::string> 	  split	  (std::string& 	  context, std::string token);
	std::vector<std::string_view> split	  (std::string_view&  context, std::string token);
	
	void						  trim 	  (std::string& 	 context, std::string token);
	
	std::vector<size_t>			  find_all(std::string& 	 context, std::string token);
	std::vector<size_t>			  find_all(std::string_view& context, std::string token);
}

std::vector<std::string> 	  string::split	  (std::string& context, std::string token)
{
	size_t ptr_split = 0,
		   ptr_seek  = 0;
	
	std::vector<std::string> res;
	for( ; (ptr_split = context.find(token, ptr_seek)) != std::string::npos ; )
	{
		res.push_back(context.substr(ptr_seek, ptr_split - ptr_seek));
		ptr_seek    = ptr_split + token.length();
	}
	
	res.push_back	 (context.substr(ptr_seek, context.length() - ptr_seek));
	return			 res;
}

std::vector<std::string_view> string::split	  (std::string_view& context, std::string token)
{
	size_t ptr_split = 0,
		   ptr_seek  = 0;

	std::vector<std::string_view> res;
	for( ; (ptr_split = context.find(token, ptr_seek)) != std::string::npos ; )
	{
		res.push_back(context.substr(ptr_seek, ptr_split - ptr_seek));
		ptr_seek    = ptr_split + token.length();
	}
	
	res.push_back	 (context.substr(ptr_seek, context.length() - ptr_seek));
	return			 res;
}

void						  string::trim 	  (std::string& context, std::string token)
{
	size_t ptr_seek = 0;
	while((ptr_seek = context.find(token, ptr_seek)) != std::string::npos)
		context.erase(ptr_seek, token.length());
}

std::vector<size_t>			  string::find_all(std::string& context, std::string token)
{
	size_t 				ptr_seek = 0;
	std::vector<size_t> ptr_res;
	
	while((ptr_seek = context.find(token, ptr_seek)) != std::string::npos)
	{
		ptr_res	   .push_back(ptr_seek);
		ptr_seek += token.length();
	}
	
	return ptr_res;
}

std::vector<size_t>			  string::find_all(std::string_view& context, std::string token)
{
	size_t 				ptr_seek = 0;
	std::vector<size_t> ptr_res;
	
	while((ptr_seek = context.find(token, ptr_seek)) != std::string::npos)
	{
		ptr_res	   .push_back(ptr_seek);
		ptr_seek += token.length();
	}
	
	return ptr_res;
}