#pragma once

#include <string>
#include "../chess_engine/global.h"

/*
���ߣ�QQ3508551694
*/

class CChessPlay;

class CCrFENHelper
{
    friend class CChessPlay;
public:
    static OneStep BestMoveToOnSetp(const std::string& _bestmoveLineStr, bool _aiUseRed);

private:
    static void LoadSituationFromFen(const std::string& _fenStr, std::string& _redBlack, int& _notKillCount, int& _totalCount);
    static std::string GetSituationFen(bool _aiUseRed);
    static void PutOneMan(int _manId, int _xFenIndex, int _yFenIndex);
    static bool isXIndex(char _c);
    static bool isYIndex(char _c);
};

