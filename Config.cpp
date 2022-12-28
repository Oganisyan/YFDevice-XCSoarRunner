#include "StdAfx.h"
#include "Config.h"
#include <sstream>
#include <fstream>


std::wstring itows(int  val) {
	std::wostringstream stream;
	stream << val;
	return stream.str();
}



Config::Config(std::wofstream &log) : std::map<std::wstring,std::wstring>(), log_(log)
{
	initConfig();
}

Config::~Config() 
{}

void Config::initConfig() {
	wchar_t buffer[MAX_PATH+1];
	memset(buffer, 0, sizeof(buffer));
	GetModuleFileName(NULL, buffer, MAX_PATH);
	wchar_t *replace = wcsstr(buffer, _T(".exe"));
	if(replace != NULL) {
		wcscpy(replace, _T(".ini"));
		log_<< _T("FileName: ") << buffer << std::endl;
		std::wifstream iniFile(buffer);
		std::wstring line;
		if(iniFile.good()) {
			while( std::getline(iniFile, line) )
			{
				log_<< _T("Line: ") << line << std::endl;
				std::wistringstream is_line(line, std::ios_base::in);
				std::wstring key;
				if(std::getline(is_line, key, _T('=')))
				{
					std::wstring value;
					if( std::getline(is_line, value) ) {
						log_<< _T("Set key: ") << key << _T(" value: ") << value << std::endl;
						(*this)[key] = value;
					}
			  }
			}
		}
	} 
}



const std::wstring& Config::get(const std::wstring &key, const std::wstring &defValue) const {
	log_<< _T("Get key: ") << key << _T(" default value: ") << defValue << std::endl;
	const_iterator it = find(key);
	return (it == end()) ? defValue : it->second;
}

const std::wstring& Config::get(const std::wstring &key, int pos, const std::wstring defValues[]) const {
	std::wostringstream keyNewStream;
	keyNewStream << key << _T("_") << pos;
	return get(keyNewStream.str(), defValues[pos]); 
}
