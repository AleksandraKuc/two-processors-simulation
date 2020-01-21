#include <systemc.h>

using namespace std;

SC_MODULE(procesor2)
{
	sc_fifo_in<int> input;
	sc_inout<sc_uint<8>> hexes[6];
	int kontrola;

	void task1() {
		kontrola = 0;
		while (1) {
			sc_uint<8> progNo = input.read();
			if (progNo == 2 && kontrola == 0) {
				cout << "Procesor 2, task 1, czas: 30min" << endl;
				hexes[5].write('T');
				hexes[4].write('A');
				hexes[3].write('S');
				hexes[2].write('K');
				hexes[1].write('.');
				hexes[0].write(2);
				kontrola = 1;
			}
			else if (progNo != 2) {
				kontrola = 0;
			}
		}
		wait(10, SC_MS);
	}

	SC_CTOR(procesor2) {
		SC_THREAD(task1);
	}
};