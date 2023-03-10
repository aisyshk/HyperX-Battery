#include "cloud_flight.h"

const int VENDOR_ID = 0x0951;
const int PRODUCT_ID = 0x1723;
//const int PRODUCT_IDS[] = { 0x1723, 0x16c4 };

int cloud_flight::HID_DEVICE(hid_device* HID_HANDLE)
{
	int HID;
	HID = hid_init();

	if (!(HID_HANDLE = hid_open(VENDOR_ID, PRODUCT_ID, NULL)))
	{
		std::cerr << "Error opening device " << PRODUCT_ID << std::endl;
	}

	do
	{
		READ_STATES(HID_HANDLE);
	} while (true);


	return EXIT();
}

int cloud_flight::READ_STATES(hid_device* HID_HANDLE)
{
	READ_BATTERY(HID_HANDLE);
	//READ_MUTE(HID_HANDLE);
	//READ_POWER(HID_HANDLE);

	return 0;
}

int cloud_flight::READ_BATTERY(hid_device* HID_HANDLE)
{
	const int REPORT_SIZE = 20;
	unsigned char REPORT[REPORT_SIZE];
	REPORT[0] = 0x21;
	REPORT[1] = 0xff;
	REPORT[2] = 0x05;

	hid_write(HID_HANDLE, REPORT, REPORT_SIZE);
	hid_read(HID_HANDLE, REPORT, REPORT_SIZE);

#ifdef DEBUG
	int j;
	for (j = 0; j < REPORT_SIZE; j++)
	{
		std::cout << int((unsigned char)REPORT[j]) << " | ";
	}
#endif

	int CHARGE_STATE = REPORT[3];
	int M_VALUE = REPORT[4] != 0 ? REPORT[4] : CHARGE_STATE;

	int BATTERY_PERCENTAGE = CALCULATE_BATTERY_PERCENTAGE(CHARGE_STATE, M_VALUE);
	if (BATTERY_PERCENTAGE == 199)
		std::cout << '\r' << "Battery: [Charging]";
	else if (BATTERY_PERCENTAGE == 200)
		std::cout << '\r' << "Battery: 100%";
	else
		std::cout << '\r' << "Battery: " << BATTERY_PERCENTAGE << "%";

	return BATTERY_PERCENTAGE;
}

// Not working
int cloud_flight::READ_MUTE(hid_device* HID_HANDLE)
{
	unsigned char REPORT[20];
	REPORT[0] = 0x21;
	REPORT[1] = 0xff;
	REPORT[2] = 0x05;

	hid_write(HID_HANDLE, REPORT, 20);
	hid_read(HID_HANDLE, REPORT, 20);

	int j;
	for (j = 0; j < 5; j++)
	{
		std::cout << int((unsigned char)REPORT[j]) << " | ";
	}

	return 0;
}

//int cloud_flight::READ_POWER(hid_device* HID_HANDLE)
//{
//	return 0;
//}

// Not using compiler extension for ranges in case x ... y:
int cloud_flight::CALCULATE_BATTERY_PERCENTAGE(int CHARGE_STATE, int M_VALUE)
{
	switch (CHARGE_STATE)
	{
		case 0x10:
			if (M_VALUE >= 11)
				return 200; // Full?
			else
				return 199; // Charging
			/*if (M_VALUE >= 0 && M_VALUE < 90)
				return 10;
			else if (M_VALUE >= 90 && M_VALUE < 119)
				return 15;
			else if (M_VALUE >= 120 && M_VALUE < 148)
				return 20;
			else if (M_VALUE >= 149 && M_VALUE < 159)
				return 25;
			else if (M_VALUE >= 160 && M_VALUE < 169)
				return 30;
			else if (M_VALUE >= 170 && M_VALUE < 179)
				return 35;
			else if (M_VALUE >= 180 && M_VALUE < 189)
				return 40;
			else if (M_VALUE >= 190 && M_VALUE < 199)
				return 45;
			else if (M_VALUE >= 200 && M_VALUE < 209)
				return 50;
			else if (M_VALUE >= 210 && M_VALUE < 219)
				return 55;
			else if (M_VALUE >= 220 && M_VALUE < 239)
				return 60;
			else if (M_VALUE >= 240 && M_VALUE < 255)
				return 65;*/

		case 0xf:
			if (M_VALUE >= 130)
				return 100;
			else if (M_VALUE < 130 && M_VALUE >= 120)
				return 95;
			else if (M_VALUE < 120 && M_VALUE >= 100)
				return 90;
			else if (M_VALUE < 100 && M_VALUE >= 70)
				return 85;
			else if (M_VALUE < 70 && M_VALUE >= 50)
				return 80;
			else if (M_VALUE < 50 && M_VALUE >= 20)
				return 75;
			else if (M_VALUE < 20 && M_VALUE > 0)
				return 70;
			/*if (M_VALUE >= 0 && M_VALUE < 19)
				return 70;
			else if (M_VALUE >= 20 && M_VALUE < 49)
				return 75;
			else if (M_VALUE >= 50 && M_VALUE < 69)
				return 80;
			else if (M_VALUE >= 70 && M_VALUE < 99)
				return 85;
			else if (M_VALUE >= 100 && M_VALUE < 119)
				return 90;
			else if (M_VALUE >= 120 && M_VALUE < 129)
				return 95;
			else if (M_VALUE >= 130 && M_VALUE < 255)
				return 100;*/

		case 0xe:
			if (M_VALUE < 250 && M_VALUE > 240)
				return 65;
			else if (M_VALUE < 240 && M_VALUE >= 220)
				return 60;
			else if (M_VALUE < 220 && M_VALUE >= 208)
				return 55;
			else if (M_VALUE < 208 && M_VALUE >= 200)
				return 50;
			else if (M_VALUE < 200 && M_VALUE >= 190)
				return 45;
			else if (M_VALUE < 190 && M_VALUE >= 180)
				return 40;
			else if (M_VALUE < 179 && M_VALUE >= 169)
				return 35;
			else if (M_VALUE < 169 && M_VALUE >= 159)
				return 30;
			else if (M_VALUE < 159 && M_VALUE >= 148)
				return 25;
			else if (M_VALUE < 148 && M_VALUE >= 119)
				return 20;
			else if (M_VALUE < 119 && M_VALUE >= 90)
				return 15;
			else if (M_VALUE < 90)
				return 10;
	}

	return 255;
}

int cloud_flight::EXIT()
{
	hid_exit();
	return 0;
}