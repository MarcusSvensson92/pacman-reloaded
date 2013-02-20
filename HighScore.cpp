#include "stdafx.h"
#include "HighScore.h"


HighScore::HighScore(void)
{
	mMaxScores = 10;
}

std::vector<HighScore::Score> HighScore::SaveScore(int score,std::string name)
{
	mHighScores.resize(10);

	for (int i =0; i < mHighScores.size();i++)
	{
		mHighScores[i].mScore = 0;
		mHighScores[i].mName = "Empty";
	}

	std::ofstream fout("Content/scores.txt", std::ios::app); 

	std::string line;

	fout << score << " "<< name << "\n";

	fout.close();

	mFileStream.open("Content/HighScores.txt", std::ios::out | std::ios::in);

	int count=0;

	while(!mFileStream.eof() && count < mHighScores.size())
	{
		Score temp;
		mHighScores.push_back(temp);
		mFileStream >> mHighScores[count].mScore >> mHighScores[count].mName;
		count++;
	}



	mFileStream.close();

	return mHighScores;
}


HighScore::~HighScore(void)
{
}
