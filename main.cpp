#include <QApplication>
#include "bturngesture.h"
#include "bpolygongesture.h"
#include "bgesturewidget.h"
#include "bturngesturev2.h"
#include "bcutgesture.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
//     bGuestures foo;
    
    
//     BTurnGestureRecognizer::registerRecognizer();
    BPolygonGestureRecognizer::registerRecognizer();
    BTurnGestureRecognizerV2::registerRecognizer();
    BCutGestureRecognizer::registerRecognizer();
    
    BGestureWidget widget;
    widget.show();
    
    return app.exec();
}
