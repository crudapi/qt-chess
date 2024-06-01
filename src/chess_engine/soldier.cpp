#include "soldier.h"

using namespace std;
Soldier::Soldier(int sidetag, int _id, int value, const Coordinate& newlocation)
:ChessBase(sidetag, _id, value,newlocation)
{
	m_legalcharacter=8;
//构造函数开始

//第91页红兵矩阵
	int positionvalue[16*16]=
	{
	//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
/*0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*1 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*2 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/*3 */	0, 0, 0,  1,  1,  1,  2,  2,  2,  1,  1,  1, 0, 0, 0, 0,
/*4 */	0, 0, 0,  2,  3,  4,  5, 6,  5,  4,  3,  2, 0, 0, 0, 0,
/*5 */	0, 0, 0,  2,  3,  3,  4,  4,  4,  3,  3,  2, 0, 0, 0, 0,
/*6 */	0, 0, 0,  2,  2,  2,  3,  3,  3,  2,  2,  2, 0, 0, 0, 0,
/*7 */	0, 0, 0,  2,  2,  2,  2,  2,  2,  2,  2,  2, 0, 0, 0, 0,
/*8 */	0, 0, 0,  1,  0,  2,  0,  2,  0,  2,  0,  1, 0, 0, 0, 0,
/*9 */	0, 0, 0,  1,  0,  1,  0,  2,  0,  1,  0,  1, 0, 0, 0, 0,
/*10*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*11*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*12*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/*13*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*14*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*15*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	this->InitMatrix(positionvalue);

//构造函数结束
}

Soldier::~Soldier(void)
{
}

void Soldier::GetMove(const Matrix& chessboard, moves& temp) const
{
	//******************
	//*******3*r*2******
	//*********1********
	//******************
	//*********1********
	//*******2*b*3******
	//******************
	std::vector<std::vector<Coordinate>> direction;
	std::vector<Coordinate> blackdirection,reddirection;

	blackdirection.push_back(Coordinate(+0,-1));
	blackdirection.push_back(Coordinate(-1, +0));
	blackdirection.push_back(Coordinate(+0, +1));

	reddirection.push_back(Coordinate(+0, -1));
	reddirection.push_back(Coordinate(+1, +0));
	reddirection.push_back(Coordinate(+0, +1));
	direction.push_back(blackdirection);
	direction.push_back(reddirection);
	//三个前进的方向，前，左，右
	for(int i=0;i<3;i++)
	{
		int xto=location_.x+direction[m_sidetag][i].x;
		int yto=location_.y+direction[m_sidetag][i].y;
		int targetman=chessboard[xto][yto];
		//下一位置在棋盘上
		if(this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
		{
			bool across=(m_sidetag==1)?((xto>=8 && xto<=12)?true:false):((xto<=7 && xto>=3)?true:false);
			//假设默认情况为过河之后
			if(targetman==0)
			{
				OneStep onestep(location_, Coordinate(xto, yto), 0);
				//走到目标位置之后，是否会导致自己的将被将军，如果会，则次走法不能生成
				g_global.GoAhead(onestep);
				if (CanMoveTo(onestep, IsBigMan()))
				{
					temp.push_back(onestep);
				}
				g_global.GoBack(onestep);
				//如果没过河，则不再向左走，不再向又走
			}
			else if( targetman/32!=m_sidetag)
			{
				OneStep onestep(location_, Coordinate(xto, yto), targetman);
				//走到目标位置之后，是否会导致自己的将被将军，如果会，则次走法不能生成
				g_global.GoAhead(onestep);
				if (CanMoveTo(onestep, IsBigMan()))
				{
					temp.push_back(onestep);
				}
				g_global.GoBack(onestep);
			}
			if (!across)
			{
				break;
			}
		}
	}
}

void Soldier::GetCaptureValue(const Matrix& chessboard) const
{
	//******************
	//*******3*r*2******
	//*********1********
	//******************
	//*********1********
	//*******2*b*3******
	//******************
	std::vector<std::vector<Coordinate>> direction;
	std::vector<Coordinate> blackdirection,reddirection;
	blackdirection.push_back(Coordinate(+0,-1));
	blackdirection.push_back(Coordinate(-1, +0));
	blackdirection.push_back(Coordinate(+0, +1));

	reddirection.push_back(Coordinate(+0, -1));
	reddirection.push_back(Coordinate(+1, +0));
	reddirection.push_back(Coordinate(+0, +1));
	direction.push_back(blackdirection);
	direction.push_back(reddirection);
	//三个前进的方向，前，左，右
	for(int i=0;i<3;i++)
	{
		int xto=location_.x+direction[m_sidetag][i].x;
		int yto=location_.y+direction[m_sidetag][i].y;
		int targetman=chessboard[xto][yto];
		//下一位置在棋盘上
		if(this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
		{
			bool across=(m_sidetag==1)?((xto>=8 && xto<=12)?true:false):((xto<=7 && xto>=3)?true:false);
			//目标位置有棋子
			if (targetman != 0)
			{
				if (IsSameSide(targetman) || IsSelfChariot(targetman))
				{
					++g_evaluate.manBeProtected[targetman];
				}
				else
				{
					++g_evaluate.manBeKilled[targetman];
				}
			}
			//如果没过河，则不再向左走，不再向又走
			if (!across)
			{
				break;
			}
		}
	}
}

bool Soldier::CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const
{
	//******************
	//*******3*r*2******
	//*********1********
	//******************
	//*********1********
	//*******2*b*3******
	//******************
	std::vector<std::vector<Coordinate>> direction;
	std::vector<Coordinate> blackdirection, reddirection;

	blackdirection.push_back(Coordinate(+0, -1));
	blackdirection.push_back(Coordinate(-1, +0));
	blackdirection.push_back(Coordinate(+0, +1));

	reddirection.push_back(Coordinate(+0, -1));
	reddirection.push_back(Coordinate(+1, +0));
	reddirection.push_back(Coordinate(+0, +1));
	direction.push_back(blackdirection);
	direction.push_back(reddirection);
	//三个前进的方向，前，左，右
	for (int i = 0; i<3; i++)
	{
		int xto = location_.x + direction[m_sidetag][i].x;
		int yto = location_.y + direction[m_sidetag][i].y;
		//下一位置在棋盘上
		if (this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
		{
			bool across = (m_sidetag == 1) ? ((xto >= 8 && xto <= 12) ? true : false) : ((xto <= 7 && xto >= 3) ? true : false);
			//如果目标位置是对方将
			if (positionofking == Coordinate(xto, yto))
			{
				return true;
			}
			if (!across)
			{
				break;
			}
		}
	}
	return false;
}
