#include "minister.h"
using namespace std;

Minister::Minister(int sidetag, int _id, int value, const Coordinate& newlocation)
:ChessBase(sidetag,_id, value,newlocation)
{
	m_legalcharacter=16;
//构造函数开始

//第89页红相矩阵
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
/*8 */	0, 0, 0, 0, 0, 25, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0,
/*9 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*10*/	0, 0, 0, 20, 0, 0, 0, 35, 0, 0, 0, 20, 0, 0, 0, 0,
/*11*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*12*/	0, 0, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0,

/*13*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*14*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*15*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	this->InitMatrix(positionvalue);

//构造函数结束
}

Minister::~Minister(void)
{
}

void Minister::GetMove(const Matrix& chessboard, moves& temp) const
{
	//******************
	//******************
	//***1*******2******
	//*****0***0********
	//*******m**********
	//*****0***0********
	//***3*******4******
	//******************
	//要移动的向量
    Coordinate direction[4] = {Coordinate(-2,-2), Coordinate(-2,+2), Coordinate(+2,-2), Coordinate(+2,+2)};
	vector<Coordinate> check;
	check.push_back(Coordinate(-1,-1));
	check.push_back(Coordinate(-1,+1));
	check.push_back(Coordinate(+1,-1));
	check.push_back(Coordinate(+1,+1));
	for(int i=0;i<4;i++)
	{
		int xto=location_.x+direction[i].x;
		int yto=location_.y+direction[i].y;
		int targetman=chessboard[xto][yto];
		//如果下一位置在棋盘上
		if(legalposition_[m_sidetag][xto][yto] & m_legalcharacter )
        {	//如果相眼位置无棋子
			if(chessboard[location_.x+check[i].x][location_.y+check[i].y]==0)
            {	if(targetman==0)//目标位置无棋子
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
				else if(targetman/32!=m_sidetag)//如果目标位置是对方棋子
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
}

void Minister::GetCaptureValue(const Matrix& chessboard) const
{
	//******************
	//******************
	//***1*******2******
	//*****0***0********
	//*******m**********
	//*****0***0********
	//***3*******4******
	//******************
	//要移动的向量
    Coordinate direction[4] = {Coordinate(-2,-2), Coordinate(-2,+2), Coordinate(+2,-2), Coordinate(+2,+2)};
	vector<Coordinate> check;
	check.push_back(Coordinate(-1,-1));
	check.push_back(Coordinate(-1,+1));
	check.push_back(Coordinate(+1,-1));
	check.push_back(Coordinate(+1,+1));

	for(int i=0;i<4;i++)
	{
		int xto=location_.x+direction[i].x;
		int yto=location_.y+direction[i].y;
		//如果下一位置在棋盘上
		if (legalposition_[m_sidetag][xto][yto] & m_legalcharacter)
		{
			//如果相眼位置无棋子
			if (chessboard[location_.x + check[i].x][location_.y + check[i].y] == 0)
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
				}
			}
		}
	}
}

bool Minister::CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const
{
	//******************
	//******************
	//***1*******2******
	//*****0***0********
	//*******m**********
	//*****0***0********
	//***3*******4******
	//******************
	//要移动的向量
    Coordinate direction[4] = {Coordinate(-2,-2), Coordinate(-2,+2), Coordinate(+2,-2), Coordinate(+2,+2)};
	vector<Coordinate> check;
	check.push_back(Coordinate(-1, -1));
	check.push_back(Coordinate(-1, +1));
	check.push_back(Coordinate(+1, -1));
	check.push_back(Coordinate(+1, +1));
	for (int i = 0; i<4; i++)
	{
		int xto = location_.x + direction[i].x;
		int yto = location_.y + direction[i].y;
		//如果下一位置在棋盘上
		if (legalposition_[m_sidetag][xto][yto] & m_legalcharacter)
		{
			//如果相眼位置无棋子
			if (chessboard[location_.x + check[i].x][location_.y + check[i].y] == 0)
			{
				//如果目标位置是对方将
				if (positionofking == Coordinate(xto, yto))
				{
					return true;
				}
			}
			
		}
		
	}
	return false;
}
