#pragma once

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "Common/Noncopyable.h"

#include <functional>

//Qt's Way
//auto cleanup = qScopeGuard([] { code you want executed goes HERE; });

namespace Neo {
	
class ScopeGuard: public Noncopyable
{
public:
	ScopeGuard(std::function<void()> &&resourceDeleter) : functor_ (resourceDeleter) { }
	~ScopeGuard() {
		if(!dismiss_) {
			functor_();
		}
	}
	void dismiss() {
		dismiss_ = true;
	}
private:
	std::function<void()> functor_;
	bool dismiss_ = false;
};

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)
#define ON_SCOPE_EXIT(callback) Neo::ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)
}

