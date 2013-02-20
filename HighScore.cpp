#include "stdafx.h"
#include "HighScore.h"
#include <algorithm>


HighScore::HighScore(void)
{
	
}

bool mostFirst (int i,int j) { return (i>j); }

std::vector<long double> HighScore::SaveScore(int score)
{
	mMaxScores = 10;
	mHighScores.resize(mMaxScores);

	//inte null
	for(int i =0;i< mHighScores.size();i++)
	{
		mHighScores[i] = 0;
	}


	//läs in befintliga
	mFileStream.open("Content/HighScores.txt");
	for(int i =0;i< mMaxScores;i++)
	{
		mHighScores.push_back(i);

		mFileStream >> mHighScores[i];
	}
	mFileStream.close();

	//kolla om score är högre än nåt i filen
	bool insertNew=false;
	for(int i =0;i< mMaxScores;i++)
	{
		if(score > mHighScores[i])
		{
			insertNew = true;
			break;
		}
	}

	//skriv in nytt
	if(insertNew)
	{
		sort(mHighScores.begin(),mHighScores.end());

		mHighScores[0] = score;

		sort(mHighScores.begin(),mHighScores.end(),mostFirst);

		std::ofstream fout("Content/HighScores.txt",std::ios_base::trunc);

		fout.trunc;

		for(int i=0;i< mMaxScores; i++)
		{
			fout << mHighScores[i] << "\n";
		}

		fout.close();
	}




	return mHighScores;
}


HighScore::~HighScore(void)
{
}
