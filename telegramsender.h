#pragma once
#include <QObject>
#include "basetelegramworker.h"


//This is an example class
//here I redefined the handlers to get the program logic I need

class TelegramSender : public baseTelegramWorker
{
    Q_OBJECT
public:
    TelegramSender(QString token);
    ~TelegramSender();


public slots:

    void handle_command(QString command, QString argument, QString chat_id, QString user_id) override;
    void handle_callback(QString callback_data, QString callback_id,  QString chat_id, QString user_id) override;


};

