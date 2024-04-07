#ifndef __DIR__STRUCT_HPP__
#define __DIR__STRUCT_HPP__

#include <string>

class DirStruct
{
private:
	std::string	_name;
	std::string	_data;
	std::string	_size;
public:
	DirStruct	();
	DirStruct	(const std::string & name, const std::string& data, const std::string& size);
	DirStruct	(const DirStruct & other);
	DirStruct & operator=(const DirStruct & other);

	std::string	getName();
	std::string& getData();
	std::string	getSize();

	void		setName(const std::string& name);
	void		setData(const std::string& data);
	void		setSize(const std::string& size);

	~DirStruct();
};


#endif