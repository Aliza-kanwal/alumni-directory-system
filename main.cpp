#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <csignal>
#include <QDebug>

// Platform-specific crash handling
#ifdef Q_OS_UNIX
#include <execinfo.h>
#include <unistd.h>
#endif

#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>
#endif

void crashHandler(int signum) {
#ifdef Q_OS_UNIX
    // Linux/Mac implementation
    void* array[50];
    size_t size = backtrace(array, 50);
    fprintf(stderr, "Error: signal %d:\n", signum);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
#endif

#ifdef Q_OS_WIN
    // Windows implementation
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    void* stack[50];
    unsigned short frames = CaptureStackBackTrace(0, 50, stack, NULL);
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    fprintf(stderr, "Error: signal %d\n", signum);
    for (unsigned int i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)stack[i], 0, symbol);
        fprintf(stderr, "%i: %s - 0x%llX\n", frames - i - 1, symbol->Name, (unsigned long long)symbol->Address);
    }
    free(symbol);
#endif

    exit(1);
}

int main(int argc, char *argv[])
{
    // Register crash handlers
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);
    signal(SIGFPE, crashHandler);

    QApplication a(argc, argv);

    // Translation setup
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Alumni_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
