#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/algorithm/string.hpp>
#include <cstring>
//#include <filesystem>
#include <boost/filesystem.hpp>
#include <iomanip>
#include "common.h"

bool query(const std::string& filter,std::string pem);

unsigned int hex2int(std::string hex)
{
    std::istringstream converter(hex);
    unsigned int value;
    converter >> std::hex >> value;
    return value;
}



void insertwhite(const std::string& pem)
{
    auto checksum=md5(pem);
    std::string part1=checksum.substr(0,6);
    std::string part2=checksum.substr(6,6);
    auto offset1=hex2int(part1);
    auto offset2=hex2int(part2);

    // check redis

    // filter 1
    if(!query("one.filter",pem))
    {
        return;
    }

    // filter 2
    else if(!query("two.filter",pem))
    {
        set("two.filter",offset1);
        set("two.filter",offset2);
        return;
    }
    
    // filter 3
    return;
    
}



void insertblack(const std::string& pem)
{
    auto checksum=md5(pem);
    std::string part1=checksum.substr(0,6);
    std::string part2=checksum.substr(6,6);
    auto offset1=hex2int(part1);
    auto offset2=hex2int(part2);

    // check redis

    // filter 1
    if(!query("one.filter",pem))
    {
        set("one.filter",offset1);
        set("one.filter",offset2);
        return;
    }

    // filter 2
    else if(!query("two.filter",pem))
    {
        return;
    }
    
    // filter 3
    else
    {
        set("three.filter",offset1);
        set("three.filter",offset2);
        return;
    }
    
}




bool query(const std::string& filter,std::string pem)
{
    const char* filterpath=filter.c_str();

    auto checksum=md5(pem);
    std::string part1=checksum.substr(0,6);
    std::string part2=checksum.substr(6,6);
    auto offset1=hex2int(part1);
    auto offset2=hex2int(part2);

    if(get(filterpath,offset1) && get(filterpath,offset1))return true;
    return false;
}

std::string file2str(std::string path)
{
	std::fstream s(path, std::fstream::binary | std::fstream::in);
	std::stringstream ss;
	ss << s.rdbuf();
	s.close();
	return	ss.str();
}

 
std::vector<std::string> visit(const std::string& dir)
{
    std::vector<std::string> filenames;
	boost::filesystem::path path(dir);
	if (!boost::filesystem::exists(path))
	{
		return {};
	}
 
	boost::filesystem::directory_iterator end_iter;
	for (boost::filesystem::directory_iterator iter(path); iter!=end_iter; ++iter)
	{
		if (boost::filesystem::is_regular_file(iter->status()))
		{
			filenames.push_back(iter->path().string());
		}
 
		if (boost::filesystem::is_directory(iter->status()))
		{
			// visit(iter->path().string(), filenames);
		}
	}
 
	return filenames;
}




int main()
{
    auto revo=visit("data/revoke");
    for(auto i:revo)
    {
        insertblack(i);
    } 
    auto norm=visit("data/normal");
    for(auto i:revo)
    {
        insertwhite(i);
    } 
	return 0;
}

std::string decode(std::string input)
{
	using namespace boost::archive::iterators;
	typedef transform_width<binary_from_base64<remove_whitespace
		<std::string::const_iterator> >, 8, 6> ItBinaryT;

	try
	{
		// If the input isn't a multiple of 4, pad with =
		size_t num_pad_chars((4 - input.size() % 4) % 4);
		input.append(num_pad_chars, '=');

		size_t pad_chars(std::count(input.begin(), input.end(), '='));
		std::replace(input.begin(), input.end(), '=', 'A');
		std::string output(ItBinaryT(input.begin()), ItBinaryT(input.end()));
		output.erase(output.end() - pad_chars, output.end());
		return output;
	}
	catch (std::exception const&)
	{
		return std::string("");
	}
}

std::string decode64(const std::string &val) {
	using namespace boost::archive::iterators;
	using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
	return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
		return c == '\0';
	});
}

std::string encode64(const std::string &val) {
	using namespace boost::archive::iterators;
	using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
	auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
	return tmp.append((3 - val.size() % 3) % 3, '=');
}