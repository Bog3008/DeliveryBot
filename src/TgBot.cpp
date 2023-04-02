//
// Created by Bogdan on 02.04.2023.
//

#include "TgBot.h"
#include <Publisher.h>
void tg_bot_run(){
    std::string topic = "test/t1";
    std::string host = "localhost";
    int port = 1883;

    std::string token("5868645498:AAGhAYq2ApK7ghZipHZqiKyRVEBRY0L0r5o");
    printf("Token: %s\n", token.c_str());

    Bot bot(token);

    // Thanks Pietro Falessi for code
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    std::vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    checkButton->text = "Start DelBot";
    checkButton->callbackData = "start bot";
    //
    std::vector<InlineKeyboardButton::Ptr> row1;
    InlineKeyboardButton::Ptr check1Button(new InlineKeyboardButton);
    check1Button->text = "Stop DelBot";
    check1Button->callbackData = "stop bot";
    //
    row0.push_back(checkButton);
    row1.push_back(check1Button);
    keyboard->inlineKeyboard.push_back(row0);
    keyboard->inlineKeyboard.push_back(row1);

    bot.getEvents().onCommand("start", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!", false, 0, keyboard);
    });
    bot.getEvents().onCommand("check", [&bot, &keyboard](Message::Ptr message) {
        std::string response = "ok";
        bot.getApi().sendMessage(message->chat->id, response, false, 0, keyboard, "Markdown");
    });
    //
    bot.getEvents().onCallbackQuery([&bot, &keyboard, &host, &port, &topic](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "start bot")) {
            std::string response = "DelBot was started";
            MPublisher Publisher(host, port);
            Publisher.publish(topic, response);
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
    });
    bot.getEvents().onCallbackQuery([&bot, &keyboard, &host, &port, &topic](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "stop bot")) {
            std::string response = "DelBot was stopped";
            MPublisher Publisher(host, port);
            Publisher.publish(topic, response);
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception& e) {
        printf("error: %s\n", e.what());
    }
}