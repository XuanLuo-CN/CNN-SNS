// #include <torch/torch.h>
#include <torch/nn/options/padding.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "NN.h"

using namespace torch;

void Printf_basic_data(GlobalMesh *global, Neural_Network *nn, std::string DIR){
  std::cout << "Printf_basic_data..."<< std::endl;
  std::ofstream File(DIR + "/CNN_result/basic_data.txt"); // 打开输出文件流

  if (File.is_open()) {
    File << "\n";
    File << std::setw(40) <<"BASIC DATA\n\n";
    File << std::setw(60) <<"*******************INPUT DATA*******************\n";
    File << "\n";
    File << std::setw(30) << std::left << "How closs" << std::setw(10) << nn->how_close << std::setw(30) << "V limit" << std::setw(10) << nn->v_limit << "\n";
    File << std::setw(30) << "Pool" << std::setw(10) << nn->pool_opt << std::setw(30) << "Num of output channels in Conv1 layer" << std::setw(10) << nn->conv1_size << "\n";
    File << std::setw(30) << "Size of kernel in Conv2 layer" << std::setw(10) << nn->conv2_kernel_size << std::setw(30) << "EPOCH_max" << std::setw(10) << nn->epoch_max << "\n";
    File <<"\n";
    File << std::setw(60) << std::right <<"****************GLOBAL MESH DATA****************\n";
    File <<"\n";
    File << std::setw(30) << std::left << "NC"<< std::setw(10) << "0" << std::setw(30) << "NP" << std::setw(10) << nn->np << "\n";
    File << std::setw(30) << "Partitions" << std::setw(10) << nn->partitions << std::setw(30) << "Num of elements" << std::setw(10) << global->nel << "\n";
    File <<"\n";   
    File << std::setw(60) << std::right <<"**************NEURAL NETWORK DATA***************\n";
    File <<"\n";
    File << std::setw(30) << std::left << "Len of unpad cube" << std::setw(10) << nn->length << std::setw(30) << "Len of pad" << std::setw(10) << nn->pad << "\n";
    File << std::setw(30) << "Len of unpad pool cube" << std::setw(10) << nn->pool_length << std::setw(30) << "Len of pool pad" << std::setw(10) << nn->pool_pad << "\n";
    File << std::setw(30) << "Len of unpool cubes" << std::setw(10) << nn->how_close * 2 + 1 << std::setw(30) << "Len of pool cubes" << std::setw(10) << nn->pool << "\n";
    File << std::setw(30) << "num of input element" << std::setw(10) << nn->pool * nn->pool * nn->pool << std::setw(30) << "Len of trainning set" << std::setw(10) << nn->inpt_num << "\n";
    File << std::setw(30) << "inside rate" << std::setw(10) << 1 - (static_cast<double>(nn->n_beyond) / (nn->inpt_num + nn->n_beyond));
    File.close();   
  }else {
    std::cerr << "Cant open Reconstruction_result.txt" << std::endl;
  }
}

void Printf_basic_test_result(Neural_Network *nn, int *search_times, std::string DIR, int Re){
  std::cout << "Printf_basic_test_result..."<< std::endl;
  int max_SNS = 0;
  double mean_NN = .0, mean_SNS = nn->particle_elm.select(1, 2).to(torch::kFloat).mean().item<double>();
    for(int i = 0; i < nn->inpt_num; i++){
      mean_NN += search_times[i] *1.0 / nn->inpt_num;
      if (search_times[i] > max_SNS)  max_SNS = search_times[i];
    }

  std::ofstream File(DIR + "/CNN_result/basic_data.txt", std::ios::app); 
  if (File.is_open()) {
    File << "\n";
    File << std::setw(60) <<"*******************TEST RESULT*******************\n";
    File << "\n";
    File << std::setw(45) << "Re" << std::setw(10) << Re << "\n";

    File << std::setw(30) << "Average NN search times" << std::setw(10) << mean_NN << std::setw(30) << "Max NN search times" << std::setw(10) << max_SNS << "\n";
    
    File << std::setw(30) << "Average SNS search times" << std::setw(10) << mean_SNS << std::setw(30) << "NN optimization" << (mean_SNS - mean_NN) / mean_SNS * 100 << "\n";


    File.close();    
  }else {
    std::cerr << "Cant open Reconstruction_result.txt" << std::endl;
  }

}

