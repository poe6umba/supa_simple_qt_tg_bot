#include "basetelegramworker.h"


baseTelegramWorker::baseTelegramWorker(QString token_)
{
    token = token_;
    manager = new QNetworkAccessManager(this);
    api_url = "https://api.telegram.org/bot" + token;
    is_running = false;
}

baseTelegramWorker::~baseTelegramWorker()
{

}

void baseTelegramWorker::startThread()
{
    if (!is_running) {
        is_running = true;
        update_bot();
    }
}

void baseTelegramWorker::stopThread()
{
    if (is_running) {
        is_running = false;
    }
}

void baseTelegramWorker::load_last_update_id()
{
    QFile file("last_update_id.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        last_update_id = doc.object()["last_update_id"].toInt();
        file.close();
    }
}

void baseTelegramWorker::save_last_update_id()
{
    QFile file("last_update_id.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject obj;
        obj["last_update_id"] = last_update_id;
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    }
}

void baseTelegramWorker::send_reply(QString chat_id, QString text)
{
    QUrl url(api_url + "/sendMessage");
    QUrlQuery query;
    query.addQueryItem("chat_id", chat_id);
    query.addQueryItem("text", text);

    QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = manager->post(request, data);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reply->deleteLater();
}

void baseTelegramWorker::send_callback_response(QString callback_id, QString text)
{
    QUrl url(api_url + "/answerCallbackQuery");
    QUrlQuery query;
    query.addQueryItem("callback_query_id", callback_id);
    if(!text.isEmpty()) query.addQueryItem("text", text); // text answer for callback button press(if empty not sending)
    query.addQueryItem("show_alert", "false");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkReply* reply = manager->post(request, data);

    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error sending callback response:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void baseTelegramWorker::send_reply_with_inline_keyboard(QString chat_id, QString text, QStringList buttons_names, QString callback_tag)
{
    QUrl url(api_url + "/sendMessage");
    QUrlQuery query;
    query.addQueryItem("chat_id", chat_id);
    query.addQueryItem("text", text);

    QJsonArray inlineKeyboard;

    int i = 0;
    for (const QString& Item : buttons_names)
    {

        const QString& Name = Item;
        QJsonObject button;
        button.insert("text", Name);

        QJsonObject inlineButton;
        inlineButton.insert("text", Name);
        //add callback tag for handle_callback
        inlineButton.insert("callback_data", callback_tag + QString::number(i));

        QJsonArray keyboardRow;
        keyboardRow.append(inlineButton);

        inlineKeyboard.append(keyboardRow);
        i++;
    }

    QJsonObject reply_keyboard;
    reply_keyboard.insert("inline_keyboard", inlineKeyboard);

    QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();
    data += "&reply_markup=" + QUrl::toPercentEncoding(QJsonDocument(reply_keyboard).toJson());

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = manager->post(request, data);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reply->deleteLater();
}

void baseTelegramWorker::send_reply_with_reply_keyboard(QString chat_id, QString text, QStringList buttons_names)
{
    QUrl url(api_url + "/sendMessage");
    QUrlQuery query;
    query.addQueryItem("chat_id", chat_id);
    query.addQueryItem("text", text);

        QJsonArray keyboardRows;
        QJsonArray currentRow;

         for (const QString& Item : buttons_names)
        {
            const QString& Name = Item;
            QJsonObject button;
            button.insert("text", Name);

            currentRow.append(button);

            // 2 buttons in row
            if (currentRow.size() == 2)
            {
                keyboardRows.append(currentRow);
                currentRow = QJsonArray(); // make new row
            } else
            {
                keyboardRows.append(currentRow);
            }
        }

        QJsonObject reply_keyboard;
        reply_keyboard.insert("keyboard", keyboardRows);
        reply_keyboard.insert("one_time_keyboard", true);
        reply_keyboard.insert("resize_keyboard", true);

        QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();
        data += "&reply_markup=" + QUrl::toPercentEncoding(QJsonDocument(reply_keyboard).toJson());

    // Отправляем запрос на сервер Телеграма
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply* reply = manager->post(request, data);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reply->deleteLater();
}

void baseTelegramWorker::send_videofile(QString chat_id, const QString& videoFilePath)
{
    QUrl url(api_url + "/sendVideo");
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart chatIdPart;
    chatIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"chat_id\""));
    chatIdPart.setBody(chat_id.toUtf8());

    QHttpPart videoPart;
    videoPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("video/mp4"));
    videoPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"video\"; filename=\"video.mp4\""));

    QFile *videoFile = new QFile(videoFilePath);
    videoFile->open(QIODevice::ReadOnly);
    videoPart.setBodyDevice(videoFile);
    videoFile->setParent(multiPart);

    multiPart->append(chatIdPart);
    multiPart->append(videoPart);

    QNetworkRequest request(url);
    QNetworkReply* reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reply->deleteLater();
}

