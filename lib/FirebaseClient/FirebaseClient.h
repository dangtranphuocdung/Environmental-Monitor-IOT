#ifndef FIREBASECLIENT_H
#define FIREBASECLIENT_H

#include <Firebase_ESP_Client.h>

class FirebaseSetting{
    private:
    //Khai báo object
    //
        FirebaseData fbdo;
        FirebaseAuth auth;
        FirebaseConfig config;
        bool signUp;
    public:
        //Config các API key và database url
        FirebaseSetting(String apiKey, String databaseURL);
        void login();
        void begin();
        void sendDataToFBS(float temp, float humid);
};

#endif