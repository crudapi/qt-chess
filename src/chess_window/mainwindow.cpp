#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QToolBar>
#include <iostream>
#include <functional>
using namespace std;
#include <QColor>
#include <QBitmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), CchessUI(5)
    , ui(new Ui::MainWindow)//5个工具栏菜单
{
    ui->setupUi(this);
	m_chessboard.load(":/images/chess_board/chessboard.bmp", "BMP");
	LoadChessManImage();
	createActions();
	ModifyUIBase();
	m_chessObject->InitChessPlay();
}
void MainWindow::ModifyUIBase()
{
    m_sizeConfig.uiXBase = 6;//水平方向：从左到右
    m_sizeConfig.uiYBase = m_operToolBar->height() + 5;//垂直方向：从上到下
	//设置整个对话框客户区的大小Rect
    this->setFixedSize(QSize(m_sizeConfig.uiXBase + m_sizeConfig.uiChessManSize * 9, m_sizeConfig.uiYBase + m_sizeConfig.uiChessManSize * 10));
    this->setWindowTitle(QString::fromLocal8Bit("Carea Chess"));
}
void MainWindow::createActions()
{
	//hint
	const QIcon hintIcon = QIcon::fromTheme("hint", QIcon(":/images/hint.png"));
	QAction* hintAct = new QAction(hintIcon, tr("&AI Hint"), this);
	hintAct->setStatusTip(tr("AI Hint"));
	//connect(hintAct, &QAction::triggered, this, &MainWindow::newFile);

	//ai-use-bottom
	const QIcon aiUseBottomIcon = QIcon::fromTheme("hint", QIcon(":/images/ai-bottom-yes.png"));
	QAction* aiUseBottomAct = new QAction(aiUseBottomIcon, tr("&AI Bottom"), this);
	aiUseBottomAct->setStatusTip(tr("AI Bottom"));
	//connect(hintAct, &QAction::triggered, this, &MainWindow::newFile);

	//ai-use-top
	const QIcon aiUseTopIcon = QIcon::fromTheme("hint", QIcon(":/images/ai-top-yes.png"));
	QAction* aiUseTopAct = new QAction(aiUseTopIcon, tr("&AI Top"), this);
	aiUseTopAct->setStatusTip(tr("AI Top"));
	//connect(hintAct, &QAction::triggered, this, &MainWindow::newFile);

	//swap
	const QIcon swapIcon = QIcon::fromTheme("hint", QIcon(":/images/swap-yes.png"));
	QAction* swapAct = new QAction(swapIcon, tr("&Swap"), this);
	swapAct->setStatusTip(tr("Swap"));
	//connect(hintAct, &QAction::triggered, this, &MainWindow::newFile);

	//swap
	const QIcon saveIcon = QIcon::fromTheme("hint", QIcon(":/images/save.png"));
	QAction* saveAct = new QAction(saveIcon, tr("&Save"), this);
	saveAct->setStatusTip(tr("Save"));
	//connect(hintAct, &QAction::triggered, this, &MainWindow::newFile);

	//! [19]
	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
	QAction* openAct = new QAction(openIcon, tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	//connect(openAct, &QAction::triggered, this, &MainWindow::open);

    const QIcon exportBoardIcon = QIcon::fromTheme("export-board", QIcon(":/images/export_board.png"));
    QAction* exportBoardAct = new QAction(exportBoardIcon, tr("&Export..."), this);
//    openAct->setShortcuts(QKeySequence::Open);
    exportBoardAct->setStatusTip(tr("Export current board"));
    connect(exportBoardAct, &QAction::triggered, this, &MainWindow::exportBoard);

		//QMenu* operMenu = menuBar()->addMenu(tr("&Oper"));
	QToolBar* operToolBar = m_operToolBar = addToolBar(tr("Oper"));

	operToolBar->addAction(hintAct);
	operToolBar->addAction(aiUseTopAct);
	operToolBar->addAction(aiUseBottomAct);
	operToolBar->addAction(swapAct);
	operToolBar->addAction(saveAct);
    operToolBar->addAction(openAct);
    operToolBar->addAction(exportBoardAct);
}

MainWindow::~MainWindow()
{
    delete ui;
}
QPixmap MainWindow::LoadBMP(std::string path)
{
    QPixmap pixmap;
    pixmap.load(path.c_str(), "BMP");
    //使用这种纯绿色颜色作为mask,只要图片中遇到这种颜色就擦除，变成透明的。而素材中的象棋棋子周围正好都是绿色。
    QBitmap mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt::MaskInColor);
    pixmap.setMask(mask);
    return pixmap;
}

