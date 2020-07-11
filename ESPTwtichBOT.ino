#include "bot_credentials.h"
#include <ESP8266WiFi.h>
#include <twitch_irc_api.h>
#include <memory_constraints.h>

//It will upload for ESP with Silver shielding however does not run or show errors IDKY
//IF IT IS NOT WORKING, CHECK THE USERNAME / PASSWORD IS BEING SENT FROM BOT CREDS
//You may see boot error(3,6 or 7 if this is the case)

struct {
  char Type[10];
  char Username[30];
  unsigned int Aamount;
       }
Alertdata[20];


int switchPin = 2; //GPI04 D2
bool Paused;
unsigned long startMillis; 
unsigned long previousMillis;
const unsigned long period = 30000;
int Writesanity = 400;
int Nextread;
int Nextwrite;

//Twitch Api Instance
TwitchWebsocketIRC twitch_api;

const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password

/* Generic Callback Functions */
void onClearChatCallback(ClearChat data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=CLEARCHAT=\nChannel Name: %s\nUsername: %s\nBan Duration: %i\n", data.channel_name, data.username, data.ban_duration);
  printf("\n---------------------------------------------------\n");
}
void onClearMsgCallback(ClearMsg data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=CLEARMSG=\nChannel Name: %s\nLogin: %s\nMessage: %s\nTargetMsgId: %s\n", data.channel_name, data.login, data.message, data.target_msg_id);
  printf("\n---------------------------------------------------\n");
}
void onNoticeCallback(Notice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=NOTICE=\nChannel Name: %s\nMsg-id: %s\nMessage: %s\n", data.channel_name, data.msg_id, data.message);
  printf("\n---------------------------------------------------\n");
}
void onGlobalUserStateCallback(GlobalUserState data) {
  printf("\n---------------------------------------------------\n");
  int count = 0;
  printf("\n=GLOBALUSERSTATE=\n");
  printf("\nBadges Info\n");
  while (data.badge_info[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
    count += 1;
  }
  count = 0;
  printf("\nBadges\n");
  while (data.badges[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
    count += 1;
  }
  count = 0;
  printf("\nEmote Sets\n");
  while (data.emote_sets[count].emote_id != 0 || count == 0) {
    printf("\t-Emote Id: %i\n", data.emote_sets[count].emote_id);
    count += 1;
  }
  printf("Color: %s\nDisplay Name: %s\nUser Id: %i\n",
    data.color, data.display_name, data.user_id);
  printf("\n---------------------------------------------------\n");
}
void onPrivMsgCallback(PrivMsg data) {
  printf("\n---------------------------------------------------\n");
  int count = 0;
  printf("\n=PRIVMSG=\n");
  printf("\nBadges Info\n");

/////////////////////////FOLLOW////////////////////////

if (strstr(data.message, "following") != NULL && (strcmp(data.display_name, "StreamElements") == 0)) {

strcpy(Alertdata[Nextwrite].Type,"Follow");
strcpy(Alertdata[Nextwrite].Username,data.display_name);
Alertdata[Nextwrite].Aamount = 0;
delay(Writesanity);
Next(&Nextwrite);

}

////////////////SUB??????????? *MAY NOT WORK
if (strstr(data.message, "subscribed") != NULL && (strcmp(data.display_name, "StreamElements") == 0)) {

strcpy(Alertdata[Nextwrite].Type,"Sub");
strcpy(Alertdata[Nextwrite].Username,data.display_name);
Alertdata[Nextwrite].Aamount = 0;
delay(Writesanity);
Next(&Nextwrite);

}

////////////////RESUB??????????? *MAY NOT WORK
if (strstr(data.message, "resubscribed") != NULL && (strcmp(data.display_name, "StreamElements") == 0)) {

strcpy(Alertdata[Nextwrite].Type,"Resub");
strcpy(Alertdata[Nextwrite].Username,data.display_name);
Alertdata[Nextwrite].Aamount = 0;
delay(Writesanity);
Next(&Nextwrite);

}


  
  while (data.badge_info[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
    count += 1;
  }
  count = 0;
  printf("\nBadges\n");
  while (data.badges[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
    count += 1;
  }
  count = 0;
  printf("\nEmote Chat Set\n");
  while (data.emotes[count].start_index_location != 0 && data.emotes[count].end_index_location != 0) {
    printf("\t-Emote Id: %i\n", data.emotes[count].emote.emote_id);
    printf("\t-Emote First Index: %i\n", data.emotes[count].start_index_location);
    printf("\t-Emote Second Index: %i\n", data.emotes[count].end_index_location);
    count += 1;
  }
  printf("Bits: %i\nColor: %s\nDisplay Name: %s\nId: %s\nMessage: %s\nMod: %i\nRoom Id: %i\nTmi Sent Ts: %s\nUser Id: %i\nChannel Name: %s\n",
    data.bits, data.color, data.display_name, data.id, data.message, data.mod, data.room_id, data.tmi_sent_ts, data.user_id, data.channel_name);
  printf("\n---------------------------------------------------\n");
}
void onRoomStateCallback(RoomState data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=ROOMSTATE=\n");
  printf("Emote Only: %i\nFollowers Only: %i\nR9K: %i\nSlow: %i\nSubs Only: %i\nChannel Name: %s\n",
    data.emote_only, data.followers_only, data.r9k, data.slow, data.subs_only, data.channel_name);
  printf("\n---------------------------------------------------\n");
}
void onUserNoticeCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=USERNOTICE=\n");
  printf("\nBadges Info\n");
  int count = 0;
  while (data.badge_info[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
    count += 1;
  }
  count = 0;
  printf("\nBadges\n");
  while (data.badges[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
    count += 1;
  }
  count = 0;
  printf("\nEmote Chat Set\n");
  while (data.emotes[count].start_index_location != 0 && data.emotes[count].end_index_location != 0){
    printf("\t-Emote Id: %i\n", data.emotes[count].emote.emote_id);
    printf("\t-Emote First Index: %i\n", data.emotes[count].start_index_location);
    printf("\t-Emote Second Index: %i\n", data.emotes[count].end_index_location);
    count += 1;
  }
  printf("Color: %s\nDisplay Name: %s\nId: %s\nLogin: %s\nMessage: %s\nMod: %i\nMsg_Id: %s\nRoom_Id: %i\nSystem_Msg: %s\nTmi Sent Ts: %s\nUser_Id: %i\nChannel_Name: %s\n",
    data.color, data.display_name, data.id, data.login, data.message, data.mod, data.msg_id, data.room_id, data.system_msg, data.tmi_sent_ts, data.user_id, data.channel_name);
  printf("\nmsg_param_cumulative_months: %i\nmsg_param_display_name: %s\nmsg_param_login: %s\nmsg_param_months: %i\nmsg_param_promo_gift_total: %i\n",
    data.msg_param_cumulative_months, data.msg_param_display_name, data.msg_param_login, data.msg_param_months, data.msg_param_promo_gift_total);
  printf("\nmsg_param_promo_name: %s\nmsg_param_recipient_display_name: %s\nmsg_param_recipient_id: %i\nmsg_param_recipient_user_name: %s\nmsg_param_sender_login: %s\nmsg_param_sender_name: %s\n",
    data.msg_param_promo_name, data.msg_param_recipient_display_name, data.msg_param_recipient_id, data.msg_param_recipient_user_name, data.msg_param_sender_login, data.msg_param_sender_name);
  printf("\nmsg_param_should_share_streak: %i\nmsg_param_streak_months: %i\nmsg_param_sub_plan: %i\nmsg_param_sub_plan_name: %s\nmsg_param_viewerCount: %i\nmsg_param_ritual_name: %s\nmsg_param_threshold: %i\n",
    data.msg_param_should_share_streak, data.msg_param_streak_months, data.msg_param_sub_plan, data.msg_param_sub_plan_name, data.msg_param_viewerCount, data.msg_param_ritual_name, data.msg_param_threshold);
  printf("\n---------------------------------------------------\n");
}
void onUserStateCallback(UserState data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=USERSTATE=\n");
  printf("\nBadges Info\n");
  int count = 0;
  while (data.badge_info[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badge_info[count].name, data.badge_info[count].version);
    count += 1;
  }
  count = 0;
  printf("\nBadges\n");
  while (data.badges[count].name != NULL) {
    printf("\t-Badge Name: %s Version: %i\n", data.badges[count].name, data.badges[count].version);
    count += 1;
  }
  count = 0;
  printf("\nEmote Sets\n");
  while (data.emote_sets[count].emote_id != 0 || count == 0) {
    printf("\t-Emote Id: %i\n", data.emote_sets[count].emote_id);
    count += 1;
  }
  printf("Color: %s\nDisplay Name: %s\nMod: %i\nChannel Name: %s\n",
    data.color, data.display_name, data.mod, data.channel_name);
  printf("\n---------------------------------------------------\n");
}
void onHostTargetCallback(HostTarget data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=HOSTTARGET=\n");
  printf("Hosting Channel Name: %s\nNumber Of Viewers: %i\nChannel Name: %s\n",
    data.hosting_channel_name, data.number_of_viewers, data.channel_name);
  printf("\n---------------------------------------------------\n");
}

/* Custom Callbacks */
void onChannelHostedCallback(Hostee data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=CHANNEL HOSTED=\n");
  printf("Hostee Display Name: %s\nViewers: %i\n", data.display_name, data.viewer_count);
  printf("\n---------------------------------------------------\n");
}
void onBitsCallback(PrivMsg data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=BITS=\n");
  printf("Bit Donater Name: %s\nBits: %i\n", data.display_name, data.bits);
  printf("\n---------------------------------------------------\n");


strcpy(Alertdata[Nextwrite].Type,"Bits");
strcpy(Alertdata[Nextwrite].Username,data.display_name);
Alertdata[Nextwrite].Aamount = data.bits;
delay(Writesanity);
Next(&Nextwrite);

  
}
void onSubCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=SUB=\n");
  printf("Subscribers Name: %s\nTotal Months Subbed: %i\nSub Streak: %i\nSub Type: %i\nSub Plan Name: %s\nShare Sub Streak: %i\n",
    data.display_name, data.msg_param_cumulative_months, data.msg_param_streak_months, data.msg_param_sub_plan, data.msg_param_sub_plan_name, data.msg_param_should_share_streak);
  printf("\n---------------------------------------------------\n");
}
void onReSubCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=RESUB=\n");
  printf("Subscribers Name: %s\nTotal Months Subbed: %i\nSub Streak: %i\nSub Type: %i\nSub Plan Name: %s\nShare Sub Streak: %i\n",
    data.display_name, data.msg_param_cumulative_months, data.msg_param_streak_months, data.msg_param_sub_plan, data.msg_param_sub_plan_name, data.msg_param_should_share_streak);
  printf("\n---------------------------------------------------\n");

}
void onSubGiftCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=SUB GIFT=\n");
  printf("Gifters Name: %s\nTotal Months Subbed: %i\nRecipient Display Name: %s\nRecipient User Id: %i\nRecipient Username: %s\nSub Plan:%i\nSub Plan Name: %s\n",
    data.display_name, data.msg_param_months, data.msg_param_recipient_display_name, data.msg_param_recipient_id, data.msg_param_recipient_user_name, data.msg_param_sub_plan, data.msg_param_sub_plan_name);
  printf("\n---------------------------------------------------\n");
}
void onAnonSubGiftCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=ANON SUB GIFT=\n");
  printf("Total Months Subbed: %i\nRecipient Display Name: %s\nRecipient User Id: %i\nRecipient Username: %s\nSub Plan:%i\nSub Plan Name: %s\n",
    data.msg_param_months, data.msg_param_recipient_display_name, data.msg_param_recipient_id, data.msg_param_recipient_user_name, data.msg_param_sub_plan, data.msg_param_sub_plan_name);
  printf("\n---------------------------------------------------\n");
}
void onSubMysteryGiftCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=SUB MYSTERY GIFT=\n");
  printf("\n---------------------------------------------------\n");
}
void onGiftPaidUpgradeCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=GIFT PAID UPGRADE=\n");
  printf("Number of gifts subs given: %i\n Sub Promo Name: %s\n Gifters Login: %s\n Gifters Display Name: %s\n",
    data.msg_param_promo_gift_total, data.msg_param_promo_name, data.msg_param_sender_login, data.msg_param_sender_name);
  printf("\n---------------------------------------------------\n");
}
void onRewardGiftCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=REWARD GIFT=\n");
  printf("\n---------------------------------------------------\n");
}
void onAnonGiftPaidUpgradeCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=ANON GIFT PAID UPGRADE=\n");
  printf("Number of gifts subs given: %i\n Sub Promo Name: %s\n",
    data.msg_param_promo_gift_total, data.msg_param_promo_name);
  printf("\n---------------------------------------------------\n");
}
void onRaidCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=RAID=\n");
  printf("Raider Display Name: %s\nRaider Channel Name: %s\nViewer Count: %i\n",
    data.msg_param_display_name, data.msg_param_login, data.msg_param_viewerCount);
  printf("\n---------------------------------------------------\n");

      twitch_api.send_chat_message("/w drpuppetmaster !raid"); 

  
}
void onUnRaidCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=UNRAID=\n");
  printf("\n---------------------------------------------------\n");
}
void onRitualCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=RITUAL=\n");
  printf("Name of ritual: %s\n", data.msg_param_ritual_name);
  printf("\n---------------------------------------------------\n");
}
void onBitsBadgeTierCallback(UserNotice data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=BITS BADGE TIER=\n");
  printf("Bits Badge Tier: %i", data.msg_param_threshold);
  printf("\n---------------------------------------------------\n");
}
void onHostStartCallback(HostTarget data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=HOST START=\n");
  printf("Hosting Channel Name: %s\nNumber Of Viewers: %i\nChannel Name: %s\n",
    data.hosting_channel_name, data.number_of_viewers, data.channel_name);
  printf("\n---------------------------------------------------\n");
}
void onHostStopCallback(HostTarget data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=HOST STOP=\n");
  printf("Hosting Channel Name: %s\nNumber Of Viewers: %i\n",
    data.hosting_channel_name, data.number_of_viewers);
  printf("\n---------------------------------------------------\n");
}
void onUserBanCallback(ClearChat data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=USER BAN=\n");
  printf("Banned User: %s\nDuration of Ban: %i\n", data.username, data.ban_duration);
  printf("\n---------------------------------------------------\n");
}
void onUserGivenModCallback(User data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=USER MOD=\n");
  printf("Modded User: %s\n", data.display_name);
  printf("\n---------------------------------------------------\n");
}
void onCustomRewardCallback(PrivMsg data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=CUSTOM REWARD ID=\n");
  printf("Custom Reward Id: %s\n", data.custom_reward_id);
  printf("\n---------------------------------------------------\n");
}
void onHighlightedMsgCallback(PrivMsg data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=HIGHLIGHTED MESSAGE=\n");
  printf("Highlighted Message: %s\n", data.message);
  printf("\n---------------------------------------------------\n");
}
void onModChatCallback(PrivMsg data) {
  printf("\n---------------------------------------------------\n");
  printf("\n=MODCHAT=\n");
  printf("Moderator's Display Name: %s\nModerator's Message: %s\n", data.display_name, data.message);
  printf("\n---------------------------------------------------\n");
}


