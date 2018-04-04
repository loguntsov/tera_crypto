/*	<3 Pinkie Pie

		Thanks to P5yl0 (original C# implementation) & Meishu (pure JS port).

		Original C# source:
		https://github.com/P5yl0/TeraEmulator_2117a/tree/master/Tera_Emulator_Source_2117/GameServer/Crypt

		Based on https://github.com/pinkipi/tera-crypto/blob/master/main.cpp
*/

class TeraCrypto {
private:
  unsigned int d1[55], d2[57], d3[58];

  int i1 = 0, i2 = 0, i3 = 0,
      b1 = 0, b2 = 0, b3 = 0,
      pos = 0;

  unsigned int sum1 = 0, sum2 = 0, sum3 = 0,
      sum = 0;

public:
  TeraCrypto(const unsigned char* data) {
		for(int i = 0; i < 55; i++) {
			d1[i] = data[i];
		}
		for(int i = 0; i < 57; i++) {
			d2[i] = data[i + 55];
		}
		for(int i = 0; i < 58; i++) {
			d3[i] = data[i + 112];
		}
	}

	void apply(unsigned char* data, unsigned int length) {
		if(pos) { // Previously non-aligned data
			int off = 4 - pos;
			length -= off;

			if(length < 0) off += length;

			// Slow: individual bytes
			switch(off) {
				case 1:
					(data++)[0] ^= ((char*)&sum)[pos];
					pos = (pos + 1) & 3;
				break;
				case 2:
					(data++)[0] ^= ((char*)&sum)[pos];
					(data++)[0] ^= ((char*)&sum)[pos + 1];
					pos = (pos + 2) & 3;
				break;
				case 3:
					(data++)[0] ^= ((char*)&sum)[pos];
					(data++)[0] ^= ((char*)&sum)[pos + 1];
					(data++)[0] ^= ((char*)&sum)[pos + 2];
					pos = 0;
				break;
			}

			if(length <= 0) return;
		}

		int len4 = length / 4;

		// Fast: 4 bytes per round
		for(int i = 0; i < len4; i++) {
			next();
			((unsigned int*)data)[0] ^= sum;
			data += 4;
		}

		if(length %= 4) { // Additional non-aligned data
			next();

			// Slow: individual bytes
			switch(length) {
				case 1:
					data[0] ^= ((char*)&sum)[pos++];
				break;
				case 2:
					(data++)[0] ^= ((char*)&sum)[pos++];
					data[0] ^= ((char*)&sum)[pos++];
				break;
				case 3:
					(data++)[0] ^= ((char*)&sum)[pos++];
					(data++)[0] ^= ((char*)&sum)[pos++];
					data[0] ^= ((char*)&sum)[pos++];
				break;
			}
		}
	}

private:
	void next() {
    int result = b1 & b2 | b3 & (b1 | b2);

    if(result == b1) {
      unsigned long long tmp = (unsigned long long)d1[i1] + (unsigned long long)d1[(i1 + 31) % 55];
      i1 = (i1 + 1) % 55;
      b1 = tmp >> 32;
      sum1 = (unsigned int)tmp;
    }

    if(result == b2) {
      unsigned long long tmp = (unsigned long long)d2[i2] + (unsigned long long)d2[(i2 + 50) % 57];
      i2 = (i2 + 1) % 57;
      b2 = tmp >> 32;
      sum2 = (unsigned int)tmp;
    }

    if(result == b3) {
      unsigned long long tmp = (unsigned long long)d3[i3] + (unsigned long long)d3[(i3 + 39) % 58];
      i3 = (i3 + 1) % 58;
      b3 = tmp >> 32;
      sum3 = (unsigned int)tmp;
    }

    sum = sum1 ^ sum2 ^ sum3;
	}
};
