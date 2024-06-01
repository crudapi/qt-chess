#pragma once

#include "chessbase.h"
// ø°¢ À
class  Official : public ChessBase {
public:
	Official(int sidetag, int _id, int value, const Coordinate& newlocation);
	~Official(void);
	void GetMove(const Matrix& chessboard, moves& temp) const;
	void GetCaptureValue(const Matrix& chessboard) const;
	bool CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const;
};