void Printf_test_result(Neural_Network *nn, int *search_times, std::string DIR, int Re){
  std::cout << "Printf_test_result..."<< std::endl;
  std::ofstream File(DIR + "/CNN_result/test_result_Re" + to_string(Re) + ".csv");
  File << nn->inpt_num << std::endl;
  if (File.is_open()) {
    for (int i = 0; i < nn->inpt_num; i++){
      // if( search_times[i] > 60) continue;

    File  <<std::setw(10)<< search_times[i] << "," // CNN-SNS search times
          <<std::setw(10)<< nn->particle_elm[i][2].item<int>() << ","; // SNS search times
    File  <<std::setw(10)<< (nn->particle_elm[i][2].item<int>() - search_times[i]) *1.0 / nn->particle_elm[i][2].item<int>() <<","; // Optimization of search times
    File  <<std::setw(10)<< nn->particle_crd_v[i][3].item<double>() << "," // Particle crd
          <<std::setw(10)<< nn->particle_crd_v[i][4].item<double>() << ","
          <<std::setw(10)<< nn->particle_crd_v[i][5].item<double>() << ",";
    File  <<std::setw(10)<< nn->particle_crd_v[i][6].item<double>()<< "," ; // Particle v
    File  <<std::setw(10)<< nn->particle_elm[i][0].item<int>() << "," 
          <<std::setw(10)<< nn->predicted[i].item<int>() << "," <<std::setw(10)
          << nn->particle_elm[i][1].item<int>() << "," << "\n";
    }
    File.close();    
  }else {
    std::cerr << "Cant open test_result_Re.csv" << std::endl;
  }

  //Save err optimization
  int n_err = 0;
  double err_CNN_SNS = .0, err_SNS = .0, err, mean_v = .0;
  for (int i = 0; i < nn->inpt_num; i++){
    // if(search_times[i] > 5) continue;

    n_err++;

    // Cal err_SNS
    err = .0;
    for (int j = 0; j < 3; j++){
      err += pow((nn->convMesh[nn->particle_elm[i][0].item<int>()][j] - nn->convMesh[nn->particle_elm[i][1].item<int>()][j]).item<double>(), 2);
    }
    err_SNS += pow(err, 0.5);

    // Cal err_CNN_SNS
    err = .0;
    for (int j = 0; j < 3; j++){
      err += pow((nn->convMesh[nn->predicted[i].item<int>()][j] - nn->convMesh[nn->particle_elm[i][1].item<int>()][j]).item<double>(), 2);
    }
    err_CNN_SNS += pow(err, 0.5);

    mean_v += nn->particle_crd_v[i][6].item<double>();
  }
  File.open(DIR + "/CNN_result/Re_err.csv", std::ios::app);
  if (File.is_open()) {
    File << Re << ", " << err_CNN_SNS / n_err << ", " << err_SNS / n_err <<  ", " << (err_SNS - err_CNN_SNS) / err_SNS * 100 <<  ", " << mean_v / n_err << "\n";
    File.close();
  }else {
    std::cerr << "Cant open Re_err.csv" << std::endl;
  }
}


void Printf_time(){    // 获取当前时间
    auto now = std::chrono::high_resolution_clock::now();

    // 转换为微秒
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

    // 打印当前时间（微秒精度）
    std::cout << duration.count() << std::endl;
}


void Printf_conv1_result(GlobalMesh *global, Neural_Network *nn){
  std::cout << "Printf_conv1_result..."<< std::endl;
  int gie, i, j;
  int inop;
  double pnod_x[4], pnod_y[4], pnod_z[4];
  std::ofstream file;
  file.open("../data/conv1_result.txt");
  if (file.is_open()){
    for (gie = 0; gie < global->nel; gie++){
    file << nn->convMesh[gie][0].item<double>() <<" "<< nn->convMesh[gie][1].item<double>()<<" "<< nn->convMesh[gie][2].item<double>()<<"\n";
    }
    file.close();
  }else {
    std::cerr << "Cant open Reconstruction_result.txt" << std::endl;
  }
}


void Printf_crd_cube(Neural_Network *nn){
  std::cout << "Printf_pool_el_cube..."<< std::endl;
  std::ofstream File("../data/Printf_crd_cube.csv"); // 打开输出文件流
  for(int i = 0; i < nn->final_length[2]; i+=10){
    for(int j = 0; j < nn->final_length[1]; j++){
      for(int k = 0; k < nn->final_length[0] * 3; k += 3){
        File << nn->crd_cube[k][j][i].item<float>() <<" "<< nn->crd_cube[k + 1][j][i].item<float>() <<" "<< nn->crd_cube[k + 2][j][i].item<float>() << "    ";
      }
    File << std::endl;
    }
  File << "\n" << std::endl;
  }
}


void Printf_el_cube(Neural_Network *nn){
  std::cout << "Printf_el_cube Testing..."<< std::endl;
  std::ofstream File("../data/Printf_el_cube.csv"); // 打开输出文件流
  for(int i = 0; i < nn->pool_length; i+=10){
    for(int j = 0; j < nn->pool_length; j++){
      for(int k = 0; k < nn->pool_length * 2; k++ ){
        File << nn->el_cube[k][j][i].item<int>() <<" ";
      }
    File << std::endl;
    }
  File << "\n" << std::endl;
  }
}


void Printf_pool_el_cube(Neural_Network *nn){
  std::cout << "Printf_pool_el_cube..."<< std::endl;
  std::ofstream File("../data/Printf_pool_el_cube.csv"); // 打开输出文件流
  for(int i = 0; i < nn->final_length[2]; i+=10){
    for(int j = 0; j < nn->final_length[1]; j++){
      for(int k = 0; k < nn->final_length[0]; k ++){
        File << nn->pool_el_cube[k][j][i].item<int>() <<" ";
      }
    File << std::endl;
    }
  File << "\n" << std::endl;
  }
}

