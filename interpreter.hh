#ifndef INTERPRETER_HH
#define INTERPRETER_HH


#include <QObject>
#include <deque>
#include <map>
#include <iostream>
#include <stack>
#include <sstream>
#include <string>


using std::deque;
using std::istream;
using std::map;
using std::ostream;
using std::stack;
using std::string;
using std::stringstream;


class Interpreter : public QObject {

    Q_OBJECT

signals:

    void put(int);

public:

    Interpreter(ostream& outputStream = std::cout, istream& inputStream = std::cin);

    bool execute();
    bool isProgramValid();
    bool load(string fileName);
    bool load(stringstream& programStream);
    void printCells();
    string toCppCode();

public slots:

    void abortExecution();


private:

    void clear();
    void execute(int command);

    bool m_programValid;
    bool m_abortExecution;
    deque<int> m_cells;
    deque<int>::iterator m_currentCell;
    istream& m_inputStream;
    map<unsigned int, unsigned int> m_jumps;
    ostream& m_outputStream;
    string m_fileName;
    stringstream m_programStream;

};

#endif // INTERPRETER_HH
