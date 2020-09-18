Abandoned project as I discover Kruiz Control

# ESPTwitchChatbot

For triggering overlays with specific bit amounts, alert actions etc.

This is used with an ESP8266 board in conjunction with OBS Chatbot and OBS Streamlabs.
It relies on Twitch_IRC_API https://github.com/jacob1421/Twitch-IRC-API

The plan for this code is to no longer require Streamlabs alerts

Chatbot scripts required:
OBS Studio Remote Parameters 
CLP (Castorr's Lazy Pack) (If you want to send text to a html overlay on the screen).

**( Or SLOBS Remote Parameters if you are using SLOBS, if this is the case the Scene management code is slightly different, you can find what you need in the Readme file for the script)



**********This is a WIP**********

Still need to test the following 
Subscribers
Re-subscribers (currently crashes)
Prime Sibscribers (seems ok, will need to double check)
Tiered Sub actions - Pending subs)
Raids (nothing shows to screen)

*** Tested ***
Queued BITS
Queued Follows
Queued and instant Rewards


This is my current overlay menu, these can all be trigger with Bits and I use this code to firstly put data in an array, wait for time (30 seconds by default) to pass and perform action on the next item in the array.

https://rb.gy/ce5g4b
