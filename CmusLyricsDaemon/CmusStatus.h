//
//  CmusStatus.h
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-5.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#ifndef __CMUS_Lyrics_Plugin__CmusStatus__
#define __CMUS_Lyrics_Plugin__CmusStatus__

#include <iostream>

class CmusStatus {

public:
	CmusStatus(std::string cmusRemotePath); // cmusRemotePath: the full path of cmus-remote
	bool refreshStatus(); // returns whether the status is successfully refreshed
	
	std::string getStatusString();
	std::string getItem(std::string item); // returns empty string if not found
	
	std::string cmusRemotePath;

private:
	std::string statusString;
};

#endif /* defined(__CMUS_Lyrics_Plugin__CmusStatus__) */
