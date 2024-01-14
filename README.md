# supa_simple_qt_tg_bot
just 2 files  
h and cpp file  (*it not even lib* 😱)  
and this is enough to work with the Telegram API  
![1](https://github.com/poe6umba/supa_simple_qt_tg_bot/assets/24863642/80161d27-988d-4311-bb36-b05cd6a84e1a)  

# What can it do
* Сan send messages, pictures, videos
* Сan send buttons with or without callbacks and react on this


# How to use
Add ```basetelegramworker.h``` and ```basetelegramworker.сpp``` to your project

After that you can make a new class based on *basetelegramworker*  
And override the handle methods

Or just override the handle methods in ```basetelegramworker.cpp```

# Example
This project is already an example  
just write your token in main.c and run
(checked in xubuntu 22.04, Qt version 5.15.3)

# Example of using a bot
CI/CD notifier bot (run on build machine)  
![photo_2023-09-29_17-36-38](https://github.com/poe6umba/supa_simple_qt_tg_bot/assets/24863642/eb608130-a7d6-4141-82ca-423aa9122943)  

This bot send message from GSC program, when user press button on program  
![3](https://github.com/poe6umba/supa_simple_qt_tg_bot/assets/24863642/ccd53cab-d43e-409d-a879-8b62869ef024)  

This bot get video from usb-camera and save it. User can get video or image.  
![2](https://github.com/poe6umba/supa_simple_qt_tg_bot/assets/24863642/7155ed7c-e9ba-495d-87a0-70c30b55873d)
![4](https://github.com/poe6umba/supa_simple_qt_tg_bot/assets/24863642/04371630-c878-46e5-b57e-660e5697de09)
