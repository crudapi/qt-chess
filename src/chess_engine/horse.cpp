#include "horse.h"
using namespace std;

Horse::Horse(int sidetag, int _id, int value, const Coordinate& newlocation)
:ChessBase(sidetag, _id, value,newlocation)
{
	m_legalcharacter=1;
//构造函数开始

//第9页，红马矩阵
	int positionvalue[16*16]=
	{
	//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
/*0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*1 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*2 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/*3 */	0, 0, 0, 7, 8, 9, 8, 7, 8, 9, 8, 7, 0, 0, 0, 0,
/*4 */	0, 0, 0, 8, 11, 12, 9, 7, 9, 12, 11, 8, 0, 0, 0, 0,
/*5 */	0, 0, 0, 9,10, 12, 12, 12, 12, 12,10, 9, 0, 0, 0, 0,
/*6 */	0, 0, 0, 9,10, 12, 13, 11, 13, 12,10, 9, 0, 0, 0, 0,
/*7 */	0, 0, 0, 9, 11, 11, 12,10, 12, 11, 11, 9, 0, 0, 0, 0,
/*8 */	0, 0, 0, 9,10,10, 11,10, 11,10,10, 9, 0, 0, 0, 0,
/*9 */	0, 0, 0, 8, 9,10,10, 9,10,10, 9, 8, 0, 0, 0, 0,
/*10*/	0, 0, 0, 8, 8, 9, 9, 8, 9, 9, 8, 8, 0, 0, 0, 0,
/*11*/	0, 0, 0, 7, 7, 7, 7, 5, 7, 7, 7, 7, 0, 0, 0, 0,
/*12*/	0, 0, 0, 6, 7, 7, 7, 6, 7, 7, 7, 6, 0, 0, 0, 0,

/*13*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*14*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*15*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	this->InitMatrix(positionvalue);

//构造函数结束
}

Horse::~Horse(void)
{
}

void Horse::GetMove(const Matrix& chessboard, moves& temp) const
{
	//******************
	//*******1*2********
	//******3*0*4*******
	//*******0h0********
	//******5*0*6*******
	//*******7*8********
	//******************
	//要移动的向量
    Coordinate direction[8] = {Coordinate(-2,-1), Coordinate(-2,+1), Coordinate(-1,-2), Coordinate(-1,+2),
                               Coordinate(+1,-2), Coordinate(+1,+2), Coordinate(+2,-1), Coordinate(+2,+1),
                              };
	//马腿位置
	vector<Coordinate> check;
	//check.reserve(8);
	check.push_back(Coordinate(-1,0));//1
	check.push_back(Coordinate(-1,0));//2
	check.push_back(Coordinate(+0,-1));//3
	check.push_back(Coordinate(+0,+1));//4
	check.push_back(Coordinate(+0,-1));//5
	check.push_back(Coordinate(+0,+1));//6
	check.push_back(Coordinate(+1,0));//7
	check.push_back(Coordinate(+1,0));//8

	//八个可行方向
	for(int i=0;i<8;i++)
	{
		int xto=location_.x+direction[i].x;
		int yto=location_.y+direction[i].y;
		int targetman=chessboard[xto][yto];
		//如果下一位置在棋盘上
		if (legalposition_[m_sidetag][xto][yto] & m_legalcharacter)
		{
			//如果马腿位置无棋子
			if (chessboard[location_.x + check[i].x][location_.y + check[i].y] == 0)
			{
				//如果目标位置无本方棋子
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
				}
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
				}
			}
		}
	}
}

void Horse::GetCaptureValue(const Matrix& chessboard) const
{
	//******************
	//*******1*2********
	//******3*0*4*******
	//*******0h0********
	//******5*0*6*******
	//*******7*8********
	//******************
	//要移动的向量
    Coordinate direction[8] = {Coordinate(-2,-1), Coordinate(-2,+1), Coordinate(-1,-2), Coordinate(-1,+2),
                               Coordinate(+1,-2), Coordinate(+1,+2), Coordinate(+2,-1), Coordinate(+2,+1),
                              };
	//马腿位置
	vector<Coordinate> check;
	//check.reserve(8);
	check.push_back(Coordinate(-1,0));//1
	check.push_back(Coordinate(-1,0));//2
	check.push_back(Coordinate(+0,-1));//3
	check.push_back(Coordinate(+0,+1));//4
	check.push_back(Coordinate(+0,-1));//5
	check.push_back(Coordinate(+0,+1));//6
	check.push_back(Coordinate(+1,0));//7
	check.push_back(Coordinate(+1,0));//8

	//八个可行方向
	for(int i=0;i<8;i++)
	{
		int xto=location_.x+direction[i].x;
		int yto=location_.y+direction[i].y;
		//如果下一位置在棋盘上
		if ((legalposition_[m_sidetag][xto][yto] & m_legalcharacter) &&
			//如果马腿位置无棋子
			(chessboard[location_.x + check[i].x][location_.y + check[i].y] == 0))
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

bool Horse::CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const
{
	//******************
	//*******1*2********
	//******3*0*4*******
	//*******0h0********
	//******5*0*6*******
	//*******7*8********
	//******************
	//要移动的向量
    Coordinate direction[8] = {Coordinate(-2,-1), Coordinate(-2,+1), Coordinate(-1,-2), Coordinate(-1,+2),
                               Coordinate(+1,-2), Coordinate(+1,+2), Coordinate(+2,-1), Coordinate(+2,+1),
                              };
	//马腿位置
	vector<Coordinate> check;
	//check.reserve(8);
	check.push_back(Coordinate(-1, 0));//1
	check.push_back(Coordinate(-1, 0));//2
	check.push_back(Coordinate(+0, -1));//3
	check.push_back(Coordinate(+0, +1));//4
	check.push_back(Coordinate(+0, -1));//5
	check.push_back(Coordinate(+0, +1));//6
	check.push_back(Coordinate(+1, 0));//7
	check.push_back(Coordinate(+1, 0));//8

	//八个可行方向
	for (int i = 0; i<8; i++)
	{
		int xto = location_.x + direction[i].x;
		int yto = location_.y + direction[i].y;
		//int targetman = chessboard[xto][yto];
		//如果下一位置在棋盘上
		if (legalposition_[m_sidetag][xto][yto] & m_legalcharacter)
		{
			//如果马腿位置无棋子
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
