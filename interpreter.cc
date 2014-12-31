#include <climits>
#include <fstream>
#include "interpreter.hh"


using std::endl;
using std::flush;
using std::fstream;


Interpreter::Interpreter(ostream& outputStream, std::istream &inputStream)
    : m_inputStream(inputStream), m_outputStream(outputStream)
{
    m_programValid = false;
    clear();
}


void Interpreter::abortExecution() {
    m_abortExecution = true;
}


void Interpreter::clear() {
    m_programStream.clear();
    m_programStream.str(string());

    m_cells.clear();
    m_cells.push_back(0);
    m_currentCell = m_cells.begin();

    m_jumps.clear();
}


bool Interpreter::execute() {
    if (!m_programValid) {
        m_outputStream << "Program is not valid! Aborting" << endl;
        return false;
    }

    m_abortExecution = false;
    m_programStream.clear();
    m_programStream.seekg(0, m_programStream.beg);

    int character = m_programStream.get();
    while (character != EOF) {
        execute(character);
        character = m_programStream.get();

        if (m_abortExecution)
            return false;
    }

    return true;
}


void Interpreter::execute(int command) {
    switch (command) {
    case '>': {
        if ((m_currentCell+1) == m_cells.end()) {
            m_cells.push_back(0);
        }
        ++m_currentCell;
        break;
    }
    case '<': {
        if (m_currentCell == m_cells.begin()) {
            m_cells.push_front(0);
        }
        --m_currentCell;
        break;
    }
    case '+': {
        (*m_currentCell)++;
        break;
    }
    case '-': {
        (*m_currentCell)--;
        break;
    }
    case '.': {
        //m_outputStream << (char)*m_currentCell << flush;
        //m_outputStream << *m_currentCell << " " << flush;
        emit(put(*m_currentCell));
        break;
    }
    case ',': {
        m_outputStream << endl << "? " << flush;
        int c = m_inputStream.get();
        m_inputStream.clear();

        if ((char)c == '\n') {
            c = 0;
        } else {
            m_inputStream.ignore(INT_MAX, '\n');
        }
        (*m_currentCell) = c;
        break;
    }
    case '[': {
        if (*m_currentCell == 0) {
            m_programStream.seekg(m_jumps[m_programStream.tellg()]);
        }
        break;
    }
    case ']': {
        m_programStream.seekg(m_jumps[m_programStream.tellg()]-1);
        break;
    }
    }
}


bool Interpreter::isProgramValid() {
    return m_programValid;
}


bool Interpreter::load(string fileName) {
    m_fileName = fileName;

    fstream fileStream(m_fileName.c_str());
    if (!fileStream.is_open()) {
        m_programValid = false;
        return false;
    } else {
        stringstream stream;
        stream << fileStream.rdbuf();
        fileStream.close();
        return load(stream);
    }
}


bool Interpreter::load(std::stringstream &programStream) {
    clear();
    m_programValid = true;
    m_programStream << programStream.rdbuf();

    int character = m_programStream.get();
    stack<unsigned int> openBraces;
    while (character != EOF) {
        if (character == '[') {
            openBraces.push(m_programStream.tellg());
        } else if (character == ']') {
            if (openBraces.empty()) {
                m_outputStream << "Error at position " << m_programStream.tellg() << ": ";
                m_outputStream << "Unmatched parenthesis" << endl;
                m_programValid = false;
                return false;
            } else {
                unsigned int destination = m_programStream.tellg();
                unsigned int origin = openBraces.top();
                openBraces.pop();
                m_jumps[origin] = destination;
                m_jumps[destination] = origin;
            }
        }
        character = m_programStream.get();
    }

    if (openBraces.empty()) {
        return true;
    } else {
        m_outputStream << "Error: There are unclosed parentheses" << endl;
        return false;
    }
}


void Interpreter::printCells() {
    m_outputStream << "Cells:" << endl;
    for (unsigned int i = 0; i < m_cells.size(); ++i) {
        m_outputStream << m_cells[i];
    }
    m_outputStream << endl;
}


string Interpreter::toCppCode() {
    if (!m_programValid) {
        m_outputStream << "Error: No valid program loaded! Aborting" << endl;
        return "";
    }

    stringstream stream;

    m_programStream.clear();
    m_programStream.seekg(0, m_programStream.beg);

    unsigned int level = 0;
    int character = m_programStream.get();
    while (character != EOF) {
        if (character == ']') {
            level--;
        }

        for (unsigned int i = 0; i < level; ++i) {
            stream << "  ";
        }

        switch (character) {
        case '>': { stream << "++ptr;" << endl;  break; }
        case '<': { stream << "--ptr;" << endl;  break; }
        case '+': { stream << "++(*ptr);" << endl;  break; }
        case '-': { stream << "--(*ptr);" << endl;  break; }
        case '.': { stream << "putchar(*ptr)" << endl;  break; }
        case ',': { stream << "(*ptr) = getchar()" << endl;  break; }
        case '[': { stream << "while (*ptr) {" << endl; level++; break; }
        case ']': { stream << "}" << endl; break; }
        }

        character = m_programStream.get();
    }

    return stream.str();
}
