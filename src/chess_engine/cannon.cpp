#include "cannon.h"

using namespace std;

Cannon::Cannon(int sidetag, int _id, int value, const Coordinate& newlocation)
	:ChessBase(sidetag, _id, value,newlocation)
{
  m_legalcharacter=1;
//构造函数开始
//第9页红炮矩阵
  int positionvalue[16*16]= {
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
/*0 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*1 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*2 */	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/*3 */	0, 0, 0, 12, 13,10, 7, 6, 7,10, 13, 12, 0, 0, 0, 0,
/*4 */	0, 0, 0, 11, 12,10, 7, 6, 7,10, 12, 11, 0, 0, 0, 0,
/*5 */	0, 0, 0,10, 12, 9, 8, 8, 8, 9, 12,10, 0, 0, 0, 0,
/*6 */	0, 0, 0, 9, 11, 9, 11, 13, 11, 9, 11, 9, 0, 0, 0, 0,
/*7 */	0, 0, 0, 9, 11, 9, 11, 13, 11, 9, 11, 9, 0, 0, 0, 0,
/*8 */	0, 0, 0, 9,10, 9, 11, 13, 11, 9,10, 9, 0, 0, 0, 0,
/*9 */	0, 0, 0, 9,10, 9, 9, 11, 9, 9,10, 9, 0, 0, 0, 0,
/*10*/	0, 0, 0, 9,10, 8, 8, 7, 8, 8,10, 9, 0, 0, 0, 0,
/*11*/	0, 0, 0, 8, 9, 8, 7, 6, 7, 8, 9, 8, 0, 0, 0, 0,
/*12*/	0, 0, 0, 8, 9, 8, 7, 6, 7, 8, 9, 8, 0, 0, 0, 0,

/*13*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*14*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*15*/	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  this->InitMatrix(positionvalue);
//构造函数结束
}

Cannon::~Cannon(void)
{

}

void Cannon::GetMove(const Matrix& chessboard,moves& temp) const
{
  //********2*********
  //********|*********
  //********2*********
  //********|*********
  //**1<-1<-h->3->3***
  //********|*********
  //********4*********
  //********|*********
  //********4*********
  Coordinate direction[4] = {Coordinate(+0,-1), Coordinate(-1,+0), Coordinate(+0,+1), Coordinate(+1,+0)};
  int flag[4]={0, 0, 0, 0};//四个方向上，每个方向遇到的棋子个数
  for(int i=0;i<4;i++)
    //每个方向最多前进9个位置
    for(int j=1;j<=9;j++) 
	{
      int xto=location_.x+direction[i].x*j;
      int yto=location_.y+direction[i].y*j;
	  int targetman=chessboard[xto][yto];
      //目标位置在棋盘上
      if(this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
      {
        //未翻山
        if(flag[i] == 0 ) 
		{
            //目标位置无棋子
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
				continue;
            }
            //目标位置有棋子
            else
			{
				++flag[i];//第1个炮台
				continue;
            }
        }
        //翻山之后
        else 
		{
			//目标位置无棋子
			if (targetman == 0)
			{
				continue;
			}
			//目标位置有对方棋子
			else
			{
				++flag[i];//第二个炮台
				//if (targetman / 32 != m_sidetag)//第2个炮台是对方棋子
				if (targetman >> 5 != m_sidetag)//第2个炮台是对方棋子
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
				else
				{
					break;//第2个炮台是自己人
				}
			}
        }
      }
      //出界，换另一个方向
      else break;
    }
}

void Cannon::GetCaptureValue(const Matrix& chessboard) const
{
  //********2*********
  //********|*********
  //********2*********
  //********|*********
  //**1<-1<-h->3->3***
  //********|*********
  //********4*********
  //********|*********
  //********4*********
  Coordinate direction[4] = {Coordinate(+0,-1), Coordinate(-1,+0), Coordinate(+0,+1), Coordinate(+1,+0)};
  bool flag[4]={false,false,false,false};

  for(int i=0;i<4;i++)
    //每个方向最多前进9个位置
    for(int j=1;j<=9;j++)
	{
      int xto=location_.x+direction[i].x*j;
      int yto=location_.y+direction[i].y*j;
	  IdType targetman = chessboard[xto][yto];
      //目标位置在棋盘上
      if(this->legalposition_[m_sidetag][xto][yto] & this->m_legalcharacter)
      {
        //未翻山
        if(flag[i]==false ) 
		{
			//目标位置有棋子
			if (targetman != 0)
			{
				//目标位置有棋子，且该棋子为对方将，则调用空头炮威胁分值
				if (!IsSameSide(targetman) && targetman == g_global.bigKingId)
				{
					g_evaluate.sideValue[m_sidetag] += HollowCannonThreat[location_.y - 3];
				}
				else if (!IsSameSide(targetman) && targetman == g_global.smallKingId)
				{
					g_evaluate.sideValue[m_sidetag] += HollowCannonThreat[12-location_.y];
				}
				flag[i]=true;
				continue;
            }
        }
        //翻山之后
        else
		{
		    //目标位置有棋子
			if (targetman != 0)
			{
				if (IsSelfKill(targetman) || IsSelfChariot(targetman))
				{
					break;
				}
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
      }
      //出界，换另一个方向
	  else
	  {
		  break;
	  }
    }
}

bool Cannon::CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const
{
	//cannon can kill king just there is only one chessman between the cannon and the king
	//*****************
	//*******c*********
	//*******1*********
	//***c*1*k*1*c*****
	//*******1*********
	//*******c*********
	//*****************
	//the cannon and the king in the same row or in the same col
	if(location_.x == positionofking.x)
	{
		int max = location_.y > positionofking.y ? location_.y : positionofking.y;
		int min = location_.y < positionofking.y ? location_.y : positionofking.y;
		int chessmanbetween = 0;
		for (min += 1; min < max; ++min)
		{
			if (chessboard[location_.x][min] != 0)
			{
				++chessmanbetween;
			}
		}
		
		if (chessmanbetween == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(location_.y == positionofking.y)
	{
		int max=location_.x>positionofking.x?location_.x:positionofking.x;
		int min=location_.x<positionofking.x?location_.x:positionofking.x;
		int chessmanbetween = 0;
		for (min += 1; min < max; ++min)
		{
			if (chessboard[min][location_.y] != 0)
			{
				++chessmanbetween;
			}
		}
		
		if (chessmanbetween == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
