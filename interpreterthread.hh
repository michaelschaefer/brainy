#ifndef INTERPRETERTHREAD_HH
#define INTERPRETERTHREAD_HH


#include <QThread>
#include "interpreter.hh"


class InterpreterThread : public QThread {

    Q_OBJECT

signals:

    void abort();
    void resultReady(bool);


public:

    InterpreterThread(Interpreter* interpreter, QObject* parent = 0);

    void run();


private:

    Interpreter* m_interpreter;

};

#endif // INTERPRETERTHREAD_HH
