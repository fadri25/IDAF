#include "deltatime.h"
#include <iostream>


DeltaTime::DeltaTime() {
	
	last = std::chrono::steady_clock::now();
	//last = std::clock();
}

DeltaTime::~DeltaTime() {

}

// Berechnet die Deltazeit zwischen frames
void DeltaTime::update() {
	now = std::chrono::steady_clock::now();
	delta += std::chrono::duration<double, std::milli>(now - last).count() / 1000.0;
	last = now;
}

// Gibt aktuelle Deltazeit an aufrufer zurück
float DeltaTime::getDelta() const {
	return (float)std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - last).count() / 1000.0;
}


// Konvertiert @member delta zu float Datentyp;
DeltaTime:: operator float() {
	return (float)delta;
}

// Verringert Deltazeit um eine Sekunde
void DeltaTime::operator--(int x) {
	delta--;
}