#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPoint>
#include <QPixmap>
#include <QMouseEvent>

#include <chess_ui_helper.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public CchessUI
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
	void ModifyUIBase();
    ~MainWindow();
private:
	void paintEvent(QPaintEvent*);
    void createActions();
	void LoadChessManImage();
	void InitGDI(void);
	void RefreshUI();
	void PaintChessboard(QPainter& painter);
	void PaintChessman(QPainter& painter);
	void PaintOneChessman(QPainter& painter, const Coordinate& p, int id);
    void PaintSelected(QPainter& painter);
    void PaintChessmanSelected(QPainter& painter, const QPoint& _point);
    void ClearPieceBitMap(void);
	void mousePressEvent(QMouseEvent* event) override;
    QPixmap LoadBMP(std::string path);
    void OnLButtonDown(QPoint point);
    void SwitchStateTo(State state);
public slots:
    void exportBoard(void);
private:
    Ui::MainWindow *ui;
	QImage m_chessboard;
	std::map<int, QPixmap> m_chessmanPixmap;
	QToolBar* m_operToolBar = nullptr;
};
#endif // MAINWINDOW_H
