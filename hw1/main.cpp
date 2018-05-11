#include <QCoreApplication>
#include <QtWidgets/QLabel>
#include <QImage>
#include <iostream>
#include <vector>
#include <clocale>
#include <Windows.h>
#include <time.h>

using namespace std;

const char * path = "C:\\pictures";
const char * path1 = "C:\\pictures\\15.jpg";
const char * outpath = "C:\\pictures\\result";
const char * outpath1 = "C:\\pictures\\result\\15.bmp";

void Breadly() {
    double d = 0.125;
    double t = 0.15;
    int start = clock();
    QImage img( path1 );

    double** for_image = new double*[img.height()];
    for (int i = 0; i < img.height(); i++) {
        for_image[i] = new double[img.width()];
    }

    if ( false == img.isNull() )
    {
        std::cout << "image opened" << std::endl;

        double loc_grey = 0;
        for ( int row = 0; row < img.height(); ++row ){
            for ( int col = 0; col < img.width(); ++col )
            {
                for_image[row][col] = 0;
                QColor clrCurrent( img.pixel( col, row ) );
                loc_grey = 0.2125*clrCurrent.red() + 0.7154*clrCurrent.green() + 0.0721*clrCurrent.blue();
                //loc_grey /= 255;
                //loc_grey = 116 * pow(loc_grey, 1.0/3) - 16;
                //img.setPixel(col, row, qRgb(loc_grey, loc_grey, loc_grey));
                if ( row != 0 && col != 0) {
                    for_image[row][col] = loc_grey + for_image[row - 1][col] + for_image[row][col - 1] - for_image[row - 1][col - 1];
                }
                else {
                    if (row == 0 && col == 0)
                        for_image[row][col] = loc_grey;
                    else {
                        if (row == 0)
                            for_image[row][col] = loc_grey + for_image[row][col - 1];
                        else
                            for_image[row][col] = loc_grey + for_image[row - 1][col];
                    }
                }

            }
        }
        const int shift_y = (int)(img.height() * d);
        const int shift_x = (int)(img.width() * d);
        double loc_sum = 0;
        double threshold = 0;
        for ( int curr_y = shift_y - 1; curr_y < img.height(); curr_y += shift_y ) {
            for ( int curr_x = shift_x - 1; curr_x < img.width(); curr_x += shift_x )
            {
                loc_sum = for_image[curr_y][curr_x];
                if (curr_x - shift_x >= 0) {
                    loc_sum -= for_image[curr_y][curr_x - shift_x];
                }
                if (curr_y - shift_y >= 0) {
                    loc_sum -= for_image[curr_y - shift_y][curr_x];
                }
                if (curr_y - shift_y >= 0 && curr_x - shift_x >= 0) {
                    loc_sum += for_image[curr_y - shift_y][curr_x - shift_x];
                }
                threshold = loc_sum * 1.0 / (shift_x * shift_y) + loc_sum*t;

                for ( int row = curr_y - shift_y + 1; row < curr_y + 1; ++row ) {
                    for ( int col = curr_x - shift_x + 1; col < curr_x + 1; ++col )
                    {
                        QColor clrCurrent( img.pixel( col, row ) );
                        loc_grey = 0.2125*clrCurrent.red() + 0.7154*clrCurrent.green() + 0.0721*clrCurrent.blue();

                        if (loc_grey * shift_x * shift_y  < loc_sum * (1.0 - t))
                            img.setPixel(col, row, qRgb(0, 0, 0));
                        else
                            img.setPixel(col, row, qRgb(255, 255, 255));
                    }
                }

            }
        }
    }


    for (int i = 0; i < img.height(); i++) {
        delete[] for_image[i];
    }
    delete[] for_image;

    bool answ = img.save(QString(outpath1));
    std::cout << (clock() - start) * 1000000.0 / CLOCKS_PER_SEC / (img.width() * img.height()) << std::endl;
}



int main(int argc, char *argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    QCoreApplication a(argc, argv);

    //const char * inputpath = argv[1];
    //const char * outputpath = argv[2];
    Breadly();
    return a.exec();
}


