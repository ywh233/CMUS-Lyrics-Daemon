//
//  Lyrics.h
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-5.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#ifndef __CMUS_Lyrics_Plugin__Lyrics__
#define __CMUS_Lyrics_Plugin__Lyrics__

#include <iostream>
#include <vector>

class Lyrics {

public:
	Lyrics();
	Lyrics(std::string lyrics);
	Lyrics(std::string lyrics, bool linearize);
	// linearize: makes lyricsMap sorted in the order of time. Strings may be duplicated.
	~Lyrics();
	
	std::string getTitle();
	std::string getArtist();
	std::string getAlbum();
	int getOffset(); // millisecond
	
	std::string getLyrics(int millisecond); // empty string if not found. same below
	std::string getLyricsAtIndex(int index);
	int getIndexOfLyrics(int millisecond); // -1 means not found
	void reset(std::string lyrics, bool linearlize); // change lyrics
	void reset(std::string lyrics);
	
private:
	static const int INIT_TIME_MAP_SIZE = 60000; // 10 mins
	//static const int TIME_MAP_SIZE = 600000; //100*60*100
	static const int LRC_MAP_SIZE = 512;
	
	int timeMapSize;
	short* timeMap; // index: centiTime; value: index to lyrics line. (-1 means no lyrics)
	std::vector<std::string> lyricsMap; // lyrics
	
	std::string title;
	std::string artist;
	std::string album;
	int offset; // millisecond
	
	int lastTmIndex; // the last index of timeMap that stores lyrics index
	
	void constructArrayFromLrc(std::string lyrics, bool linearize);
	void setMetaInfo(std::string field, std::string data);
	
	void initArray();
};

#endif /* defined(__CMUS_Lyrics_Plugin__Lyrics__) */
