#ifndef GOBANG_H
#define GOBANG_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVector>
#include <QString>
#include <QTextCodec>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <QRadialGradient>
#include <QLabel>
#include <QMessageBox>
#include <QDir>
#include "chessitem.h"

#define CHESS_R 15
#define CHESS_C 15
#define MAX_X   50
#define MAX_Y   50


/*
 * Qt的2d绘画
*/
QT_BEGIN_NAMESPACE
namespace Ui { class GoBang; }
QT_END_NAMESPACE


class GoBang : public QMainWindow
{
    Q_OBJECT

public:
    GoBang(QWidget *parent = nullptr);
    ~GoBang();
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    void InitUI(); // 初始化界面
    void DrawChessBody(); // 画棋盘
    void DrawHandChess(); // 画棋子
    void DrawHandItem(); // 绘制棋盘上的棋子
    void DrawChessAtPoint(QPainter &painter, QPoint &point); // 画棋子的样式和位置
    int CountNearItem(ChessItem item, QPoint pt); // 判断棋子是否五子连接
private:
    QTextCodec *codec = QTextCodec::codecForName("GB2312");


private:
    Ui::GoBang *ui;
    bool b_black; // 定义棋子的颜色
    QVector<ChessItem> p_ChessItem; // 定义棋子个数
    void initQssFiles();
};
#endif // GOBANG_H
