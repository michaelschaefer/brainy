#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH


#include <QMainWindow>
#include <QMenu>
#include <QPlainTextEdit>
#include <QSplitter>
#include "interpreter.hh"
#include "interpreterthread.hh"


class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);


protected:

    void closeEvent(QCloseEvent* event);


private slots:

    void abortProgram();
    void executeProgram();
    void executionFinished(bool result);
    void openProgram();
    void putCharacter(int character);
    void saveProgram();


private:

    void setupMenus();
    void setupCodeArea();

    Interpreter m_interpreter;
    InterpreterThread* m_interpreterThread;
    QAction* m_actionAbortProgram;
    QAction* m_actionClearTerminal;
    QAction* m_actionExecuteProgram;
    QAction* m_actionOpenProgram;
    QAction* m_actionOutputAsNumber;
    QAction* m_actionOutputWithSpace;
    QAction* m_actionQuit;
    QAction* m_actionSaveProgram;
    QMenu* m_menuFile;
    QMenu* m_menuProgram;
    QMenu* m_menuProgramOutput;
    QPlainTextEdit* m_editor;
    QPlainTextEdit* m_terminal;
    QSplitter* m_splitter;

};

#endif // MAINWINDOW_HH
