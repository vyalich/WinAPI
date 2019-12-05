#include <fstream>
#include <iostream>

using namespace std;

struct dot{
    float x;
    float y;
};

int main(int argc, char* argv[])
{
    ofstream out("../data/dots.dat", ios_base::binary);

    float x = -5;
    float y = 20;

    out.write((char*)&x, sizeof(x));
    out.write((char*)&y, sizeof(y));
    x = 16.789;
    y = -2.6546;
    out.write((char*)&x, sizeof(x));
    out.write((char*)&y, sizeof(y));
    x = 0.534;
    y = -37.46;
    out.write((char*)&x, sizeof(x));
    out.write((char*)&y, sizeof(y));
    x = -10.789;
    y = -102;
    out.write((char*)&x, sizeof(x));
    out.write((char*)&y, sizeof(y));

    out.close();

    return 0;
}
