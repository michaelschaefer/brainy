#include "interpreterthread.hh"


InterpreterThread::InterpreterThread(Interpreter* interpreter, QObject* parent) : QThread(parent) {
    m_interpreter = interpreter;
    connect(this, SIGNAL(abort()), m_interpreter, SLOT(abortExecution()));
}


void InterpreterThread::run() {
    bool result = m_interpreter->execute();
    emit(resultReady(result));
}
