float smoothedVal;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing

float smoothedVal2;   // this would be the buffer value for another sensor if you needed to smooth two different sensors - not used in this sketch



void setup()
{
	Serial.begin(115200);
}

unsigned long lastPrint = millis();

void loop()
{ 
	int a0data = analogRead(A0);
	
	smoothedVal = smooth(a0data, 0.9f, smoothedVal); 

	if (millis() - lastPrint > 0)
	{
		lastPrint = millis();
		Serial.print(F("RAW: "));
		Serial.print(a0data, DEC);

		Serial.print(F(", SMOTTHED: "));
		Serial.println((int)smoothedVal);
	}
}


int smooth(int data, float filterVal, float smoothedVal){


	if (filterVal > 1){      // check to make sure param's are within range
		filterVal = .99;
	}
	else if (filterVal <= 0){
		filterVal = 0;
	}

	smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

	return (int)smoothedVal;
}

