#pragma once

/*
 * this file is a part of Neo
 * @author: linjianpeng
 * @date:	2019-5-31
 */

#include "Common/Noncopyable.h"
#include "Global.h"

#include <functional>

//Qt's Way
//auto cleanup = qScopeGuard([] { code you want executed goes HERE; });

namespace neo {
	
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

#define ON_SCOPE_EXIT(callback) neo::ScopeGuard NEO_NAME_LINE_CAT(scopeGuard, __LINE__)(callback);
}