void MainWindow::LoadChessManImage()
{
	//由于该函数可能会因为交换红黑双方而反复调用，所以需要先释放上次开辟的内存
	m_chessmanPixmap.clear();
	string cstrIDB;
	string pre = ":/images/chess_man/";

	for (size_t id = 16; id <= 47; ++id)
	{
		if (!m_swaped)//默认是不交换，不交换的时候红棋子在下方
		{
			switch (id) {
				//将
			case 16: cstrIDB = "red_shuai.bmp"; break;
			case 32: cstrIDB = "black_jiang.bmp";   break;
				//士
			case 17: cstrIDB = "red_shi.bmp";  break;
			case 18: cstrIDB = "red_shi.bmp";  break;
			case 33: cstrIDB = "black_shi.bmp";  break;
			case 34: cstrIDB = "black_shi.bmp";  break;
				//象
			case 19: cstrIDB = "red_xiang.bmp";  break;
			case 20: cstrIDB = "red_xiang.bmp";  break;
			case 35: cstrIDB = "black_xiang.bmp";  break;
			case 36: cstrIDB = "black_xiang.bmp";  break;
				//马
			case 21: cstrIDB = "red_horse.bmp";  break;
			case 22: cstrIDB = "red_horse.bmp";  break;
			case 37: cstrIDB = "black_horse.bmp";  break;
			case 38: cstrIDB = "black_horse.bmp";  break;
				//车
			case 23: cstrIDB = "red_che.bmp";  break;
			case 24: cstrIDB = "red_che.bmp";  break;
			case 39: cstrIDB = "black_che.bmp";  break;
			case 40: cstrIDB = "black_che.bmp";  break;
				//炮
			case 25: cstrIDB = "red_pao.bmp";  break;
			case 26: cstrIDB = "red_pao.bmp";  break;
			case 41: cstrIDB = "black_pao.bmp";  break;
			case 42: cstrIDB = "black_pao.bmp";  break;

			case 27: cstrIDB = "red_bing.bmp";  break;
			case 28: cstrIDB = "red_bing.bmp";  break;
			case 29: cstrIDB = "red_bing.bmp";  break;
			case 30: cstrIDB = "red_bing.bmp";  break;
			case 31: cstrIDB = "red_bing.bmp";  break;
			case 43: cstrIDB = "black_zu.bmp";  break;
			case 44: cstrIDB = "black_zu.bmp";  break;
			case 45: cstrIDB = "black_zu.bmp";  break;
			case 46: cstrIDB = "black_zu.bmp";  break;
			case 47: cstrIDB = "black_zu.bmp";  break;
			default:  cstrIDB = ""; assert(false); break;
			}
		}
		else
		{
			switch (id) {
				//将
			case 32: cstrIDB = "red_shuai.bmp"; break;
			case 16: cstrIDB = "black_jiang.bmp";   break;
				//士
			case 33: cstrIDB = "red_shi.bmp";  break;
			case 34: cstrIDB = "red_shi.bmp";  break;
			case 17: cstrIDB = "black_shi.bmp";  break;
			case 18: cstrIDB = "black_shi.bmp";  break;
				//象
			case 35: cstrIDB = "red_xiang.bmp";  break;
			case 36: cstrIDB = "red_xiang.bmp";  break;
			case 19: cstrIDB = "black_xiang.bmp";  break;
			case 20: cstrIDB = "black_xiang.bmp";  break;
				//马
			case 37: cstrIDB = "red_horse.bmp";  break;
			case 38: cstrIDB = "red_horse.bmp";  break;
			case 21: cstrIDB = "black_horse.bmp";  break;
			case 22: cstrIDB = "black_horse.bmp";  break;
				//车
			case 39: cstrIDB = "red_che.bmp";  break;
			case 40: cstrIDB = "red_che.bmp";  break;
			case 23: cstrIDB = "black_che.bmp";  break;
			case 24: cstrIDB = "black_che.bmp";  break;
				//炮
			case 41: cstrIDB = "red_pao.bmp";  break;
			case 42: cstrIDB = "red_pao.bmp";  break;
			case 25: cstrIDB = "black_pao.bmp";  break;
			case 26: cstrIDB = "black_pao.bmp";  break;

			case 43: cstrIDB = "red_bing.bmp";  break;
			case 44: cstrIDB = "red_bing.bmp";  break;
			case 45: cstrIDB = "red_bing.bmp";  break;
			case 46: cstrIDB = "red_bing.bmp";  break;
			case 47: cstrIDB = "red_bing.bmp";  break;

			case 27: cstrIDB = "black_zu.bmp";  break;
			case 28: cstrIDB = "black_zu.bmp";  break;
			case 29: cstrIDB = "black_zu.bmp";  break;
			case 30: cstrIDB = "black_zu.bmp";  break;
			case 31: cstrIDB = "black_zu.bmp";  break;

			default:  cstrIDB = ""; assert(false); break;
			}
		}

		if (m_chessmanPixmap.find(id) == m_chessmanPixmap.end())//当前ID还没有对应的位图，插入一个位图
		{
            auto chess_man_pix = LoadBMP(pre + cstrIDB);
			m_chessmanPixmap.insert(std::make_pair(id, chess_man_pix));
		}
	}
}
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    PaintChessboard(painter);
	PaintChessman(painter);
    PaintSelected(painter);
}

