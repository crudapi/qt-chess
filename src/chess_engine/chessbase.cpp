#include "chessbase.h"
#include <cassert>
using namespace std;

std::vector<Matrix> ChessBase::legalposition_;

ChessBase::ChessBase(int sidetag, int _id, int value, const Coordinate& newlocation)
  :m_sidetag(sidetag)
  ,m_id(_id)
  ,m_value(value)
  ,m_othersidetag(sidetag == 0 ? 1 : 0)
  ,location_(newlocation)
  ,alive_(true) 
{
	chessmanvalue_ =
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//0-15
		30,20,20,20,20,40,40,90,90,45,45,10,10,10,10,10,//16-31
		30,20,20,20,20,40,40,90,90,45,45,10,10,10,10,10,//32-48
	};
}
ChessBase::~ChessBase(void)
{
}

void ChessBase::GetMove(const Matrix& _chessboard,moves& temp) const
{
}

//int ChessBase::GetCaptureValue(const Matrix& _chessboard)const
//{
//	return 0;
//}
IdType ChessBase::GetSelfKing(void) const
{
	return	m_sidetag == 1 ? g_global.bigKingId : g_global.smallKingId;
}
// 供派生类构造函数使用，实现位置价值数组的初始化
void ChessBase::InitMatrix(int* _positionvalueFromArray)
{
	//positionvalue_.reserve(256);
	//假设输入的价值数组所对应的棋子在棋盘的下方(开局时)
	vector<int> tempLine;

	if(m_sidetag==0)//小号棋子在棋盘下方
		for(int i=0;i<16;++i)
		{
			tempLine.assign(_positionvalueFromArray + i * 16, _positionvalueFromArray + (i + 1) * 16);															//	将区间[first, last)的元素赋值到当前的vector容器中，或者赋n个值为x的元素到vector容器中，这个容器会清除掉vector容器中以前的内容。
			positionvalue_.push_back(tempLine);
			tempLine.clear();																					//	清空临时容器
		}
	else if(m_sidetag==1)//大号棋子在棋盘上方
		for(int i=15;i>=0;--i)
		{
			tempLine.assign(_positionvalueFromArray + i * 16, _positionvalueFromArray + (i + 1) * 16);
			positionvalue_.push_back(tempLine);
			tempLine.clear();
		}
    else
    {
        assert(false);
    }
}

void ChessBase::InitLegalPosition(void)
{
	//初始化合法位置数组
	int legalPosition[256]=
	{
		//默认为大号棋子的合法位置
	//	0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //1
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //2
		0, 0, 0, 1, 1, 17,7, 3, 7, 17,1, 1, 0, 0, 0, 0,  //3
		0, 0, 0, 1, 1, 1, 3, 7, 3, 1, 1, 1, 0, 0, 0, 0,  //4
		0, 0, 0, 17,1, 1, 7, 19,7, 1, 1, 17,0, 0, 0, 0,   //5
		0, 0, 0, 9, 1, 9, 1, 9, 1, 9, 1, 9, 0, 0, 0, 0,   //6
		0, 0, 0, 9, 1, 25,1, 9, 1, 25,1, 9, 0, 0, 0, 0,  //7
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,  //8
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,  //9
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,  //10
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,  //11
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,  //12
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //13
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //14
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //15
	};
	vector<int> tempLine;
	Matrix redlegalposition,blacklegalposition;
	//先放小号棋子legalposition[0]
	for(int i=15;i>=0;--i)
	{
		tempLine.assign(legalPosition+i*16,legalPosition+(i+1)*16);
		blacklegalposition.push_back(tempLine);
		tempLine.clear();
	}
	legalposition_.push_back(blacklegalposition);
	//再放大号棋子legalposition[1]
	for(int i=0;i<16;++i)
	{
		tempLine.assign(legalPosition+i*16,legalPosition+(i+1)*16);
		redlegalposition.push_back(tempLine);
		tempLine.clear();
	}
	legalposition_.push_back(redlegalposition);
}
bool ChessBase::CanMoveTo(const OneStep& _onestep, bool _isBigMan)
{
	//大号棋子在检查自己走棋的时候是看是否存在小号棋子会将军
	if (_isBigMan)
	{
		for (auto itr = g_global.smallManIDs.cbegin(); itr != g_global.smallManIDs.cend(); ++itr)
		{
			if (g_global.g_chessman[*itr]->GetAlive() && g_global.g_chessman[*itr]->CanKillKing(g_global.g_chessboard, g_global.g_chessman[g_global.bigKingId]->location()))
			{
				return false;
			}
		}
		return true;
	} 
	else
	{
		for (auto itr = g_global.bigManIDs.cbegin(); itr != g_global.bigManIDs.cend(); ++itr)
		{
			if (g_global.g_chessman[*itr]->GetAlive() && g_global.g_chessman[*itr]->CanKillKing(g_global.g_chessboard, g_global.g_chessman[g_global.smallKingId]->location()))
			{
				return false;
			}
		}
		return true;
	}
}
