#include "FirebaseClient.h"
#include "DHT11.h"
//Setting firebase, config API
FirebaseSetting::FirebaseSetting(String apiKey, String databaseURL){
    config.api_key = apiKey;
    config.database_url = databaseURL;
    signUp = false;
}
//Login firebase
void FirebaseSetting::login(){ //log in anonymous
    if (Firebase.signUp(&config, &auth, "", "")) { 
        Serial.println("Sign in sucessfull");
        signUp = true;
    }
    else {
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
}
//Start firebase
void FirebaseSetting::begin(){
    Firebase.begin(&config, &auth);
}
//Send data to firebase
void FirebaseSetting::sendDataToFBS(float temp, float humid){
    if (Firebase.ready() && signUp) {
        //Send temperature to firebase
        if(Firebase.RTDB.setFloat(&fbdo, "Sensor/temp_data", temp)) {
            Serial.println(); 
            Serial.print(temp);
            Serial.print("Saved to: ");
            Serial.print(fbdo.dataPath());
            Serial.print("(");
            Serial.print(fbdo.dataType());
            Serial.print(")");
        }
        else {
            Serial.println("Failed: ");
            Serial.println(fbdo.errorReason());
        };
        //Send humidity to firebase
        if(Firebase.RTDB.setFloat(&fbdo, "Sensor/humid_data", humid)) {
            Serial.println(); 
            Serial.print(humid);
            Serial.print("Saved to: ");
            Serial.print(fbdo.dataPath());
            Serial.print("(");
            Serial.print(fbdo.dataType());
            Serial.print(")");
        }
        else {
            Serial.println("Failed: ");
            Serial.println(fbdo.errorReason());
        };
    };
}

