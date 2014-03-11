//
//  main.cpp
//  CmusLyricsDaemon
//
//  Created by Yuwei Huang on 14-3-8.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

#include "CmusStatus.h"
#include "Lyrics.h"

#define SLEEP_PERIOD 3 // second
#define BUFFER_SIZE 512

#define LINE_BUF_SIZE 256
#define FILE_BUF_SIZE 16384

// #define _DEBUG 1

std::string get_file_contents(const char *filename, bool* error);
void writeToPipe(const char* pipePath, std::string thing);

int main(int argc, const char * argv[])
{

#ifndef _DEBUG
	if (argc != 6) {
		std::cout << "USAGE: CmusLyricsDaemon <period (msec)> <number of lines before or after current line> <lrc directory> <cmus-remote path> <pipe path>" << std::endl;
		return 0;
	}
	
	int period = atoi(argv[1]);
	int extraLines = atoi(argv[2]);
	const char* lrcdir = argv[3];
	const char* remotePath = argv[4];
	const char* pipePath = argv[5];
#else
	int period = 700;
	int extraLines = 3;
	const char* remotePath = "/usr/local/bin/cmus-remote";
	const char* lrcdir = "/Volumes/Data/Music/lyrics";
	const char* pipePath = "/tmp/cmuslyricspipe";
#endif
	
	std::string currentFile = "";
	mkfifo(pipePath, 0666);
	
	CmusStatus* status = new CmusStatus(remotePath);
	Lyrics* lrc = new Lyrics();
	bool lrcLoaded = false;
	
	while (1) {
		status->refreshStatus();
		if (status->getItem("status").compare("playing") != 0) { // not playing
			sleep(SLEEP_PERIOD);
			continue;
		}
		
		std::string fullname = status->getItem("file");
		std::string basename = fullname.substr(fullname.find_last_of('/'));
		int dotIndex = (int)basename.find_last_of('.');
		if (dotIndex > 0)
			basename = basename.substr(0, dotIndex) + ".lrc";
		else
			basename += ".lrc";
		
		std::string newFile = lrcdir + basename;
		
		if (newFile.compare(currentFile) != 0) { // different file
			bool err = false;
			std::string lrcCode = get_file_contents(newFile.c_str(), &err);
			if (err) { // file not found
				lrcLoaded = false;
				currentFile = newFile;
				writeToPipe(pipePath, ""); // writes an empty string
				continue;
			}
			lrc->reset(lrcCode, true);
			currentFile = newFile;
			lrcLoaded = true;
		} else if (!lrcLoaded) { // current = new, lrc not loaded
			sleep(SLEEP_PERIOD);
			continue;
		}
		
		std::string lrcString = "";
		
		int position = atoi(status->getItem("position").c_str());
		int index = lrc->getIndexOfLyrics(position * 1000);
		
		for (int i =extraLines; i > 0; i--) {
			lrcString += "  "+lrc->getLyricsAtIndex(index-i) + "\n";
		}
		
		if (extraLines > 0)
			lrcString+="> ";
		
		lrcString += lrc->getLyricsAtIndex(index) + "\n";
		
		for (int i = 0; i < extraLines; i++) {
			lrcString += "  "+lrc->getLyricsAtIndex(index + i + 1) + "\n";
		}
		
		writeToPipe(pipePath, lrcString);
		usleep(period);
	}
	
	

	
    return 0;
}

void writeToPipe(const char* pipePath, std::string thing) {
	FILE* fp = fopen(pipePath, "w");
	fprintf(fp, "%s", thing.c_str());
	fclose(fp);
}


std::string get_file_contents(const char *filename, bool* error)
{
  FILE* fp = fopen(filename, "r");
    if (!fp){
		*error = true;
		return "";
	 }
    char lineBuf[LINE_BUF_SIZE];
	char fileBuf[FILE_BUF_SIZE];
	int fileBufIndex = 0;
    while(!feof(fp)) {
    	if(fgets(lineBuf, LINE_BUF_SIZE, fp) != NULL && fileBufIndex + strlen(lineBuf) -1 < FILE_BUF_SIZE) {
    		strcpy(fileBuf+fileBufIndex, lineBuf);
			fileBufIndex+=strlen(lineBuf);
		}
    }
	fclose(fp);
	
	return fileBuf;
}


