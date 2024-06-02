#pragma once
#include "global.h"
#include <array>

/*
作者：QQ3508551694
*/

class  ChessBase
{
 public:
	
	ChessBase(int sidetag, int _id, int value,const Coordinate& newlocation);																///<	用于构造一个棋子的基类部分
	virtual ~ChessBase(void);																									///<	基类虚析构函数
	virtual void GetMove(const Matrix& _chessboard, moves& tem) const = 0;														///<	得到棋子走法，属性不得修改
	virtual void GetCaptureValue(const Matrix& _chessboard) const = 0;														///<	得到棋子的价值
	virtual bool CanKillKing(const Matrix& _chessboard, const Coordinate& positionofking) const=0;									///<	胜利的判定
	//	一个棋子贡献的价值 = 位置价值+棋子本身的价值
	inline int PositionValue(void) const { return positionvalue_[location_.x][location_.y];}					///<	inline内联函数
	//	返回棋子所属的
	inline int sidetag(void) const {return m_sidetag;}
	inline bool IsBigMan(void) const { return m_sidetag == 1 ? true : false; }													///<	是否是大号棋子的判定
	inline const Coordinate& location(void) const { return location_; }																	///<	返回棋子在棋盘上的位置
	inline Coordinate& location(void) { return location_; }																	///<	返回棋子在棋盘上的位置
	inline void SetLocation(const Coordinate& newlocat) {this->location_=newlocat;}												///<	设定棋子在棋盘上的位置
	inline int Value(void) const { return m_value;}																				///<	
	inline bool GetAlive(void) {return alive_;}																					///<	判断棋子是否还在
	inline void SetAlive(bool a) {alive_=a;}																					///<	
	IdType GetSelfKing(void) const;																								///<
	inline int GetId(void) const { return m_id; }
	inline bool IsSameSide(IdType _targetMan) const { return m_sidetag == _targetMan / 32; }
	inline bool IsSelfKill(IdType _targetMan) const { return m_sidetag == 1 ? g_global.bigKingId == _targetMan : g_global.smallKingId == _targetMan; }
	inline bool IsSelfChariot(IdType _targetMan) const { return m_sidetag == 1 ? (40 == _targetMan || 39 == _targetMan ): (23 == _targetMan || 24 == _targetMan); }
public:
	static void InitLegalPosition(void);																						///<	static函数

private:
	ChessBase(const ChessBase&);																								///<	拷贝构造
	ChessBase& operator=(const ChessBase&);																						///<	重载赋值运算符

protected:
	
	void InitMatrix(int* begin);																								// 供派生类构造函数使用，实现位置价值数组的初始化
	//合法位置数组,legalposition_[0],legalposition_[1]
	static bool CanMoveTo(const OneStep& _onestep, bool _isBigMan);

protected:
    int m_sidetag;																												//sidetag =0 小号棋子 sidetag =1 大号棋子
    int m_id;
    int m_value;
    int m_legalcharacter;																										//棋子的位置特征值，跟合法位置数组按位与之后可以得出能否到达目标位置
    int m_othersidetag;

	Coordinate location_;																											// 棋子在棋盘上的位置
	
	Matrix positionvalue_;																										//位置价值数组
	
	bool alive_;																												//标志棋子是否被吃掉

protected:
	//合法位置数组
	static std::vector<Matrix> legalposition_;
	std::array<int,48> chessmanvalue_;
};
