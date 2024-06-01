#include "official.h"
using namespace std;

Official::Official(int sidetag, int _id, int value, const Coordinate& newlocation)
:ChessBase(sidetag, _id, value,newlocation)
{
	m_legalcharacter=4;
//构造函数开始

//第89页红仕矩阵
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
/*10*/	0, 0, 0, 0, 0, 0, 30, 0, 30, 0, 0, 0, 0, 0, 0, 0,
/*11*/	0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0,
/*12*/	0, 0, 0, 0, 0, 0, 30, 0, 30, 0, 0, 0, 0, 0, 0, 0,

/*13*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*14*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*15*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	this->InitMatrix(positionvalue);

//构造函数结束
}

Official::~Official(void)
{
}

void Official::GetMove(const Matrix& chessboard, moves& temp) const
{
	//******************
	//******************
	//******1***2*******
	//********h*********
	//******3***4*******
	//******************
	//******************
    Coordinate direction[4] = {Coordinate(-1,-1), Coordinate(-1,+1), Coordinate(+1,-1), Coordinate(+1,+1)};
	for(int i=0;i<4;i++)
		{
			int xto=location_.x+direction[i].x;
			int yto=location_.y+direction[i].y;
			int targetman=chessboard[xto][yto];
			if(this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
            {
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
				}
				else if(targetman/32!=m_sidetag)
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
            }
		}
}

void Official::GetCaptureValue(const Matrix& chessboard) const
{
	//******************
	//******************
	//******1***2*******
	//********h*********
	//******3***4*******
	//******************
	//******************
    Coordinate direction[4] = {Coordinate(-1,-1), Coordinate(-1,+1), Coordinate(+1,-1), Coordinate(+1,+1)};
	for(int i=0;i<4;i++)
	{
		int xto=location_.x+direction[i].x;
		int yto=location_.y+direction[i].y;
		if (this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
		{
			int targetman = chessboard[xto][yto];
			//目标位置有棋子
			if (IsSelfKill(targetman) || IsSelfChariot(targetman))
			{
				continue;
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
			}
		}
	}
}

bool Official::CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const
{
	//******************
	//******************
	//******1***2*******
	//********h*********
	//******3***4*******
	//******************
	//******************
    Coordinate direction[4] = {Coordinate(-1,-1), Coordinate(-1,+1), Coordinate(+1,-1), Coordinate(+1,+1)};
	for (int i = 0; i<4; i++)
	{
		int xto = location_.x + direction[i].x;
		int yto = location_.y + direction[i].y;
		//如果下一位置在棋盘上
		if (this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
		{
			//如果目标位置是对方将
			if (positionofking == Coordinate(xto, yto))
			{
				return true;
			}
		}
	}
	return false;
}
