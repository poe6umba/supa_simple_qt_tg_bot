#pragma once
#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QThread>
#include <QUrl>
#include <QFile>
#include <QJsonObject>

//poe6umba 2023

/*
How does it work

1) create an object
2) call startThread
3) the update_bot() function start work in a separate thread.
    thread get last update id, parse messages
    after parsing incoming messages from users
    the thread calls two handlers: for the command or for the callback button

    By changing the behavior of the handlers, you can completely change the logic of the bot

that all, gl

*/
class baseTelegramWorker : public QObject
{
    Q_OBJECT
public:
    baseTelegramWorker(const QString token);
    ~baseTelegramWorker();
    QString apiUrl;
    QNetworkAccessManager* manager;

    //sry for this :D
    QString cube="\U0001F3B2", crown = "\U0001F451", thumbsUp = "\U0001F44D";
    QString person = "\U0001F9D1";
    QString smile = "\U0001F60A";
    QString heartEyes = "\U0001F60D";
    QString laughing = "\U0001F606";
    QString kissing = "\U0001F618";
    QString crying = "\U0001F62D";
    QString skull = "\U00002620";
    QString bomb = "\U0001F4A3";
    QString robot = "\U0001F916";
    QString fire = "\U0001F525";
    QString rocket = "\U0001F680";
    QString rock = "\U0000270A";
    QString paper = "\U0000270B";
    QString scissors = "\U0000270C";

public slots:
    void start_thread();
    void stop_thread();

    //thread function
    void update_bot();

    //methods for sending all sorts of stuff to the user
    //chat_id and callback_id you can get from handles
    void send_reply(QString chat_id, QString text);
    void send_callback_response(QString callback_id, QString text);
    ///no more than 20 MB
    void send_videofile(QString chat_id, const QString& videoFilePath);
    void send_image(QString chat_id, const QString& imageFilePath);
    //callback_tag needed for handle_callback
    void send_reply_with_inline_keyboard(QString chat_id, QString text, QStringList buttons_names, QString callback_tag);
    //two buttons in row
    void send_reply_with_reply_keyboard(QString chat_id, QString text, QStringList buttons_names);

private:
    QString mToken;

    bool mRunning;

    /*
    This functions will determine the latest update_id and save it locally in a JSON file,
    so that after a restart the bot will process the latest messages, and not all of them.
    */
    void load_last_update_id();
    void save_last_update_id();

    int mLastUpdateId = 0;


    //command - is exactly a command, what we write through /, if there is no command in the message, then “invalid” will come (you can replace "invalid" for something else in update_bot())
    //argument - what comes after the command, it is also just text if the user sent a simple message
    //for example:
    //if user send "/command 12345", you get command = "command" argument = "12345"
    //if user send "command 12345" you get command = "invalid", argument = "command 12345"
    void virtual handle_command(QString command, QString argument, QString chat_id, QString user_id);
    void virtual handle_callback(QString callback_data, QString callback_id,  QString chat_id, QString user_id);
};
