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

class ZNCNode : public CGlobalModule
{
public:
	GLOBALMODCONSTRUCTOR(ZNCNode) {
//		openlog("znc", LOG_PID, LOG_DAEMON);
	}
	
	void HelloWorld(const char *);
};

GLOBALMODULEDEFS(ZNCNode, "Run node.js scripts within ZNC")

#pragma GCC visibility pop
#endif