void setup() {

    // Connect to wifi
   WiFi.begin(ssid, password);
 
   
  // Check if connected to wifi
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("No Wifi");
    delay(2000);
  }

    Serial.begin(74880);
    Serial.println("Connected to Wifi.");
    startMillis = millis();  //initial start time
    previousMillis = millis(); // initial data set for first loop

  //Connect to twitch via websockets
  twitch_api.connect_to_twitch_websocket();

  //Register generic callbacks
  twitch_api.onClearChat(onClearChatCallback);
  twitch_api.onClearMsg(onClearMsgCallback);
  twitch_api.onNotice(onNoticeCallback);
  twitch_api.onGlobalUserState(onGlobalUserStateCallback);
  twitch_api.onPrivMsg(onPrivMsgCallback);
  twitch_api.onRoomState(onRoomStateCallback);
  twitch_api.onUserNotice(onUserNoticeCallback);
  twitch_api.onUserState(onUserStateCallback);
  twitch_api.onHostTarget(onHostTargetCallback);

  //Register custom callbacks
  twitch_api.onChannelHosted(onChannelHostedCallback);
  twitch_api.onBits(onBitsCallback);
  twitch_api.onSub(onSubCallback);
  twitch_api.onReSub(onReSubCallback);
  twitch_api.onSubGift(onSubGiftCallback);
  twitch_api.onAnonSubGift(onAnonSubGiftCallback);
  twitch_api.onSubMysteryGift(onSubMysteryGiftCallback);
  twitch_api.onGiftPaidUpgrade(onGiftPaidUpgradeCallback);
  twitch_api.onRewardGift(onRewardGiftCallback);
  twitch_api.onAnonGiftPaidUpgrade(onAnonGiftPaidUpgradeCallback);
  twitch_api.onRaid(onRaidCallback);
  twitch_api.onUnRaid(onUnRaidCallback);
  twitch_api.onRitual(onRitualCallback);
  twitch_api.onBitsBadgeTier(onBitsBadgeTierCallback);
  twitch_api.onHostStart(onHostStartCallback);
  twitch_api.onHostStop(onHostStopCallback);
  twitch_api.onUserBan(onUserBanCallback);
  twitch_api.onUserGivenMod(onUserGivenModCallback);
  twitch_api.onCustomReward(onCustomRewardCallback);
  twitch_api.onHighlightedMsg(onHighlightedMsgCallback);
  twitch_api.onModChat(onModChatCallback);

  //Helper Functions
  /*
    A sub plan is returned on subs, resub, gift, etc. You can use the helper function to convert int value to string.
  */
  printf("%s\n", twitch_api.sub_type_to_string(0));
  printf("%s\n", twitch_api.sub_type_to_string(1000));
  printf("%s\n", twitch_api.sub_type_to_string(2000));
  printf("%s\n", twitch_api.sub_type_to_string(3000));
  /*
    Emotes are returned when people say them in chat or in messages when donating. You can use the function to get the image url of the icon.
    Sizes: IMG_SMALL, IMG_MEDIUM, IMG_LARGE
  */
  printf("Emote Image URL: %s\n", twitch_api.get_emote_image_url(301448586, TwitchWebsocketIRC::Emote_Image_Size::IMG_MEDIUM));
}

