#pragma once
#include "chessbase.h"
//³µ
class  Chariot : public ChessBase {
public:
	Chariot(int sidetag, int _id, int value, const Coordinate& newlocation);
	~Chariot(void);
	void GetMove(const Matrix& chessboard, moves& temp) const;
	void GetCaptureValue(const Matrix& chessboard) const;
	bool CanKillKing(const Matrix& chessboard, const Coordinate& positionofking) const;
};
