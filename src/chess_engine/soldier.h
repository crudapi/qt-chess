#pragma once

#include "chessbase.h"
//±ø¡¢×ä
class  Soldier : public ChessBase {
public:
	Soldier(int sidetag, int _id, int value, const Coordinate& newlocation);
	~Soldier(void);
	void GetMove(const Matrix& chessboard, moves& temp) const;
	void GetCaptureValue(const Matrix& chessboard) const;
	bool CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const;
};