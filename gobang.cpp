#include "gobang.h"
#include "ui_gobang.h"
#include <QDebug>
GoBang::GoBang(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GoBang)
{
    ui->setupUi(this);
    InitUI();
    initQssFiles();
}

GoBang::~GoBang()
{
    delete ui;
}

void GoBang::paintEvent(QPaintEvent *event)
{

    DrawChessBody(); //
    DrawHandChess();
    DrawHandItem();
    update();
}

void GoBang::InitUI()
{

    // 定义棋盘大小
    resize((CHESS_C + 1) * MAX_X,(CHESS_R+1) * MAX_Y);
    this->setMaximumSize((CHESS_C + 1) * MAX_X,(CHESS_R+1) * MAX_Y);
    this->setMinimumSize((CHESS_C + 1) * MAX_X,(CHESS_R+1) * MAX_Y);
    setWindowTitle(codec->toUnicode("刘盼的五子棋程序"));
    b_black = false; // 默认白色
}

void GoBang::initQssFiles()
{
    //读取qss文件
    setWindowIcon(QIcon(":/img/icon.png"));
    QString styleFileName = "style";
    QFile File(":/qss/" + styleFileName +".qss");
    File.open(QFile::ReadOnly);
    QString styleSheet = (File.readAll());
//    qDebug() << styleSheet;
    setStyleSheet(styleSheet);
    File.close();
}
void GoBang::DrawChessBody()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing,true); // 防止变形
    painter.setPen(QPen(QColor(Qt::black),2)); // 边框颜色
    painter.setBrush(QColor("#D1C34D")); // bg color
    for (int i = 0; i < CHESS_C; ++i) {
        for (int j = 0; j < CHESS_R; ++j) {
            // 绘制矩形
            painter.drawRect((i+0.5)*MAX_X,(j+0.5)*MAX_Y,MAX_X,MAX_Y);
        }
    }

}

void GoBang::DrawHandChess()
{
    QPainter painter(this);


    // 设置渐变色 辐射渐变
    //创建了一个QRadialGradient对象实例，参数分别为中心坐标，半径长度和焦点坐标,如果需要对称那么中心坐标和焦点坐标要一致
    //    QRadialGradient linear(mapFromGlobal(QCursor::pos()),15,(mapFromGlobal(QCursor::pos())));
    //    linear.setColorAt(0, Qt::black);
    //    linear.setColorAt(0.1, Qt::black);
    //    linear.setColorAt(1, Qt::white);
    //    QLinearGradient linear1;
    //    linear1.setColorAt(1, Qt::black);
    //    linear1.setColorAt(0, Qt::white);

    painter.setPen(QPen(QColor(Qt::transparent))); // 给棋子的边框透明
    if(b_black)
    {
        painter.setBrush(Qt::black);
    }
    else
    {
        painter.setBrush(Qt::white);
    }
    painter.setRenderHint(QPainter::HighQualityAntialiasing,true); // 防止变形
    painter.drawEllipse(mapFromGlobal(QCursor::pos()),MAX_X / 2,MAX_Y /2);
}

void GoBang::DrawHandItem()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent))); //给棋子的边框线透明
    // 遍历棋子 有就绘制，没有就不绘制
    for (int i =0; i < p_ChessItem.size();++i) {
        ChessItem item = p_ChessItem[i]; // 当前的棋子的样式和位置赋值给棋子的这个类
        if(item._black)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::white);
        }
        DrawChessAtPoint(painter,item._pt);
    }
    //    painter.drawEllipse();
}

void GoBang::DrawChessAtPoint(QPainter &painter, QPoint &point)
{
    // 获取棋子的摆放位置
    QPoint ptCenter((point.x()+0.5)*MAX_X,(point.y()+0.5)*MAX_Y);
    painter.drawEllipse(ptCenter,MAX_X/2,MAX_Y/2);

}



void GoBang::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        this->close();

}

void GoBang::mousePressEvent(QMouseEvent *event)
{
    const QString HandStyle[2] = {"min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:white","min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:black"};
    static int barHandType = 0;

    // 获取鼠标点击的位置
    QStatusBar *bar = new QStatusBar(this);
    QLabel *LabHandType = new QLabel;
    bar->addWidget(LabHandType);
    LabHandType->setStyleSheet(HandStyle[barHandType++ % 2]);
    bar->setSizeGripEnabled(1);
    bar->showMessage(codec->toUnicode("    当前落子位置：(") + QString::number(event->pos().x()) + ','+ QString::number(event->pos().y()) + ')');
    setStatusBar(bar);

    QPoint pt ;
    pt.setX(event->pos().x() / MAX_X);
    pt.setY(event->pos().y() / MAX_Y);

    // 判断棋子是否存在
    for(int i =0; i < p_ChessItem.size(); i++)
    {
        ChessItem item = p_ChessItem[i];
        if(item._pt == pt)
            return; // 如果存在
    }
    // 如果不存在就绘制当前五子棋
    ChessItem item(pt,b_black);
    p_ChessItem.append(item);

    // 统计四个点位是否连接
    int nLeft       = CountNearItem(item,QPoint(-1,0));
    int nLeftUp     = CountNearItem(item,QPoint(-1,-1));
    int nUp         = CountNearItem(item,QPoint(0,-1));
    int nRight      = CountNearItem(item,QPoint(1,0));
    int nRightUp    = CountNearItem(item,QPoint(1,-1));
    int nRightDown  = CountNearItem(item,QPoint(1,1));
    int nDown       = CountNearItem(item,QPoint(0,1));
    int nLeftDown   = CountNearItem(item,QPoint(-1,1));
    if((nLeft + nRight) >=4 ||
            (nLeftUp + nRightDown) >= 4 ||
            (nUp + nDown) >=4 ||
            (nRightUp + nLeftDown) >=4)
    {
        QString str = codec->toUnicode(b_black?"黑棋胜":"白棋胜");
        QMessageBox msgBox;
        msgBox.setStyleSheet(QString("QMessageBox{width:100px;height:35px;background:white;}QLabel{min-width: 100px;}"));
        msgBox.setWindowTitle(codec->toUnicode("游戏结束"));
        msgBox.setText(str);
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.exec();
        p_ChessItem.clear();
        return;
    }
    // 棋子取反
    b_black = !b_black;
}
int GoBang::CountNearItem(ChessItem item, QPoint pt)
{
    int nCount = 0;
    item._pt += pt;
    while (p_ChessItem.contains(item)) {
        nCount++;
        item._pt += pt;
    }
    return nCount;
}
