// #include "stdafx.h"
#include "CrFenHelper.h"

#include "../chess_engine/chessbase.h"
#include "../chess_engine/global.h"

#include <unordered_map>
#include <sstream>
#include <vector>
#include <cctype>

void CCrFENHelper::PutOneMan(int _manId, int _xFenIndex, int _yFenIndex)
{
	g_global.g_chessman[_manId]->SetAlive(true);
	g_global.g_chessman[_manId]->SetLocation(Coordinate(_xFenIndex + g_global.g_xStart, _yFenIndex + g_global.g_yStart));
	g_global.g_chessboard[_xFenIndex + g_global.g_xStart][_yFenIndex + g_global.g_yStart] = _manId;
}
void CCrFENHelper::LoadSituationFromFen(const std::string& _fenStr, std::string& _redBlack, int& _notKillCount, int& _totalCount)
{
	//demo "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1"
	//demo "rhbakabhr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RHBAKABHR w - - 0 1"  冰河反馈的信息
	//demo position fen 1rbakabnr/9/2n1c2c1/p1p1p1p1p/9/9/P1P1P1P1P/1CN3NC1/9/R1BAKAB1R  r - - 0 3
	//use '/' to getline
	std::vector<std::string> arrLines;
	std::istringstream iss(_fenStr);
	std::string tempLine;
	while (std::getline(iss, tempLine, '/'))
	{
		arrLines.push_back(tempLine);
	}
	if (arrLines.size() != 10)
	{
		throw std::runtime_error("invalid fen string, not 10 lines.");
	}
	//删除第一行的position fen
	auto firstLine = arrLines[0];
	auto itrFind = firstLine.find("position fen ");
	firstLine.erase(itrFind, itrFind + std::string("position fen ").length());
	arrLines[0] = firstLine;
	//处理最后一行
	std::istringstream issLastLine(arrLines[9]);
	issLastLine >> arrLines[arrLines.size() - 1];//读取最后一行在棋盘上的数据
	issLastLine >> _redBlack;//轮到哪一方走子
	std::string nothing;
	issLastLine >> nothing >> nothing;// - -
	issLastLine >> _notKillCount >> _totalCount;

	//清空棋盘
	g_global.Clear();
	std::unordered_map<char, int> idCreateMap;
	idCreateMap['r'] = 39;
	idCreateMap['n'] = 37;
	idCreateMap['h'] = 37;
	idCreateMap['b'] = 35;
	idCreateMap['a'] = 33;
	idCreateMap['k'] = 32;
	idCreateMap['R'] = 23;
	idCreateMap['N'] = 21;
	idCreateMap['H'] = 21;
	idCreateMap['B'] = 19;
	idCreateMap['A'] = 17;
	idCreateMap['K'] = 16;
	idCreateMap['c'] = 41;
	idCreateMap['p'] = 43;
	idCreateMap['C'] = 25;
	idCreateMap['P'] = 27;
	int yFenIndex = 0;
	for (size_t x = 0; x < arrLines.size(); ++x)
	{
		yFenIndex = 0;//从新开始一行
		std::string tempLine = arrLines[x];
		for (size_t y = 0; y < tempLine.size(); ++y)
		{
			char element = tempLine[y];
			switch (element)
			{
			case 'r':
			case 'n':
			case 'h':
			case 'b':
			case 'a':
			case 'k':
			case 'R':
			case 'N':
			case 'H':
			case 'B':
			case 'A':
			case 'K':
			case 'c':
			case 'p':
			case 'C':
			case 'P':
			{
						PutOneMan(idCreateMap[element]++, x, yFenIndex);
						yFenIndex++;
						break;
			}
			case '1':
			{
						yFenIndex += 1;
						break;
			}
			case '2':
			{
						yFenIndex += 2;
						break;
			}
			case '3':
			{
						yFenIndex += 3;
						break;
			}
			case '4':
			{
						yFenIndex += 4;
						break;
			}
			case '5':
			{
						yFenIndex += 5;
						break;
			}
			case '6':
			{
						yFenIndex += 6;
						break;
			}
			case '7':
			{
						yFenIndex += 7;
						break;
			}
			case '8':
			{
						yFenIndex += 8;
						break;
			}
			case '9':
			{
						yFenIndex += 9;
						break;
			}
			default:
				throw std::runtime_error("invalid char[" + std::string(1, element) + "] in fen string.");
				break;
			}
		}
	}
}

