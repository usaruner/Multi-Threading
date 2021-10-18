#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
 
/* Global variables, Look at their usage in main() */
int image_height;
int image_width;
int image_maxShades;
int** inputImage;
int** outputImage;
int num_threads;
int maxChunk;
int doneChunk;
std::mutex Pmtx;
/* ****************Change and add functions below ***************** */
void serial(int startx,int starty,int width, int height){
    
    int maskX[2][2];
    int maskY[2][2];
    int grad_x;
    int grad_y;
    int grad;
    /* 2x2 Roberts cross mask for X Dimension. */
    maskX[0][0] = +1; maskX[0][1] = 0;
    maskX[1][0] = 0; maskX[1][1] = -1;
    /* 2x2 Roberts cross mask for Y Dimension. */
    maskY[0][0] = 0; maskY[0][1] = +1;
    maskY[1][0] = -1; maskY[1][1] = 0;
    for (int x = startx; x < height; ++x) {
        for (int y = starty; y < width; ++y) {
            grad_x = 0;
            grad_y = 0;
            /* For handling image boundaries */
            if (x == (image_height - 1) || y == (image_width - 1))
                grad = 0;
            else {
                /* Gradient calculation in X Dimension */
                for (int i = 0; i <= 1; i++) {
                    for (int j = 0; j <= 1; j++) {
                        grad_x += (inputImage[x + i][y + j] * maskX[i][j]);
                    }
                }
                /* Gradient calculation in Y Dimension */
                for (int i = 0; i <= 1; i++) {
                    for (int j = 0; j <= 1; j++) {
                        grad_y += (inputImage[x + i][y + j] * maskY[i][j]);
                    }
                }
                /* Gradient magnitude */
                grad = (int)sqrt((grad_x * grad_x) + (grad_y * grad_y));
            }
            outputImage[x][y] = grad <= 255 ? grad : 255;
            
        }
        mtx.lock();
        //std::cout << "x: " << x << std::endl;
        mtx.unlock();
    }
    //std::cout << "x: " << startx << " y:" << starty << "height" << height << "width"  << width << std::endl;
}
bool cont() {
    mtx.lock();
    bool truth = (doneChunk != maxChunk);
    mtx.unlock();
    return truth;
}
void threadfunc() {
    int chunkSize = image_height/maxChunk;
    int thisChunk;
    int start;
    while (cont()) {
        mtx.lock();
        thisChunk = doneChunk;
        doneChunk++;
        mtx.unlock();
        start = thisChunk * chunkSize;
        if (start + chunkSize > image_height) {
            chunkSize = image_height - start - 1;
        }
        serial(start, 0, image_width, start + chunkSize);
    }
}
void dispatch_threads()
{
    doneChunk = 0;
    std::vector<std::thread> threads;
    //std::thread(eat_meal,std::ref(philosophers[0]),std::ref(philosophers[0].fork));
    for (int i = 0; i < num_threads; i++)
    {
        threads.push_back(std::thread(threadfunc));
    }
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }
}

/* ****************Need not to change the function below ***************** */

int main(int argc, char* argv[])
{
    if(argc != 5)
    {
        std::cout << "ERROR: Incorrect number of arguments. Format is: <Input image filename> <Output image filename> <# of Threads> <# of Chunks>" << std::endl;
        return 0;
    }
 
    std::ifstream file(argv[1]);
    if(!file.is_open())
    {
        std::cout << "ERROR: Could not open file " << argv[1] << std::endl;
        return 0;
    }
    num_threads = std::atoi(argv[3]);
    /* maxChunk is total number of chunks to process */
    maxChunk = std::atoi(argv[4]);

    std::cout << "Detect edges in " << argv[1] << " using " << num_threads << " threads\n" << std::endl;

    /* ******Reading image into 2-D array below******** */

    std::string workString;
    /* Remove comments '#' and check image format */ 
    while(std::getline(file,workString))
    {
        if( workString.at(0) != '#' ){
            if( workString.at(1) != '2' ){
                std::cout << "Input image is not a valid PGM image" << std::endl;
                return 0;
            } else {
                break;
            }       
        } else {
            continue;
        }
    }
    /* Check image size */ 
    while(std::getline(file,workString))
    {
        if( workString.at(0) != '#' ){
            std::stringstream stream(workString);
            int n;
            stream >> n;
            image_width = n;
            stream >> n;
            image_height = n;
            break;
        } else {
            continue;
        }
    }

    inputImage = new int*[image_height];
    outputImage = new int*[image_height];
    for(int i = 0; i < image_height; ++i){
        inputImage[i] = new int[image_width];
        outputImage[i] = new int[image_width];
    }

    /* Check image max shades */ 
    while(std::getline(file,workString))
    {
        if( workString.at(0) != '#' ){
            std::stringstream stream(workString);
            stream >> image_maxShades;
            break;
        } else {
            continue;
        }
    }
    /* Fill input image matrix */ 
    int pixel_val;
    for( int i = 0; i < image_height; i++ )
    {
        if( std::getline(file,workString) && workString.at(0) != '#' ){
            std::stringstream stream(workString);
            for( int j = 0; j < image_width; j++ ){
                if( !stream )
                    break;
                stream >> pixel_val;
                inputImage[i][j] = pixel_val;
            }
        } else {
            continue;
        }
    }

    /************ Function that creates threads and manage dynamic allocation of chunks *********/
    dispatch_threads();

    /* ********Start writing output to your file************ */
    std::ofstream ofile(argv[2]);
    if( ofile.is_open() )
    {
        ofile << "P2" << "\n" << image_width << " " << image_height << "\n" << image_maxShades << "\n";
        for( int i = 0; i < image_height; i++ )
        {
            for( int j = 0; j < image_width; j++ ){
                ofile << outputImage[i][j] << " ";
            }
            ofile << "\n";
        }
    } else {
        std::cout << "ERROR: Could not open output file " << argv[2] << std::endl;
        return 0;
    }
    return 0;
}