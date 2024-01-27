#ifndef _ACTIVEMETODS_HPP_
#define _ACTIVEMETODS_HPP_

#include <iostream>

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
	const bool GetPost() const;
	const bool GetGet() const;
	const bool GetDelete() const;
	ActiveMetods& SetPost(const bool post);
	ActiveMetods& SetGet(const bool get);
	ActiveMetods& SetDelete(const bool DeleteData);
};

#endif