std::string CCrFENHelper::GetSituationFen(bool _aiUseRed)
{
	//主要是遍历棋盘上的棋子，一边遍历一边生成FEN串
	Matrix chessboard180;

	////180度旋转==上下对调+左右对调
	//auto& board = g_global.g_chessboard;
	//for (auto itr = g_global.g_chessboard.crbegin(); itr != g_global.g_chessboard.crend(); ++itr)
	//{
	//	std::vector<int> tempLine(++(itr->crbegin()), itr->crend());//原来的右侧多一列0，所以对调之后要去掉，以保证局面坐标的合理性
	//	chessboard180.push_back(tempLine);
	//}

	chessboard180 = g_global.g_chessboard;
	
	std::string fenStr = "position fen ";

	for (int i = g_global.g_yStart; i < g_global.g_yEnd; ++i)
	{
		if (i > g_global.g_yStart)
		{
			fenStr += "/";
		}
		for (int j = g_global.g_xStart; j < g_global.g_xEnd;)
		{
			switch (chessboard180[i][j])
			{
			case 0:
			{
				int count = 0;
				while (chessboard180[i][j] == 0 && j < g_global.g_xEnd){ ++j; ++count;}
				fenStr += std::to_string(count);  break;
			}
			case 16:{ fenStr += 'K'; ++j; break; }
			case 17:
			case 18:{ fenStr += 'A'; ++j; break; }
			case 19:
			case 20:{ fenStr += 'B'; ++j; break; }
			case 21:
			case 22:{ fenStr += 'N'; ++j; break; }
			case 23:
			case 24:{ fenStr += 'R'; ++j; break; }
			case 25:
			case 26:{ fenStr += 'C'; ++j; break; }
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:{ fenStr += 'P'; ++j; break; }

			case 32:{ fenStr += 'k'; ++j; break; }
			
			case 33:
			case 34:{ fenStr += 'a'; ++j; break; }
			case 35:
			case 36:{ fenStr += 'b'; ++j; break; }
			case 37:
			case 38:{ fenStr += 'n'; ++j; break; }
			case 39:
			case 40:{ fenStr += 'r'; ++j; break;}
			case 41:
			case 42:{ fenStr += 'c'; ++j; break;}
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:{ fenStr += 'p'; ++j; break; }
			default: throw std::runtime_error("global g_chessboard has invalid value.");
			}
		}
	}

	//fenStr += _aiUseRed ? "  b - - 0 1" : "  r - - 0 1";
	// 轮到哪方走棋w=red r=red b=black
	fenStr += _aiUseRed ? "  r - - 0 1" : "  b - - 0 1";
	//static int moveCount = 0;
	//fenStr += _aiUseRed ? "  b - - 0 " : "  r - - 0 " + std::to_string(++moveCount);
	
	return fenStr;
}
bool CCrFENHelper::isXIndex(char c)
{
	if ( c >= '0' && c <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool CCrFENHelper::isYIndex(char c)
{
	if (c >= 'a' && c <= 'i')
	{
		return true;
	}
	else
	{
		return false;
	}
}
OneStep CCrFENHelper::BestMoveToOnSetp(const std::string& _bestmoveLineStr, bool _aiUseRed)
{
	std::istringstream iss(_bestmoveLineStr);
	std::string tempLine;
	//(1)//bestmove e2e6
	if (_bestmoveLineStr.find("bestmove") != std::string::npos)
	{
		iss >> tempLine >> tempLine;
	}
	//(2)info depth 1 score 3 pv i0h0
	//(3)"info depth 14 score 38 time 893 nodes 2396946 nps 2684150 pv h7e7 h0g2 h9g7 g3g4 i9h9 i0h0 h9h5 b0c2 b9c7 c3c4 a9a8 h2i2 h5h0 g2h0 \r"
	else
	{
		auto itrPv = _bestmoveLineStr.find("pv");
		tempLine = _bestmoveLineStr.substr(itrPv, 7);
		tempLine = tempLine.substr(3);
	}
	
	//                                     yFrom xFrom yTo xTo
	//                                      0     1     2   3
	//x
	//9  3
	//8  4
	//....
	//0  12

	//y
	//abcdefg h  i
	//3456789 10 11
	
	char xFrom, xTo, yFrom, yTo;
	
	xFrom = tempLine[1];
	xTo = tempLine[3];

	yFrom = tempLine[0];
	yTo = tempLine[2];
	
	if (isXIndex(xFrom) && isXIndex(xTo) &&isYIndex(yFrom) && isYIndex(yTo))
	{
		//do nothing
	}
	else
	{
		throw std::runtime_error("bestmove content " + _bestmoveLineStr + " is invalid !");
	}

	Coordinate from, to;
	from = Coordinate(12 - (xFrom - '0'), yFrom - 'a' + 3);
	to = Coordinate(12 - (xTo - '0'), yTo - 'a' + 3);

	////坐标做一次180度旋转才会到对面的计算机那里，x相加等于15，y相加等于14
	//from.x = 15 - from.x;
	//to.x = 15 - to.x;

	//from.y = 14 - from.y;
	//to.y = 14 - to.y;

	
	int id = g_global.g_chessboard[to.x][to.y];
	return OneStep(from, to, id);
}
