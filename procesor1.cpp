#include <systemc.h>
#include <conio.h>

using namespace std;

SC_MODULE(procesor1) {
	sc_inout<sc_uint<8>> switches;
	sc_inout<sc_uint<8>> hexes[6];
	sc_inout<sc_uint<8>> diodes;
	sc_fifo_out<int> output;


	bool er = false;
	bool er_read = false;
	int last = 0;
	int count;
	bool constructed = false;

	bool sw_states[5] = { false, false, false, false, false };

	/* Obsluga menu + error */
	void task1() {

		int f = 0;
		while (1) {
			wait(500, SC_NS);

			if (_kbhit()) {

				displayHEX();
				displayLED();
				displaySW();
				wait(25, SC_NS);
				cout << "Wybrano funkcje: ";
				cin >> f;
				cout << endl;

				if (f > 0 && f < 5)
				{
					wait(500, SC_NS);
					sc_uint<8> sw_state = switches.read() ^ toBitFlag(f);
					switches.write(sw_state);
					wait(500, SC_NS);
				}
				else
				{
					cout << "switch poza zakresem! <1-4>" << endl;
				}
			}

			wait(500, SC_NS);
			if (f == 2 && diodes.read() != 2) {
				diodes.write(2);
				output.write(f);
			}
			wait(500, SC_NS);
			count = 0;
			wait(500, SC_NS);
			sc_uint<8> sw_state = switches.read();
			sc_uint<8> oldest_bit = 0x80;
			wait(500, SC_NS);

			//sprawdzamy ile jest w³¹czonych funkcji
			for (int i = 7; i >= 0; i--) {
				if (oldest_bit & sw_state) count++;
				oldest_bit = oldest_bit >> 1;
			}

			wait(500, SC_NS);

			/* Error */
			if (count > 1) {
				diodes.write(0x80);
				hexes[5].write('.');
				hexes[4].write('E');
				hexes[3].write('R');
				hexes[2].write('R');
				hexes[1].write('O');
				hexes[0].write('R');
			}
			else if (count == 0) {
				diodes.write(sw_state);
				getEmptyHexes();
			}
			wait(500, SC_NS);
			switches.write(sw_state);
			wait(100, SC_MS);
		}
	}

	void task2() {
		int kont_task1 = 0;
		while (1) {
			sc_uint<8> progNo = switches.read();

			if (progNo == 1 && diodes.read() != 1 && kont_task1 == 0) {

				cout << "Procesor 1, task 1, czas: 15min" << endl;
				diodes.write(1);
				hexes[5].write('T');
				hexes[4].write('A');
				hexes[3].write('S');
				hexes[2].write('K');
				hexes[1].write('.');
				hexes[0].write(1);
				kont_task1 = 1;
			}
			else if (progNo != 1) {
				kont_task1 = 0;
			}
			wait(10, SC_MS);
		}
	}

	void task3() {
		int kont_task3 = 0;
		while (1) {
			sc_uint<8> progNo = switches.read();

			if (progNo == 4 && diodes.read() != 4 && kont_task3 == 0) {

				cout << "Procesor 1, task 3, czas: 60min" << endl;
				diodes.write(4);
				hexes[5].write('T');
				hexes[4].write('A');
				hexes[3].write('S');
				hexes[2].write('K');
				hexes[1].write('.');
				hexes[0].write(3);
				kont_task3 = 1;
			}
			else if (progNo != 1) {
				kont_task3 = 0;
			}
			wait(10, SC_MS);
		}
	}

	void task4() {
		int kont_task4 = 0;
		while (1) {
			sc_uint<8> progNo = switches.read();

			if (progNo == 8 && diodes.read() != 8 && kont_task4 == 0) {

				cout << "Procesor 1, task 4, czas: 180min" << endl;

				hexes[5].write('T');
				hexes[4].write('A');
				hexes[3].write('S');
				hexes[2].write('K');
				hexes[1].write('.');
				hexes[0].write(4);
				kont_task4 = 1;
			}
			else if (progNo != 8) {
				kont_task4 = 0;
			}
			wait(10, SC_MS);
		}
	}

	void displayLED() {
		sc_uint<8> diodes_state = diodes.read(); //odczytujemy diody
		sc_uint<8> old_bit = 0x80;

		cout << "Nr.:   8 7 6 5 4 3 2 1 " << endl;
		
		cout << "LEDS:  ";

		for (int i = 7; i >= 0; i--) {
			if (diodes_state & old_bit) {
				cout << "1 ";
			}
			else {
				cout << "0 ";
			}
			old_bit = old_bit >> 1;
		}

		cout << endl << endl;
		
	}

	/* Funkcja odpowiedzialna za wyswietlanie switchy i ich statusow */
	void displaySW() {
		sc_uint<8> switches_state = switches.read();
		sc_uint<8> old_bit = 0x80;

		cout << "Nr.:   8 7 6 5 4 3 2 1 " << endl;

		cout << "SWI:   ";

		for (int i = 7; i >= 0; i--) {
			if (switches_state & old_bit) {
				cout << "1 ";
			}
			else {
				cout << "0 ";
			}
			old_bit = old_bit >> 1;
		}
		cout << endl << endl;
	}

	/* Funkcja odpowiedzialna za wyswietlanie hexow i ich statusow */
	void displayHEX() {
		if (!constructed) getEmptyHexes();

		/* Odczytywanie hexow */
		sc_uint<8> hex[6];
		for (int i = 0; i < 6; i++)
			hex[i] = hexes[i].read();

		cout << "Nr.:     6 5 4 3 2 1 " << endl;

		cout << "HEX:     ";
		for (int i = 5; i >= 0; i--) {
			if (hex[i] >= 0 && hex[i] <= 6) {
				cout << hex[i];
				cout << " ";
			}
			else if (hex[i] == (sc_uint<8>) 'E') {
				cout << "E ";
			}
			else if (hex[i] == (sc_uint<8>) 'R') {
				cout << "R ";
			}
			else if (hex[i] == (sc_uint<8>) 'O') {
				cout << "O ";
			}
			else if (hex[i] == (sc_uint<8>) 'T') {
				cout << "T ";
			}
			else if (hex[i] == (sc_uint<8>) 'A') {
				cout << "A ";
			}
			else if (hex[i] == (sc_uint<8>) 'S') {
				cout << "S ";
			}
			else if (hex[i] == (sc_uint<8>) 'K') {
				cout << "K ";
			}
			else if (hex[i] == (sc_uint<8>) ' ') {
				cout << "_ ";
			}
			else if (hex[i] == (sc_uint<8>) '.') {
				cout << "  ";
			}
		}
		cout << endl << endl;
	}

	sc_uint<8> toBitFlag(int value) {

		switch (value) {
		case 1:
		case 2:
			return value;
		case 3:
			return 0x4;
		case 4:
			return 0x8;
		default:
			return 0;
		}

	}

	/* Zerowanie hexów */
	void getEmptyHexes() {
		for (int i = 0; i < 6; i++)
			hexes[i].write((sc_uint<8>) ' ');
	}

	SC_CTOR(procesor1) {
		SC_THREAD(task1); // menu, obsluga erroru
		SC_THREAD(task2);  
		//SC_THREAD(task3); obs³ugiwany na drugim procesorze
		SC_THREAD(task3);  
		SC_THREAD(task4); 
	}
};