void baseTelegramWorker::send_image(QString chat_id, const QString& imageFilePath)
{
    QUrl url(api_url + "/sendPhoto");
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart chatIdPart;
    chatIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"chat_id\""));
    chatIdPart.setBody(chat_id.toUtf8());

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg")); //if not JPEG, change type content
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"photo\"; filename=\"image.jpg\""));

    QFile *imageFile = new QFile(imageFilePath);
    imageFile->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(imageFile);
    imageFile->setParent(multiPart);

    multiPart->append(chatIdPart);
    multiPart->append(imagePart);

    QNetworkRequest request(url);
    QNetworkReply* reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    reply->deleteLater();
}

void baseTelegramWorker::handle_command(QString command, QString arguments, QString chat_id, QString user_id)
{
    send_reply(chat_id, "Base handle command. You say -  command:" + command + ", arguments: " + arguments);
}

void baseTelegramWorker::handle_callback(QString callback_data, QString callback_id, QString chat_id, QString user_id)
{
    send_reply(chat_id, callback_data + " CALLBACK");

    //unblock callback button
    send_callback_response(callback_id, "Callback success!");
}

void baseTelegramWorker::update_bot() {

    while (is_running) {
        load_last_update_id();
        // make request Telegram API with offset from JSON
        QUrl url(api_url + "/getUpdates");
        QUrlQuery query;
        query.addQueryItem("offset", QString::number(last_update_id + 1));
        query.addQueryItem("timeout", "30");
        url.setQuery(query);
        QNetworkRequest request(url);

        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error getting updates:" << reply->errorString();
            reply->deleteLater();
            continue;
        }

        QByteArray data = reply->readAll();
        reply->deleteLater();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        if (obj["ok"].toBool()) {
            QJsonArray updates = obj["result"].toArray();
            for (auto update : updates) {
                if (update.isObject()) {
                    QJsonObject updateObject = update.toObject();
                    if (updateObject.contains("message")) {
                        //Parse text message
                        QJsonObject message = updateObject["message"].toObject();
                        QString text = message["text"].toString().trimmed();
                        QJsonObject chatObject = message["chat"].toObject();
                        QString chat_id = QString::number(chatObject["id"].toVariant().toULongLong());
                        QJsonObject fromObject = message["from"].toObject();
                        QString user_id = QString::number(fromObject["id"].toVariant().toULongLong());

                        if (!text.isEmpty() && text.startsWith('/')) {
                            QStringList command_parts = text.split(' ', QString::SkipEmptyParts);
                            QString command_name = command_parts[0].mid(1);

                            QString arguments;
                            if (command_parts.size() > 1) {
                                arguments = command_parts.mid(1).join(' ');
                            }
                            handle_command(command_name, arguments, chat_id, user_id);
                        } else if (!text.isEmpty()) {
                            //mb send invalid is not the best solution when there is no command in message
                            handle_command("invalid", text, chat_id, user_id);
                        }
                    } else if (updateObject.contains("callback_query")) {
                        // Parse callback
                        QJsonObject callbackQuery = updateObject["callback_query"].toObject();
                        QString callback_data = callbackQuery["data"].toString();
                        QString callback_id = callbackQuery["id"].toString();
                        QJsonObject message = callbackQuery["message"].toObject();
                        QJsonObject chatObject = message["chat"].toObject();
                        QString chat_id = QString::number(chatObject["id"].toVariant().toULongLong());
                        QJsonObject fromObject = callbackQuery["from"].toObject();
                        QString user_id = QString::number(fromObject["id"].toVariant().toULongLong());          
                        handle_callback(callback_data, callback_id, chat_id, user_id);
                    }
                }

                // Update offset in JSON
                last_update_id = update.toObject()["update_id"].toInt();
                save_last_update_id();
            }
        }
    }
}