void MainWindow::PaintChessboard(QPainter& painter)
{
    //(1) your code
    // auto toolbar_height = m_operToolBar->height();
    // auto y = toolbar_height;

    /* 使用painter 的 drawPixmap 成员函数绘制 m_chessboard
    *  通过搜索可以发现 m_chessboard 已经在当前构造函数中加载了图片资源，这里只需要调用 drawPixmap 绘制显示即可
    * 上面两行注释起来的代码对你是有帮助的，因为绘制的时候你会发现棋盘不能遮挡住菜单的位置。
    * 让棋盘往下移动菜单高度的距离，即可保证棋盘不会遮挡住菜单
    */

}
void MainWindow::PaintChessmanSelected(QPainter& painter, const QPoint& point)
{
    //(2) your code
    if (point.x() == point.y() && point.x() == 0)//刚开始初始化界面的时候不要画哪个棋子被选中
    {
        return;
    }
    string path = ":/images/chess_man/SELECTED.BMP";
    auto selectMask = LoadBMP(path);

    Coordinate position = UIPointToChessManCoordinate(point);
    auto centerPoint = ChessManCoordinateToUIPoint(position);
    //draw pixmap

}

void MainWindow::PaintChessman(QPainter& painter)
{
    //(3) your code
	std::vector<Coordinate> aliveman;
	//获取所有活着的棋子，并根据他们的位置绘制界面上的位置，根据他们的ID绘制对应的图像
	aliveman = m_chessObject->AliveMan();

}
void MainWindow::PaintOneChessman(QPainter& painter, const Coordinate& cordiante, int _id)
{
    //(4) your code
//    QPoint point = ChessManCoordinateToUIPoint(cordiante);

}
void MainWindow::mousePressEvent(QMouseEvent* event)
{
	switch (event->button()) {
	case Qt::LeftButton:
	{
		//鼠标左键按下处理
		 event->pos();
         auto p = event->pos();
         OnLButtonDown(p);
         update();
    }
	break;
	case Qt::RightButton:     // 0x00000002
	{
		//鼠标右键按下处理
	}
	break;
	default:
		break;
	}
}
void MainWindow::PaintSelected(QPainter& painter)
{
    //分四个步骤绘制
    //1 点击本方棋子的起点位置
    //2 点击本方棋子的终点位置
    //3 对方棋子的起点位置
    //4 对方棋子的终点位置
    switch (m_selectState) {
    case State::EMPTY:
    {
        //do nothing
    }
    case State::AFTER_SELECTED_START:
    {
        PaintChessmanSelected(painter, m_userManPointStart);
        break;
    }
    case State::AFTER_SELECTED_TO:
    {
        //(5) your code


        break;
    }
    case State::AFTER_AI_SELECTED_START_AND_TO:
    {
        //(6) your code


        break;
    }
    default:
        assert(false);
        break;
    }
}
void MainWindow::OnLButtonDown(QPoint point)
{
    switch (m_selectState) {
    case State::AFTER_AI_SELECTED_START_AND_TO:
    {
        //(7) your code
        //切换状态

    }
    case State::EMPTY:
    {

        //第一次鼠标按下：获取起点
        auto can = CanMoveFrom(point);
        if (!can.first)
        {
           //(8) your code
           //切换状态


            return;
        }
        //用户走棋的出发点
        m_userManPointStart = point;
        //(9) your code
        //切换状态

        break;
    }
    case State::AFTER_SELECTED_START:
    {
        m_userManPointTo = point;

        Coordinate positionstart = UIPointToChessManCoordinate(m_userManPointStart);
        Coordinate positiontarget = UIPointToChessManCoordinate(m_userManPointTo);
        auto can = CanMoveTo(point);
        //检查从当前位置是否可达目标位置(point)
        if (can.first)
        {
            //执行人选择的走法
            OneStep userStep(positionstart, positiontarget, m_chessObject->ChessBoard(positiontarget));
            GoAhead(userStep);

            //重绘窗口
            update();//看到计算机走了一步棋

            //执行计算机的走法
            OneStep computerStep = m_chessObject->KillSortAlfaBataSearch(1);

            if (computerStep == g_global.killStep)
            {
                QMessageBox msgBox;
                msgBox.setText("恭喜你！你赢了！");
                msgBox.exec();
                return;
            }
            GoAhead(computerStep);
            m_moveOnceAISelected = computerStep.move;

            //看到计算机走了一步棋
            SwitchStateTo(State::AFTER_AI_SELECTED_START_AND_TO);

            if (m_chessObject->IsUserFailed())
            {
                //AfxMessageBox("你输了！");
                QMessageBox msgBox;
                msgBox.setText("你输了！");
                msgBox.exec();
            }
        }
        else {
            //invalid goahead , can not go there
            SwitchStateTo(State::EMPTY);
        }
        break;
    }
    case State::AFTER_SELECTED_TO:
    {
        //(10) your code
        //切换状态

        break;
    }
    default:
        assert(false);
        break;
    }
}

void MainWindow::SwitchStateTo(CchessUI::State state)
{
    m_selectState = state;
    update();
}

void MainWindow::exportBoard()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    qDebug()<<"filename="<<fileName;

    auto fen = m_chessObject->GetSituationFen(false);
    qDebug()<<"fen="<<fen.c_str();

    ofstream fout("fen.txt");
    fout<<fen;

}
