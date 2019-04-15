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

unsigned int hex2int(std::string hex)
{
    std::istringstream converter(hex);
    unsigned int value;
    converter >> std::hex >> value;
    return value;
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
    int white=0;
    int black=0;
    auto norm=visit("data/normal");
    for(auto i:norm)
    {
        if(!query("one.filter",i))
        {
            white++;
        }
        else if(!query("two.filter",i))
        {
            black++;
        }
        else if(!query("three.filter",i))
        {
            white++;
        }
        else black++;
    }
    std::cout<<"white = "<<white<<std::endl;
    std::cout<<"black = "<<black<<std::endl;

    return 0;
}


