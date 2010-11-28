/*
 *  ZNCNode.cp
 *  ZNCNode
 *
 *  Created by Steve Streza on 11/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include "node.h"
#include "ZNCNode.h"
#include "ZNCNodePriv.h"

void ZNCNode::HelloWorld(const char * s)
{
	node::Start(0, 0);
	
	ZNCNodePriv *theObj = new ZNCNodePriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void ZNCNodePriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

