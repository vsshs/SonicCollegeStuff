#include <TimerOne.h>


#define DIR_CCW 0x10
#define DIR_CW 0x20

#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
	// R_START
	{R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
	// R_CW_FINAL
	{R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
	// R_CW_BEGIN
	{R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
	// R_CW_NEXT
	{R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
	// R_CCW_BEGIN
	{R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
	// R_CCW_FINAL
	{R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
	// R_CCW_NEXT
	{R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

volatile unsigned char state1 = 0;
volatile unsigned char state2 = 0;
volatile unsigned char state3 = 0;
volatile unsigned char state4 = 0;

volatile int tempo = 0;
volatile int tempo_res = 4;
volatile int  filter = 0;
volatile int speed = 0;
volatile int pich = 0;


void rotary_init() {
	pinMode(4, INPUT); digitalWrite (4, HIGH);
	pinMode(5, INPUT); digitalWrite (5, HIGH);

	pinMode(6, INPUT); digitalWrite (6, HIGH);
	pinMode(7, INPUT); digitalWrite (7, HIGH);

	pinMode(8, INPUT); digitalWrite (8, HIGH);
	pinMode(9, INPUT); digitalWrite (9, HIGH);

	pinMode(10, INPUT); digitalWrite (10, HIGH);
	pinMode(11, INPUT); digitalWrite (11, HIGH);
}


void timer_init()
{
	Timer1.initialize(1000); 
	Timer1.attachInterrupt( timer_isr );
}

void timer_isr()
{
	unsigned char result1 = rotary_process_1();
	if (result1) {

		if (result1 == DIR_CW)
			tempo++;
		else 
			tempo--;

		if (tempo > 19) tempo = 0;
		if (tempo < 0) tempo = 19;

		if (tempo >= 0 && tempo < 5) tempo_res = 4;

		if (tempo >= 5 && tempo <10) tempo_res = 8;

		if (tempo >=10 && tempo < 15) tempo_res = 16;

		if (tempo >= 15 && tempo_res < 20) tempo_res = 32;

		

		
	}

	unsigned char result2 = rotary_process_2();
	if (result2) {

		if (result2 == DIR_CW)
			filter+=50;
		else
			filter-=50;

		if (filter > 1000) filter = 0;
		if (filter < 0) filter = 1000;

		Serial.println(filter);
	}

	unsigned char result3 = rotary_process_3();
	if (result3) {


		if (result3 == DIR_CW)
			speed+=50;
		else
			speed-=50;

		if (speed > 1000) speed = 0;
		if (speed < 0) speed = 1000;
	}


	unsigned char result4 = rotary_process_4();
	if (result4) {
		if (result4 == DIR_CW)
			pich+=50;
		else
			pich-=50;

		if (pich > 1000) pich = 0;
		if (pich < 0) pich = 1000;
	}


}

unsigned char rotary_process_1() {
	unsigned char pinstate = (digitalRead(5) << 1) | digitalRead(4);
	state1 = ttable[state1 & 0xf][pinstate];
	return (state1 & 0x30);
}


unsigned char rotary_process_2() {
	unsigned char pinstate = (digitalRead(7) << 1) | digitalRead(6);
	state2 = ttable[state2 & 0xf][pinstate];
	return (state2 & 0x30);
}


unsigned char rotary_process_3() {
	unsigned char pinstate = (digitalRead(9) << 1) | digitalRead(8);
	state3 = ttable[state3 & 0xf][pinstate];
	return (state3 & 0x30);
}

unsigned char rotary_process_4() {
	unsigned char pinstate = (digitalRead(11) << 1) | digitalRead(10);
	state4 = ttable[state4 & 0xf][pinstate];
	return (state4 & 0x30);
}

void setup() {
	Serial.begin(115200);
	rotary_init();
	timer_init();

}



void loop() {

}

