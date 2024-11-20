
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <chrono>//Time
#include <ctime>
#include "NN.h"

///////////////////////////////////////
class MyDataset : public torch::data::Dataset<MyDataset> {
  private:
    torch::Tensor states_, labels_;

  public:
    explicit MyDataset(torch::Tensor loc_states, torch::Tensor loc_labels) 
    : states_(loc_states),
      labels_(loc_labels) {}

  torch::data::Example<> get(size_t index) override{
  // You may for example also read in a .csv file that stores locations
  // to your data and then read in the data at this step. Be creative.
    return {states_[index], labels_[index]};
  } 
  torch::optional<size_t> size() const override {
    return labels_.size(0);
  };
};
////////////////////////////////////////

void NN(GlobalMesh *global, char* argv[]) { 

  Neural_Network *nn = new Neural_Network;
  NN_init(global, nn, argv);
  std::string DIR("/public/home/graduate/sFlow-LUO2/Neural_Network/data/unstructed_" + std::string(CASE) + "/" + std::string(CASE) + "_" + std::string(MODEL_ID));

  //****Build sorted Eulerian model****//
  // Adv2csv(global, DIR);
  //  **CU_sort** //
  // Mesh_csv2torch(global, nn, DIR);

  Mesh_torch_load(nn, DIR);

  // Printf_el_cube(nn);
  // Printf_pool_el_cube(nn);
  // Printf_crd_cube(nn);
  

  //****Build input layer****//
  
  if(nn->opt == 1) Particle_csv_2_label(nn, DIR, global); //Save label

  if(COM_ID.myprocid == 0){
    std::cout << "Start CNN..."<< std::endl;

    if(nn->opt == 1) {
      Particle_csv2torch(nn, DIR, nn->repeat, 400);
      torch::load(nn->label,  DIR + "/Torch_data/" + std::to_string(100) + "_label.bin");
      if(nn->label.size(0) != nn->inpt_num) std::cout << "n_label = "<< nn->label.size(0) <<" != nn->inpt_num" << std::endl;
      CNN_train(global, nn, DIR);
    }

    else if(nn->opt == 0) {
      int search_result[2];
      int *search_time = new int[nn->inpt_num];
      Particle_csv2torch(nn, DIR, nn->repeat, 400);
      CNN_test(global, nn, DIR);    

      std::cout << " SNS... "<< std::endl;
      for(int i = 0; i < nn->inpt_num; i++){
        search_element(nn->predicted[i].item<int>(), global, nn->particle_crd_v[i][3].item<double>(), nn->particle_crd_v[i][4].item<double>(), nn->particle_crd_v[i][5].item<double>(), search_result);  //NN search time
        search_time[i] = search_result[0] + 1;
      }

      std::cout << " Save test_result... "<< std::endl;
      Printf_basic_test_result(nn, search_time, DIR, 400);
      Printf_test_result(nn, search_time, DIR, 400);
    }

    else if(nn->opt == 2) {
      int search_result[2];
      for(int Re = 100; Re <= 2000; Re += 100){
        Particle_csv2torch(nn, DIR, nn->repeat, Re);
        CNN_test(global, nn, DIR);

        std::cout << " Save test_result... "<< std::endl;
        int *search_time = new int[nn->inpt_num];

        for(int i = 0; i < nn->inpt_num; i++){
          search_element(nn->predicted[i].item<int>(), global, nn->particle_crd_v[i][3].item<double>(), nn->particle_crd_v[i][4].item<double>(), nn->particle_crd_v[i][5].item<double>(), search_result);  //NN search time
          search_time[i] = search_result[0] + 1;
        }
        
        Printf_basic_test_result(nn, search_time, DIR, Re);
        Printf_test_result(nn, search_time, DIR, Re);

        delete[] search_time;
      }
    }
    std::cout << "Done!"<< std::endl;

  }
}
////////////////////////////////////

