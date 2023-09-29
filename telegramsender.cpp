#include "telegramsender.h"

TelegramSender::TelegramSender(QString token)
    : baseTelegramWorker(token)
{
}

TelegramSender::~TelegramSender()
{

}

void TelegramSender::handle_command(QString command, QString argument, QString chat_id, QString user_id)
{
    QStringList buttons_names;
    buttons_names.clear();

    if(command == "start")
    {
            send_reply_with_inline_keyboard(chat_id,"Hi! Im supa simple tg bot by poe6umba" + robot + "\nhttps://github.com/poe6umba/supa_simple_qt_tg_bot", QStringList{"Test Callback Button"}, "callback_tag");
    }
    if(command == "contact")
    {
            send_reply(chat_id, "https://github.com/poe6umba/supa_simple_qt_tg_bot");
    }

    if(command == "invalid")
    {
        //you can respond not only to commands, but also to simple text
        if(argument == "Cool")
        {
           send_reply(chat_id, "Demonstration is over, thank" + fire);
        }
        else
        {
            send_reply(chat_id, "You send message, not command" + crying);
        }

    }
}

void TelegramSender::handle_callback(QString callback_data, QString callback_id, QString chat_id, QString user_id)
{

    if(callback_data.contains("callback_tag"))
    {
      send_reply_with_reply_keyboard(chat_id, "Got callback!\nAnd here is a simple keyboard, check it", QStringList{"Cool"});
    }

    //unblock callback button
    send_callback_response(callback_id, "Get and close you callback" + heart_eyes);

}
