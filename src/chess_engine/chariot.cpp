#include "chariot.h"
using namespace std;
//车
Chariot::Chariot(int sidetag, int _id, int value, const Coordinate& newlocation)
:ChessBase(sidetag, _id, value,newlocation)
{
	m_legalcharacter=1;
//构造函数开始

//第90页红车矩阵
	int positionvalue[16*16]=
	{
	//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
/*0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*1 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*2 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/*3 */	0, 0, 0, 16, 17, 16, 15, 15, 15, 16, 17, 16, 0, 0, 0, 0,
/*4 */	0, 0, 0, 17, 18, 17, 19, 25, 19, 17, 18, 17, 0, 0, 0, 0,
/*5 */	0, 0, 0, 17, 19, 20, 22, 24, 22, 20, 19, 17, 0, 0, 0, 0,
/*6 */	0, 0, 0, 18, 22, 21, 24, 25, 24, 21, 22, 18, 0, 0, 0, 0,
/*7 */	0, 0, 0, 18, 22, 21, 24, 25, 24, 21, 22, 18, 0, 0, 0, 0,
/*8 */	0, 0, 0, 18, 22, 21, 24, 25, 24, 21, 22, 18, 0, 0, 0, 0,
/*9 */	0, 0, 0, 17, 19, 18, 22, 24, 22, 18, 19, 17, 0, 0, 0, 0,
/*10*/	0, 0, 0, 17, 18, 17, 17, 16, 17, 17, 18, 17, 0, 0, 0, 0,
/*11*/	0, 0, 0, 16, 17, 16, 16, 15, 16, 16, 17, 16, 0, 0, 0, 0,
/*12*/	0, 0, 0, 15, 16, 15, 16, 15, 16, 15, 16, 15, 0, 0, 0, 0,

/*13*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*14*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*15*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

    this->InitMatrix(positionvalue);

//构造函数结束
}

Chariot::~Chariot(void)
{
}

void Chariot::GetMove(const Matrix& chessboard, moves& temp) const
{
	//******************
	//********2*********
	//********|*********
	//*****1<-h->3******
	//********|*********
	//********4*********
	//******************
	//四个方向
    Coordinate direction[4] = {Coordinate(+0,-1), Coordinate(-1,+0), Coordinate(+0,+1), Coordinate(+1,+0)};
	for (int i = 0; i < 4; i++)
	{
		//每个方向最多前进9个位置
		for (int j = 1; j <= 9; j++)
		{
			int xto = location_.x + direction[i].x*j;
			int yto = location_.y + direction[i].y*j;
			int targetman = chessboard[xto][yto];
			//目标位置在棋盘上
			if (this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
			{
				//目标位置无棋子
				if (targetman == 0)
				{
					OneStep onestep(location_, Coordinate(xto, yto), 0);
					//走到目标位置之后，是否会导致自己的将被将军，如果会，则次走法不能生成
					g_global.GoAhead(onestep);
					if (CanMoveTo(onestep, IsBigMan()))
					{
						temp.push_back(onestep);
					}
					g_global.GoBack(onestep);
					continue;
				}
				//目标位置为对方棋子
				else if (targetman / 32 != m_sidetag)
				{
					OneStep onestep(location_, Coordinate(xto, yto), targetman);
					//走到目标位置之后，是否会导致自己的将被将军，如果会，则次走法不能生成
					g_global.GoAhead(onestep);
					if (CanMoveTo(onestep, IsBigMan()))
					{
						temp.push_back(onestep);
					}
					g_global.GoBack(onestep);
					break;
				}
				else break;
			}
			//出界，换另一个方向
			else break;
		}
	}
		
}

void Chariot::GetCaptureValue(const Matrix& chessboard) const
{
	//******************
	//********2*********
	//********|*********
	//*****1<-h->3******
	//********|*********
	//********4*********
	//******************
	//四个方向
    Coordinate direction[4] = {Coordinate(+0,-1), Coordinate(-1,+0), Coordinate(+0,+1), Coordinate(+1,+0)};

	for (int i = 0; i < 4; i++)
	{
		//每个方向最多前进9个位置
		for(int j=1;j<=9;j++)
		{
			int xto=location_.x+direction[i].x*j;
			int yto=location_.y+direction[i].y*j;
			//目标位置在棋盘上
			if(this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
			{
				int targetman = chessboard[xto][yto];
				//目标位置有棋子
				if (IsSelfKill(targetman) || IsSelfChariot(targetman))
				{
					break;
				}
				if (targetman != 0)
				{
					if (IsSameSide(targetman))
					{
						++g_evaluate.manBeProtected[targetman];
					}
					else
					{
						++g_evaluate.manBeKilled[targetman];
					}
					break;
				}
			}
			//出界，换另一个方向
			else
			{
				break;
			}
		}
	}
}

bool Chariot::CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const
{
	if (location_.y == positionofking.y)
	{
		int startIndex = std::min(positionofking.x, location_.x);
		int endIndex = std::max(positionofking.x, location_.x);
		for (int row = startIndex + 1; row < endIndex; ++row)
		{
			if (g_global.g_chessboard[row][location_.y] != 0)
			{
				return false;
			}
		}
		return true;
	}
	else if (location_.x == positionofking.x)
	{
		int startIndex = std::min(positionofking.y, location_.y);
		int endIndex = std::max(positionofking.y, location_.y);
		for (int column = startIndex + 1; column < endIndex; ++column)
		{
			if (g_global.g_chessboard[location_.x][column] != 0)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}
