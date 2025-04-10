![Voice chat ui image](vc_ui.png "Voice chat ui image")

# Voice chat system
Due to unforeseen(but expected) circumstances -such as a very hungry little boy reselling my work- I have decided to make this version public.


### This version's features:
- Proximity chat (local)
- Guild chat (in-map)
- Party chat (in-map)
- Global/Master volume
- Per-speaker volume handling (with the ability to pin speakers at the top of the list)
- Configurable sampling frequency and channels
(I still recommend sticking to 32kHz and 1 channel because of packet sizes.)
- You can not speak over a channel unless hearing is also enabled
- Sound reduction to prevent loud sounds from blasting your ears
- Basic but functional sound detection to avoid unnecessary packets
- FLAC encoding/decoding to reduce packet sizes wherever possible
- ?? Fuck knows, I can't remember.


### Notes:
- M2's networking doesn't handle partial packets.
    - You'll have to either:
        - Disable `IMPROVED_PACKET_ENCRYPTION`, or 
        - Implement that part yourself(I won't help you with this, don't DM me.)

- Don't bother with pull requests, this repo was released out of necessity, it won't receive any updates.
    - If you want to make changes, feel free to fork it.

- The system has been live for over a year on multiple servers and tested by thousands of users.
    - If it doesn't work on your server, you did something wrong.


#### QFlags
- `/e voice_chat_disable 1/0` to enable/disable
- `/e voice_chat_hear_yourself 1/0` to be able to hear yourself(only on test_server)


### Technologies used:
- [Miniaudio - An audio playback and capture library for C and C++](https://miniaud.io/)
- [FLAC - Free losless audio codec](https://xiph.org/flac/index.html)


All of my customers will receive an update with more features soon.
This version received its last update on `28.01.2025`
Good luck.


Forums/Topics:
- [Metin2Dev](https://metin2.dev/topic/33630-voice-chat-system/)
- [Metin2Forum](https://metin2.forum/post5105.html#p5105)
- [Just4Metin](https://just4metin.ro/viewtopic.php?p=833110#p833110)
- [TurkMMO](https://forum.turkmmo.com/konu/3917308-voice-chat-system/)


If you'd like to add your forum here, DM me on discord at `amun3808`
