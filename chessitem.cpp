#include "chessitem.h"

ChessItem::ChessItem()
{

}

ChessItem::ChessItem(QPoint point, bool isBlack)
{
    _pt = point;
    _black = isBlack;

}