void CNN_train(GlobalMesh  *global, Neural_Network *nn, std::string DIR){
  double min_loss = 100;
  Net net(nn->pool, nn->fc_hidden); 
  torch::Device device(torch::kCPU);
  if (torch::cuda::is_available()) {
    std::cout << "CUDA is available! Training on GPU." << std::endl;
    device = torch::Device(torch::kCUDA);
  }

  std::cout << "Begin trainning:" << std::endl;
  // loss and optimation
  torch::nn::CrossEntropyLoss criterion;
  torch::optim::Adam optimizer(net.parameters(), torch::optim::AdamOptions(nn->opt_rate).betas(std::make_tuple (0.5, 0.5)));
  //data set

  auto data_set = MyDataset(nn->close_crd.to(torch::kFloat), torch::cat({nn->particle_crd_v.slice(1, 3, 6).to(torch::kFloat), nn->label.to(torch::kFloat)}, 1)).map(torch::data::transforms::Stack<>());
  auto data_loader = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(
    std::move(data_set), 
    nn->batch_size);

  // nn->close_crd_test = nn->close_crd_test.to(torch::kFloat).to(device);
  // nn->inpt_data_test = nn->inpt_data_test.to(torch::kFloat).to(device);
  // nn->label_test = nn->label_test.to(torch::kLong).to(device);

  // nn->close_crd = nn->close_crd.to(torch::kFloat).to(device);
  // nn->inpt_data = nn->inpt_data.to(torch::kFloat).to(device);
  // nn->label = nn->label.to(torch::kLong).to(device);
  net.to(device);

  for (int epoch = 0; epoch < nn->epoch_max; ++epoch) {
    int64_t batch_index = 0;
    for (torch::data::Example<>& batch : *data_loader) {
      auto close_crd = batch.data.to(device);
      auto label = batch.target.to(device);

      optimizer.zero_grad();  // 清空梯度

      net.train();
      auto outputs = net.forward(close_crd, label.slice(1, 0, 3), nn->pool);  // Forward
      auto loss = criterion(outputs, label.slice(1, 3, 4).squeeze(1).to(torch::kLong));  // 计算损失

      // net.eval();
      // // outputs = net.forward(nn->close_crd_test, nn->inpt_data_test, nn->pool);  // Forward
      // // auto loss_test = criterion(outputs, nn->label_test);
      // outputs = net.forward(nn->close_crd, nn->inpt_data, nn->pool);  // Forward
      // auto loss_test = criterion(outputs, nn->label);

      loss.backward();  // 反向传播
      optimizer.step();  // 更新参数

      if(loss.item<double>() < min_loss){
        NN_model_save(nn, net, DIR);
        min_loss = loss.item<double>();
      }

      if(batch_index == 0 && epoch % 100 ==0) printf("[%d] Loss: %e test_loss: %e min_Loss: %e  \n", epoch, loss.item<double>(), loss.item<double>(), min_loss);
      batch_index++;
    }
  }
  std::cout << "Trainning done! Save NN model... ";
  NN_model_save(nn, net, DIR);
  std::cout << "Done! " << std::endl;

} 

void CNN_test(GlobalMesh  *global, Neural_Network *nn, std::string DIR){
  std::cout << "CNN_test..." << std::endl;

  Net net(nn->pool, nn->fc_hidden); 
  NN_model_load(nn, net, DIR);
  net.to(torch::Device(torch::kCPU));
  net.eval();

  // nn->close_crd = nn->close_crd.to(torch::kFloat).to(torch::Device(torch::kCUDA));
  // nn->particle_crd_v = nn->particle_crd_v.to(torch::kFloat).to(torch::Device(torch::kCUDA));
  // Printf_time();
  Printf_time();

  auto predicted = net.forward(nn->close_crd.to(torch::kFloat), nn->particle_crd_v.slice(1, 3, 6).to(torch::kFloat), nn->pool).argmax(1);  //Forward
  
  Printf_time();
  int* search_result;
  nn->predicted.resize_({nn->inpt_num});

  for(int i = 0; i < nn->inpt_num; i++){
    nn->predicted[i] = nn->close_el[i].view({-1})[predicted[i].item<int>()];
  }
  std::cout << " CNN_test Done!" << std::endl;
}

void NN_model_save(Neural_Network *nn, Net& net, std::string DIR){
  
  torch::save(net.conv1, DIR + "/NN_model/conv1.bin");
  // torch::save(net.conv2, DIR + "/NN_model/conv2.bin");
  // torch::save(net.fc_crd, DIR + "/NN_model/fc_crd.bin");
  torch::save(net.fc1, DIR + "/NN_model/fc1.bin");
  torch::save(net.fc2, DIR + "/NN_model/fc2.bin");
}

void NN_model_load(Neural_Network *nn, Net& net, std::string DIR){
  torch::load(net.conv1, DIR + "/NN_model/conv1.bin");
  // torch::load(net.conv2, DIR + "/NN_model/conv2.bin");
  // torch::load(net.fc_crd, DIR + "/NN_model/fc_crd.bin");
  torch::load(net.fc1, DIR +  + "/NN_model/fc1.bin");
  torch::load(net.fc2, DIR + "/NN_model/fc2.bin");
}