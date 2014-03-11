//
//  CmusStatus.cpp
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-5.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#include "CmusStatus.h"
#include <stdio.h>
#include <regex>

CmusStatus::CmusStatus(std::string path) {
	cmusRemotePath = path;
    refreshStatus();
}

bool CmusStatus::refreshStatus() {
	FILE* pipe = popen((cmusRemotePath + " -Q 2>/dev/null").c_str(), "r");
    if (!pipe){
	 return false;
	 }
    char buffer[32];
	char statusBuf[1024];
	int statusBufIndex = 0;
    while(!feof(pipe)) {
    	if(fgets(buffer, 32, pipe) != NULL && statusBufIndex + strlen(buffer) < 1023) {
    		strcpy(statusBuf+statusBufIndex, buffer);
			statusBufIndex+=strlen(buffer);
		}
    }
	statusString = statusBuf;
	pclose(pipe);
	
	if (statusString.length() == 0)
		return false;
	
	return true;
}

std::string CmusStatus::getItem(std::string item) {
	std::regex exp(item + " (.+)(\\n|$)");
	std::smatch match;
	if (std::regex_search(statusString, match, exp))
		return match[1];
	else
		return "";
}

std::string CmusStatus::getStatusString() {
	return statusString;
}