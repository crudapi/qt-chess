#pragma once																		///<	防止重复引用机制

#include<vector>																	///<	顺序表容器
#include<iostream>																	///<	输入输出流
#include<iomanip>																	///<	manip是manipulator(操纵器的缩写),setw(域宽)，setbase(n)将数字转换成n进制
#include<utility>																	///<	STL标准模板库，use pair<int,int>模板类
#include<list>																		///<	链表容器
#include <unordered_map>															///<	存放历史走法及其评估结果，不用下次再重新评估
#include <chrono>  
#include <random>																	///<	随机数生成器
#include <array>

typedef int IdType;																	///<	ID类型
typedef std::vector<std::vector<int>> Matrix;													///<	vector套vector,二维数组
struct Coordinate
{
	Coordinate(void){ x = 0; y = 0; }
	Coordinate(int _x, int _y){ x = _x; y = _y; }
	friend bool operator==(const Coordinate& _lhs, const Coordinate& _rhs) { return _lhs.x == _rhs.x && _lhs.y == _rhs.y; }
	friend std::ostream& operator<<(std::ostream& os, const Coordinate& coordiname) { return os << "[" << coordiname.x << "," << coordiname.y << "]"; }
public:
	short x;
	short y;
};
struct MoveOnce
{
public:
	MoveOnce(void) :from(0, 0), to(0, 0){}
	MoveOnce(int _x1, int _y1, int _x2, int _y2) :from(_x1, _y1), to(_x2, _y2){}
	MoveOnce(const Coordinate& _from, const Coordinate& _to) :from(_from), to(_to){}
	bool operator==(const MoveOnce& _rhs) { return from == _rhs.from && to == _rhs.to; }
public:
	Coordinate from;
	Coordinate to;
};
struct  OneStep
{
public:
	OneStep(void){}
	OneStep(int _x1, int _y1, int _x2, int _y2, int _id) :move(_x1, _y1, _x2, _y2), targetId(_id){}
	OneStep(const MoveOnce& _moveonce, int _id) :move(_moveonce), targetId(_id){}
	OneStep(const Coordinate& _from, const Coordinate& _to, int _id) :move(_from, _to), targetId(_id){}
	bool operator==(const OneStep& _rhs) { return move == _rhs.move && targetId == _rhs.targetId; }
	friend bool operator< (const OneStep& _lhs, const OneStep& _rhs) { return true; }
public:
	MoveOnce move;
	IdType targetId;
};
typedef std::vector<OneStep> moves;														///<	vector容器管理每一个可行的，合理的走法
//全局函数

bool  good_step(const OneStep& s1, const OneStep& s2);					///<	严格弱排序
const int HollowCannonThreat[10] = { 0, 0, 0, 37, 43, 55, 57, 67, 53, 49 };
const int HorseCentralCannonThreat[10] = { 0, 0, 0, 43, 35, 24, 18, 9, 9, 9 };
const int BottomCannonThreat[9] = { 25, 21, 8, 0, 0, 0, 8, 21, 25 };

class ChessBase;																	///<	棋子基类

//Global成员的初始化交给CChessPlay
class  Global
{
public:
	Global(void);
	void InitChessManIDArray(void);
	void Clear(void);//清空棋盘
	std::string to_string(const Matrix& _matrix);
	std::string to_string(const OneStep& _step);

public:
	inline bool IsKillStep(const OneStep&  s1) {	return s1.targetId != 0;}											///<	返回OneStep的value比较大的一个								
	inline bool IsNotKillStep(const OneStep&  s1){ return s1.targetId == 0; }										///<	返回OneStep的value比较大的一个		
	bool BeProtected(bool _isTargetBigMan, int _x, int _y);
	OneStep GoAhead(const OneStep&);
	void GoBack(const OneStep&);
	inline bool IsValidId(IdType _id) { return smallManIDs[0] <= _id && _id <= bigManIDs[bigManIDs.size() - 1]; }

public:
	//棋盘全局变量
	Matrix g_chessboard;															///<	声明棋盘数组,全局变量[3][3]->[12][11]
	int g_xStart;//3棋盘下标开始的位置
	int g_xEnd;//3棋盘下标结束的下一个位置
	int g_yStart;//3棋盘下标开始的位置
	int g_yEnd;//3棋盘下标结束的下一个位置
	//棋子全局变量，这个数组中的动态内存交给CChessPlay类来管理：创建和释放
	std::vector<ChessBase*> g_chessman;												///<	声明棋子数组的顺序表容器

	const IdType bigKingId;
	const IdType smallKingId;

	std::array<IdType, 16> bigManIDs;
	std::array<IdType, 16> smallManIDs;
	//Zobrist
	int g_zobristMatrix[48][13][12];												//<		Zobrist 随机数组
	std::unordered_map<int, int> g_historyEvaluate;									//<		历史走法与评价结果
                                                    ///<	全局的随机数生成器

	//test status
	std::vector<std::string> g_chessmanName;												//		每个棋子的名字：车马炮
	std::array<int, 48> g_chessmanValue;

	OneStep killStep;
	OneStep engineCannotUseStep;
    //随机数生成器全局变量
    std::mt19937 g_randomer;
};

class  Evaluate
{
public:
	void Clear(void);

public:
	std::array<IdType, 48> manBeKilled;

	std::array<IdType, 48> manBeProtected;

	std::array<int, 2> sideValue;
};

extern Global g_global;
extern Evaluate g_evaluate;
