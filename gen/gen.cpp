#include <cstdlib>
#include <vector>
#include <iostream>
#include <set>
#include <sstream>
using namespace std;

#include "Image.h"


int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Usage: ./gen inputName sampleRatio" << endl;
        return 0;
    }


    try {
        string imgfile(argv[1]);
        string name = imgfile.substr(0, imgfile.length()-4);

        // Load Image
        Image Y = Image(imgfile, false, false, 1);

        // Number of samples
        int n = Y.getWidth() * Y.getHeight();
        double sr = atof(argv[2]);
        int k = int((1.0-sr)*n);

        set<int> aux;
        int j = 0;
        vector<vector<bool> > samples(Y.getHeight(), vector<bool>(Y.getWidth(), true));
        while (j < k) {
            int i = rand()%n;
            if (aux.find(i) == aux.end()) {
                aux.insert(i);
                int x = i%Y.getWidth();
                int y = i/Y.getWidth();
                Y.setPixel(x, y, 0, 0, 0);
                samples[y][x] = false;
                j++;
            }
        }

        Matrix S(Y.getHeight(), Y.getWidth());
        for (int i = 0; i < Y.getHeight(); i++) {
            for (int j = 0; j < Y.getWidth(); j++) {
                if (samples[i][j]) {
                    S.insert_element(i, j, 1.0);
                }
                else {
                    S.insert_element(i, j, 0.0);
                }
            }
        }


        int ratio = sr*100+0.5;
        ostringstream oss;
        oss << name << "H" << ratio;
        string outname = oss.str();

        Y.save(outname, false);
        string smpfile = outname + ".txt";
        output_samples(smpfile.c_str(), S);

    }
    catch (int e) {
        switch (e) {
            case IMAGE_NOT_FOUND:
                cerr << "Error: IMAGE NOT FOUND" << endl;
            break;
        }
    }
}
