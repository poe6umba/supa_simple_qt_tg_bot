#include <QCoreApplication>
#include "telegramsender.h"
#include <QDir>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //1) set_token
    QString your_token = "your token";

    //2) make tg_bot object
    TelegramSender *tgSender;
    tgSender = new TelegramSender(your_token);

    //3) run and enjoy
    tgSender->start_thread();

    return a.exec();
}
