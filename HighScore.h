#pragma once
#include <fstream>

class HighScore
{
	struct Score
	{
		int			mScore;
		std::string mName;
	};

public:
	HighScore(void);
	~HighScore(void);

	std::vector<Score> SaveScore(int score,std::string name);

private:

	std::vector<Score>	mHighScores;
	std::fstream		mFileStream;
	int					mMaxScores;
};

