/*
 *  ZNCNode.h
 *  ZNCNode
 *
 *  Created by Steve Streza on 11/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ZNCNode_
#define ZNCNode_

#include "Modules.h"

/* The classes below are exported */
#pragma GCC visibility push(default)

class ZNCNode : public CModule
{
	public:
		void HelloWorld(const char *);
};

#pragma GCC visibility pop
#endif
