#pragma once
#include <fstream>

class HighScore
{


public:
	HighScore(void);
	~HighScore(void);

	std::vector<long double> SaveScore(int score);
	int							mMaxScores;

private:

	std::vector<long double>	mHighScores;
	std::fstream				mFileStream;
	
};

