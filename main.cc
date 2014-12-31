#include <QApplication>
#include "mainwindow.hh"
#include <iostream>
#include "interpreter.hh"


using std::cout;
using std::endl;


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow* window = new MainWindow();
    window->show();
    return app.exec();

    /*if (argc != 2) {
        cout << "Usage: bfInt <filename>" << endl;
        return 0;
    }

    Interpreter interpreter;
    if (!interpreter.load(argv[1])) {
        cout << "Program could not be loaded. Aborting." << endl;
        return -1;
    }

    //cout << "C code:" << endl << endl << interpreter.toCppCode() << endl;
    cout << "Program execution:" << endl;

    if (interpreter.execute()) {
        cout << endl;
        return 0;
    } else {
        cout << endl;
        return -1;
    }*/
}

