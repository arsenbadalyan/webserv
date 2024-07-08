#ifndef __DIR__STRUCT_HPP__
#define __DIR__STRUCT_HPP__

#include <string>

class DirController
{
private:
	std::string	_name;
	std::string _href;
	std::string	_data;
	std::string	_size;
public:
	DirController	();
	DirController	(const std::string & name, const std::string & href, const std::string& data, const std::string& size);
	DirController	(const DirController & other);
	DirController & operator=(const DirController & other);

	std::string	getName();
	std::string	getHref();
	std::string& getData();
	std::string	getSize();

	void		setName(const std::string& name);
	void		setHref(const std::string& href);
	void		setData(const std::string& data);
	void		setSize(const std::string& size);

	~DirController();
};


#endif