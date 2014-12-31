#include <QDebug>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QThread>
#include <sstream>
#include "interpreterthread.hh"
#include "mainwindow.hh"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_interpreterThread = 0;

    setupCodeArea();
    setupMenus();

    m_splitter = new QSplitter(Qt::Vertical, this);
    m_splitter->addWidget(m_editor);
    m_splitter->addWidget(m_terminal);

    setCentralWidget(m_splitter);
    statusBar()->show();
}


void MainWindow::abortProgram() {
    if (m_interpreterThread != 0) {
        emit(m_interpreterThread->abort());
    }
}


void MainWindow::closeEvent(QCloseEvent* event) {
    event->accept();
}


void MainWindow::executeProgram() {
    std::stringstream stream;
    stream << m_editor->toPlainText().toStdString();
    if (!m_interpreter.load(stream)) {
        statusBar()->showMessage("Error while loading program");
    } else {
        statusBar()->showMessage("Program successfully loaded. Executing...");

        m_interpreterThread = new InterpreterThread(&m_interpreter, this);
        connect(m_interpreterThread, &InterpreterThread::resultReady, this, &MainWindow::executionFinished);
        connect(m_interpreterThread, &InterpreterThread::finished, m_interpreterThread, &QObject::deleteLater);

        m_actionExecuteProgram->setEnabled(false);
        m_interpreterThread->start();
    }
}


void MainWindow::executionFinished(bool result) {
    m_actionExecuteProgram->setEnabled(true);
    if (result) {
        statusBar()->showMessage("Execution successfully finished");
    } else {
        statusBar()->showMessage("Execution not successful");
    }
    m_terminal->appendPlainText(QString());
}


void MainWindow::openProgram() {
    QString fileName = QFileDialog::getOpenFileName(this, trUtf8("Open Program"), QDir::homePath(), trUtf8("Brainfuck files (*.bf)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            m_editor->setPlainText(QString(file.readAll()));
            file.close();
        } else {
            QMessageBox::critical(this, trUtf8("brainy"), trUtf8("File could not be opened!"));
        }
    }
}


void MainWindow::putCharacter(int character) {
    m_terminal->moveCursor(QTextCursor::End);

    if (m_actionOutputAsNumber->isChecked()) {
        m_terminal->insertPlainText(QString::number(character));
    } else {
        m_terminal->insertPlainText(QString((char)character));
    }

    if (m_actionOutputWithSpace->isChecked()) {
        m_terminal->insertPlainText(" ");
    }

    m_terminal->moveCursor(QTextCursor::End);
}


void MainWindow::saveProgram() {
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Save Program"), QDir::homePath(), trUtf8("Brainfuck files (*.bf)"));
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".bf")) {
            fileName.append(".bf");
        }

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(m_editor->toPlainText().toUtf8());
            file.close();
        } else {
            QMessageBox::critical(this, trUtf8("brainy"), trUtf8("File could not be written!"));
        }
    }
}


void MainWindow::setupCodeArea() {
    m_editor = new QPlainTextEdit(this);
    m_editor->setFont(QFont("monospace"));

    m_terminal = new QPlainTextEdit(this);
    m_terminal->setReadOnly(true);
    m_terminal->setFont(QFont("monospace"));
    //m_terminal->setStyleSheet("background-color: black; color: white;");

    connect(&m_interpreter, SIGNAL(put(int)), this, SLOT(putCharacter(int)));
}


void MainWindow::setupMenus() {
    m_menuFile = new QMenu(trUtf8("&File"), menuBar());
    m_menuProgram = new QMenu(trUtf8("&Program"), menuBar());
    m_menuProgramOutput = new QMenu(trUtf8("&Output"), m_menuProgram);

    // init actions
    m_actionAbortProgram = new QAction(trUtf8("&Abort Program"), m_menuProgram);
    m_actionClearTerminal = new QAction(trUtf8("&Clear Terminal"), m_menuProgram);
    m_actionExecuteProgram = new QAction(trUtf8("&Execute"), m_menuProgram);
    m_actionOpenProgram = new QAction(trUtf8("&Open Program"), m_menuFile);
    m_actionOutputAsNumber = new QAction(trUtf8("As &Number"), m_menuProgram);
    m_actionOutputWithSpace = new QAction(trUtf8("Add &Space"), m_menuProgram);
    m_actionQuit = new QAction(trUtf8("&Quit"), m_menuFile);
    m_actionSaveProgram = new QAction(trUtf8("&Save Program"), m_menuFile);

    // set shortcuts
    m_actionClearTerminal->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
    m_actionExecuteProgram->setShortcut(Qt::Key_F5);
    m_actionOpenProgram->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    m_actionQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    m_actionSaveProgram->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    m_actionOutputAsNumber->setCheckable(true);
    m_actionOutputAsNumber->setChecked(true);
    m_actionOutputWithSpace->setCheckable(true);
    m_actionOutputWithSpace->setChecked(true);

    // connect actions
    connect(m_actionAbortProgram, SIGNAL(triggered()), this, SLOT(abortProgram()));
    connect(m_actionClearTerminal, SIGNAL(triggered()), m_terminal, SLOT(clear()));
    connect(m_actionExecuteProgram, SIGNAL(triggered()), this, SLOT(executeProgram()));
    connect(m_actionOpenProgram, SIGNAL(triggered()), this, SLOT(openProgram()));
    connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_actionSaveProgram, SIGNAL(triggered()), this, SLOT(saveProgram()));


    m_menuFile->addAction(m_actionOpenProgram);
    m_menuFile->addAction(m_actionSaveProgram);
    m_menuFile->addAction(m_actionQuit);
    m_menuProgram->addAction(m_actionExecuteProgram);
    m_menuProgram->addAction(m_actionAbortProgram);
    m_menuProgram->addMenu(m_menuProgramOutput);
    m_menuProgram->addAction(m_actionClearTerminal);
    m_menuProgramOutput->addAction(m_actionOutputAsNumber);
    m_menuProgramOutput->addAction(m_actionOutputWithSpace);

    menuBar()->addMenu(m_menuFile);
    menuBar()->addMenu(m_menuProgram);
}
