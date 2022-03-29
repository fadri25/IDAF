#include "deltatime.h"
#include <iostream>


DeltaTime::DeltaTime() {
	last = std::clock();
}

DeltaTime::~DeltaTime() {

}

// Berechnet die Deltazeit zwischen frames
void DeltaTime::update() {
	now = std::clock();
	delta += (now - last) / 1000.0;
	last = now;
}

// Gibt aktuelle Deltazeit an aufrufer zurück
float DeltaTime::getDelta() const {
	return (float)( (std::clock() - last) / 1000.0);
}


// Konvertiert @member delta zu float Datentyp;
DeltaTime:: operator float() {
	return (float)delta;
}

// Verringert Deltazeit um eine Sekunde
void DeltaTime::operator--(int x) {
	delta--;
}