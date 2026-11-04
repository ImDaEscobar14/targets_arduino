#include <Arduino.h>
#include <MicroOscSlip.h>
#include <Chrono.h>
#include <Bounce2.h>

Bounce2::Button bouton;
Chrono chronoPot;

MicroOscSlip<128> monOsc(&Serial);

void setup()
{
    Serial.begin(115200);
    bouton.attach(9, INPUT_PULLUP);
    bouton.setPressedState(LOW);
    pinMode(6, OUTPUT);
}

void loop()
{
    monOsc.onOscMessageReceived(traiterMessageOsc); // ACQUISITION DES MESSAGES

    bouton.update();
    if (bouton.pressed())
    { // Bouton vient d’être appuyé
        monOsc.sendInt("/tir", 1);
    }
    

   if (chronoPot.hasPassed(22))
    {                        // SI LE CHRONO DÉPASSE 22 MILLISECONDES
        chronoPot.restart(); // REPARTIR LE CHRONO

        // LE 2 CI_BAS CORRESPOND À LA BROCHE ANALOGIQUE A1
        int valeur = analogRead(1); // LECTURE DE LA TENSION DE LA BROCHE A2 ENTRE 0 ET 1023

        monOsc.sendInt("/rot", valeur); // ENVOYER LA VALEUR
    }


}

void traiterMessageOsc(MicroOscMessage &message)
{
    // SI L’ADRESSE DU MESSAGE EST "/tir" ET QUE LE MESSAGE CONTIENT UN ARGUMENT ENTIER ("i") :
    if (message.checkOscAddressAndTypeTags("/tir", "1"))
    {

        int valeur = message.nextAsInt(); // RÉCUPÉRER L’ENTIER ("i")

        digitalWrite(9, valeur); // ASSIGNER LA VALEUR A LA DEL
    }

    // SI L’ADRESSE DU MESSAGE EST "/lumiere" ET QUE LE MESSAGE CONTIENT UN ARGUMENT ENTIER ("i") :
    if (message.checkOscAddressAndTypeTags("/ok", "i"))
    {

        int valeur = message.nextAsInt(); // RÉCUPÉRER L’ENTIER ("i")

        digitalWrite(6, valeur); // ASSIGNER LA VALEUR A LA DEL
    }
}




