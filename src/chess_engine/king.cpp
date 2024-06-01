#include "king.h"
#include "global.h"
#include "chessbase.h"
using namespace std;

King::King(int sidetag, int _id, int value, const Coordinate& newlocation)
:ChessBase(sidetag, _id, value,newlocation)
{
	m_legalcharacter=2;
    int positionvalue[16*16]=
	{
	//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
/*0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*1 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*2 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/*3 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*4 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*5 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*6 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*7 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*8 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*9 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*10*/	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
/*11*/	0, 0, 0, 0, 0, 0, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0,
/*12*/	0, 0, 0, 0, 0, 0, 15, 20, 15, 0, 0, 0, 0, 0, 0, 0,

/*13*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*14*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*15*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	this->InitMatrix(positionvalue);

//构造函数结束
}
King::~King(void)
{
}
bool King::IsKingKillKing(const IdType _id, const int _xTo, const int _yTo) const
{
	IdType anotherKingThere = (_id == g_global.bigKingId) ? g_global.smallKingId : g_global.bigKingId;
	
	int anotherY = g_global.g_chessman[anotherKingThere]->location().x;
	//两个老将不在一条直线上
	if (anotherY != _yTo)
	{
		return false;
	}
	//两个老将在一条直线上
	bool kill = true;
	int anotherX = g_global.g_chessman[anotherKingThere]->location().x;
	int start, end;
	(_xTo < anotherX) ? (start = _xTo, end = anotherX) : (start = anotherX, end = _xTo);
	for (int i = start + 1; i <= end -1; ++i)
	{
		if (g_global.g_chessboard[i][anotherY] !=0)
		{
			kill = false;
			break;
		}
	}
	return kill;
}
void King::GetMove(const Matrix& chessboard, moves& _temp) const
{
	//******************
	//******************
	//********2*********
	//******1*h*3*******
	//********4*********
	//******************
	//******************
	//四个方向前进的距离
    Coordinate direction[4] = {Coordinate(+0,-1), Coordinate(-1,+0), Coordinate(+0,+1), Coordinate(+1,+0)};
	//四个方向都尝试走一遍
	for(int i=0;i<4;i++)
	{
		int xto=location_.x+direction[i].x;
		int yto=location_.y+direction[i].y;
		int targetman=chessboard[xto][yto];
		if(legalposition_[m_sidetag][xto][yto] & m_legalcharacter)//只考虑当前棋子本身在走法上能不能到达目的位置
        {
			if(targetman==0 && !IsKingKillKing(GetSelfKing(), xto, yto))
			{
				OneStep onestep(location_, Coordinate(xto, yto), 0);
				//走到目标位置之后，是否会导致自己的将被将军，如果会，则次走法不能生成
				g_global.GoAhead(onestep);
				if (CanMoveTo(onestep, IsBigMan()))
				{
					_temp.push_back(onestep);
				}
				g_global.GoBack(onestep);
			}
			else if (targetman != 0 && (targetman / 32 != m_sidetag) && !IsKingKillKing(GetSelfKing(), xto, yto))//吃掉对方棋子
			{
				OneStep onestep(location_, Coordinate(xto, yto), targetman);
				//走到目标位置之后，是否会导致自己的将被将军，如果会，则次走法不能生成
				g_global.GoAhead(onestep);
				if (CanMoveTo(onestep, IsBigMan()))
				{
					_temp.push_back(onestep);
				}
				g_global.GoBack(onestep);
			}
        }
	}
}

void King::GetCaptureValue(const Matrix& chessboard) const
{
	//******************
	//******************
	//********2*********
	//******1*h*3*******
	//********4*********
	//******************
	//******************
	//四个方向前进的距离
    Coordinate direction[4] = {Coordinate(+0,-1), Coordinate(-1,+0), Coordinate(+0,+1), Coordinate(+1,+0)};
	//四个方向都尝试走一遍
	for (int i = 0; i<4; i++)
	{
		int xto = location_.x + direction[i].x;
		int yto = location_.y + direction[i].y;
		int targetman = chessboard[xto][yto];
		if (legalposition_[m_sidetag][xto][yto] & m_legalcharacter)//只考虑当前棋子本身在走法上能不能到达目的位置
		{
			if (!IsKingKillKing(GetSelfKing(), xto, yto) && targetman != 0)
			{
				//目标位置有棋子
				if (IsSameSide(targetman))
				{
					++g_evaluate.manBeProtected[targetman];
				}
				else
				{
					++g_evaluate.manBeKilled[targetman];
				}
			}
		}
	}
}

bool King::CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const
{
	int colmumn = location_.y;
	if (colmumn == positionofking.y)
	{
		int startIndex = std::min(positionofking.x, location_.x);
		int endIndex = std::max(positionofking.x, location_.x);
		for (int row = startIndex + 1; row < endIndex; ++row)
		{
			if (g_global.g_chessboard[row][colmumn] != 0)
			{
				return false;
			}
		}
		return true;
	}
	
	return false;
}
