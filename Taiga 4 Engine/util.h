
class cColor
{
public:
	int r;
	int g;
	int b;
	int a;

	cColor() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	};

	cColor(int R, int G, int B, int A) {
		r = R;
		g = G;
		b = B;
		a = A;
	}
};

class cUtil
{
public:
	void detectVideoCard();
};