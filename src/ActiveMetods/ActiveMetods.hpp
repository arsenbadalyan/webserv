#ifndef _ACTIVEMETODS_HPP_
#define _ACTIVEMETODS_HPP_

#include <iostream>
#include "RootConfigs.hpp"
#include "Util.hpp"

struct SupportedMethods {
	static const std::string POST;
	static const std::string GET;
	static const std::string DELETE;
};

class ActiveMetods
{
private:
	bool _post;
	bool _get;
	bool _delete;
public:
	ActiveMetods();
	ActiveMetods(bool post, bool get, bool del);
	ActiveMetods(const ActiveMetods& other);
	ActiveMetods& operator= (const ActiveMetods& other);
	~ActiveMetods();
	bool getPost();
	bool getGet();
	bool getDelete();
	bool isAvailableMethod(std::string methodName);
	ActiveMetods& setPost(const bool post);
	ActiveMetods& setGet(const bool get);
	ActiveMetods& setDelete(const bool DeleteData);
};

#endif