#include <QCoreApplication>
#include "telegramsender.h"
#include <QDir>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //1) set_token
    QString your_token = "6549080077:AAGzVLEPQb-hPn27Qif0C5sroPr_y0SpidU";

    //2) make tg_bot object
    TelegramSender *tg_sender;
    tg_sender = new TelegramSender(your_token);

    //3) run and enjoy
    tg_sender->startThread();

    return a.exec();
}
