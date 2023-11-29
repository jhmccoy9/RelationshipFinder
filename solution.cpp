// #include "htk.h"
// #include <iostream>
// #include <omp.h>


// // play around with the chunk size
// #define CHUNK_SIZE 16


// int iterations;


// int main(int argc, char *argv[]) {
//   htkArg_t args;
//   int width;
//   int height;
//   int channels;
//   char *inputFile;
//   htkImage_t input;
//   htkImage_t output;
//   float *hostInputData;
//   float *hostOutputData;

//   args = htkArg_read(argc, argv);
//   if (args.inputCount != 1) {htkLog(ERROR, "Missing input"); return 1;}

//   htkTime_start(IO, "Importing data and creating memory on host");
//   inputFile = htkArg_getInputFile(args, 0);
//   input = htkImport(inputFile);
//   width  = htkImage_getWidth(input);
//   height = htkImage_getHeight(input);
//   channels  = htkImage_getChannels(input);
//   if (channels != 1) {htkLog(ERROR, "Expecting gray scale image"); return 1;}
//   output = htkImage_new(width, height, channels);
//   hostInputData  = htkImage_getData(input);
//   hostOutputData = htkImage_getData(output);
//   htkTime_stop(IO, "Importing data and creating memory on host");
//   htkLog(TRACE, "Image dimensions WxH are ", width, " x ", height);




//   iterations = 0;

//   // add some run again reduction thing here maybe

//   // make the output be identical to the input, at the beginning
//   // otherwise you lose your initial heat values...
//   for (int i = 0; i < height; i++)
//   {
//     for (int j = 0; j < width; j++)
//     {
//       hostOutputData[i * width + j] = hostInputData[i * width + j];
//     }
//   }

//   // this will hold the temp array address later
//   float* temp;

//   // this determines if we will run again
//   int run_again = 1;


//   htkTime_start(Compute, "Doing the computation");
//   // rework this into a loop
//   //for (int k = 0; k < 3963; k++){
//   while(run_again)
//   {
//     run_again = 0;
//     // this code calls the hot plate code, which returns an int so you know if it needs to be run again or not
//     #pragma omp parallel for schedule(guided, CHUNK_SIZE) reduction(|:run_again) 
//     for (int i = 1; i < height - 1; i++)
//     {
//       //std::cout << omp_get_thread_num() << std::endl;
//       //static int hot_plate_parallel(float *out, float *in, int width, int row)
//       run_again |= hot_plate_parallel(hostOutputData, hostInputData, width, i);

//     }
//     //swap the pointers
//     temp = hostInputData; 
//     hostInputData = hostOutputData;
//     hostOutputData = temp;

//     iterations++;
//   } 

//   //hot_plate(hostOutputData, hostInputData, width, height, EPSILON);





//   htkTime_stop(Compute, "Doing the computation");
//   htkLog(TRACE, "Solution iterations: ", iterations);












//   htkSolution(args, output);

//   htkImage_delete(output);
//   htkImage_delete(input);

//   return 0;
// }