void loop() {

/** figure this out later
if (digitalRead(switchPin) == 1){
    Paused = false;
    Serial.println("NOT PAUSED DEFAULT"); 
}
else if (digitalRead(switchPin) == 0){
  Paused = true;
  Serial.println("PAUSED");
}
Serial.println(Paused);

Serial.println(digitalRead(switchPin));
*/

  
  if(twitch_api.available()) {
        twitch_api.poll();
    }

if (Paused){
        //Check if paused
        //if so do nothing
}



if (Nextread == Nextwrite){
  printf("No more Alerts \n");
}
if ((unsigned long)(millis() - previousMillis) >= period){
//test whether the period has elapsed
        
// determine the type
    if (strcmp(Alertdata[Nextread].Type,"Bits") == 0){

/////////////PERFORM BITS ACTION!!!!!

//////!tswears 35

        if (Alertdata[Nextread].Aamount == 35){
            char message[] = {"/w drpuppetmaster !tswears "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!omrwhat 40

        if (Alertdata[Nextread].Aamount == 40){
            char message[] = {"/w drpuppetmaster !omrwhat "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }
        
//////!bthatshot 45

        if (Alertdata[Nextread].Aamount == 45){
            char message[] = {"/w drpuppetmaster !bthatshot "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!death 48
     
        if (Alertdata[Nextread].Aamount == 48){
            char message[] = {"/w drpuppetmaster !death "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }


//////!lilbitch 50
     
        if (Alertdata[Nextread].Aamount == 50){
            char message[] = {"/w drpuppetmaster !lilbitch "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!cblind 51
     
        if (Alertdata[Nextread].Aamount == 51){
            char message[] = {"/w drpuppetmaster !cblind "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }


//////!huge 69
     
        if (Alertdata[Nextread].Aamount == 69){
            char message[] = {"/w drpuppetmaster !huge "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!tech 70
     
        if (Alertdata[Nextread].Aamount == 70){
            char message[] = {"/w drpuppetmaster !tech "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!worked 73
     
        if (Alertdata[Nextread].Aamount == 73){
            char message[] = {"/w drpuppetmaster !worked "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }


//////!subtle 75 TAUNTS - TIER 1
     
        if (Alertdata[Nextread].Aamount == 75){
            char message[] = {"/w drpuppetmaster !subtle "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }        

//////!kevtool 80
     
        if (Alertdata[Nextread].Aamount == 80){
            char message[] = {"/w drpuppetmaster !kevtool "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!thelp 85
     
        if (Alertdata[Nextread].Aamount == 85){
            char message[] = {"/w drpuppetmaster !thelp "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }


//////!fukev 90

        if (Alertdata[Nextread].Aamount == 90){
            char message[] = {"/w drpuppetmaster !fukev "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!mynameis 95

        if (Alertdata[Nextread].Aamount == 95){
            char message[] = {"/w drpuppetmaster !mynameis "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }        

//////!fart 100
     
        if (Alertdata[Nextread].Aamount == 100){
            char message[] = {"/w drpuppetmaster !fart "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!OMR 110
     
        if (Alertdata[Nextread].Aamount == 110){
            char message[] = {"/w drpuppetmaster !OMR "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!mlyrics 120

        if (Alertdata[Nextread].Aamount == 120){
            char message[] = {"/w drpuppetmaster !mlyrics "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!bthot 125

        if (Alertdata[Nextread].Aamount == 125){
            char message[] = {"/w drpuppetmaster !bthot "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!bfinger 130

        if (Alertdata[Nextread].Aamount == 130){
            char message[] = {"/w drpuppetmaster !bfinger "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }

//////!cknuckle 140
     
        if (Alertdata[Nextread].Aamount == 140){
            char message[] = {"/w drpuppetmaster !cknuckle "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }   
      
//////!mean 150 TAUNTS - TIER 2
     
        if (Alertdata[Nextread].Aamount == 150){
            char message[] = {"/w drpuppetmaster !mean "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);          
            Next(&Nextread);
            previousMillis = millis();
        }    

//////!tsick 160
     
        if (Alertdata[Nextread].Aamount == 160){
            char message[] = {"/w drpuppetmaster !tsick "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);          
            Next(&Nextread);
            previousMillis = millis();
        }  

//////!crage 170
     
        if (Alertdata[Nextread].Aamount == 170){
            char message[] = {"/w drpuppetmaster !crage "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }   

//////!mhangout 180
     
        if (Alertdata[Nextread].Aamount == 180){
            char message[] = {"/w drpuppetmaster !mhangout "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);
            Next(&Nextread);
            previousMillis = millis();
        }   

//////!stopboobs 200
     
        if (Alertdata[Nextread].Aamount == 200){
            char message[] = {"/w drpuppetmaster !stopboobs "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);          
            Next(&Nextread);
            previousMillis = millis();
        }   
        
//////!bcomplain 210
     
        if (Alertdata[Nextread].Aamount == 210){
            char message[] = {"/w drpuppetmaster !bcomplain "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);   
            Next(&Nextread);
            previousMillis = millis();
        }   

//////!milkshakes 250
     
        if (Alertdata[Nextread].Aamount == 250){
            char message[] = {"/w drpuppetmaster !milkshakes "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);  
            Next(&Nextread);
            previousMillis = millis();
        }           
//////!tits 280
     
        if (Alertdata[Nextread].Aamount == 280){
            char message[] = {"/w drpuppetmaster !tits "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);    
            Next(&Nextread);
            previousMillis = millis();
        }     


//////!nasty 300 TAUNTS - TIER 3
     
        if (Alertdata[Nextread].Aamount == 300){
            char message[] = {"/w drpuppetmaster !nasty "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);   
            Next(&Nextread);
            previousMillis = millis();
        }
        
//////!csmellfinger 320
     
        if (Alertdata[Nextread].Aamount == 320){
            char message[] = {"/w drpuppetmaster !csmellfinger "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);   
            Next(&Nextread);
            previousMillis = millis();
        }

//////!bnudes 500
     
        if (Alertdata[Nextread].Aamount == 500){
            char message[] = {"/w drpuppetmaster !bnudes "};
            strncat (message,Alertdata[Nextread].Username,100);
            twitch_api.send_chat_message(message);   
            Next(&Nextread);
            previousMillis = millis();
        }
        
/////////////SUB ACTIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    else if (strcmp(Alertdata[Nextread].Type,"Sub") == 0){

      twitch_api.send_chat_message("/w drpuppetmaster !newsub");    
      Next(&Nextread);
      previousMillis = millis();

    }
/////////////RESUB ACTIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      
    
    else if (strcmp(Alertdata[Nextread].Type,"Resub") == 0){
      
      char message[] = {"/w drpuppetmaster !resub "};
      strncat (message,Alertdata[Nextread].Username,100);
      twitch_api.send_chat_message(message);
      Next(&Nextread);
      previousMillis = millis();
    }
/////////////////FOLLOW ACTIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      
    
    else if (strcmp(Alertdata[Nextread].Type,"Follow") == 0){

      char message[] = {"/w drpuppetmaster !follow "};
      strncat (message,Alertdata[Nextread].Username,100);
      twitch_api.send_chat_message(message);
      Next(&Nextread);
      previousMillis = millis();
    }
//////////////////RAID ACTIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
      
 /* REMOVED, WE WANT THIS TO TRIGGER IMMEDIATELY   
  *  
  
    else if (strcmp(Alertdata[Nextread].Type,"Raid") == 0){


      twitch_api.send_chat_message("/w drpuppetmaster !raid"); 
      Next(&Nextread);
      previousMillis = millis();
    }

   */
 /////////////////////REWARDID!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
    
    
    else if (strcmp(Alertdata[Nextread].Type,"Reward") == 0){

      Next(&Nextread);
      previousMillis = millis();
    }
/////////////////////DONATION ACTIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    else if (strcmp(Alertdata[Nextread].Type,"Donation") == 0){

      previousMillis = millis();


    
    if (Alertdata[Nextread].Aamount >= 50){ 
      //large donations
          char message[] = {"/w drpuppetmaster !ldono "};
          strncat (message,Alertdata[Nextread].Username,100);
          twitch_api.send_chat_message(message);
          Next(&Nextread);
          previousMillis = millis();
              
    }
          
    else {
      //standard donations
      char message[] = {"/w drpuppetmaster !dono "};
      strncat (message,Alertdata[Nextread].Username,100);
      twitch_api.send_chat_message(message);
      Next(&Nextread);
      previousMillis = millis();
  
    }
   
} // END OF ALERTS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
}
    
    Serial.print ("Type is ");
    Serial.println (Alertdata[Nextread].Type);
    Serial.print ("Username is ");
    Serial.println (Alertdata[Nextread].Username);
    Serial.print ("Amount is ");
    Serial.println (Alertdata[Nextread].Aamount);
    
                      //if so read from array
                      //determine and action
       
    
    delay(1000);
}

void Next(int *pNext)
{
  (*pNext)++;
  if (*pNext > 20)
  {
    *pNext = 0;
  }
}
