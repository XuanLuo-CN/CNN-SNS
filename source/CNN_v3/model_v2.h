
#include <torch/torch.h>

struct Net : torch::nn::Module {
  Net(int64_t pool, int64_t fc_hidden) {

    conv1 = register_module("conv1", torch::nn::Conv3d(torch::nn::Conv3dOptions(1, 1, {3, 1, 1}).stride({3, 1, 1})));//.padding({1, 0, 0})))
    pool1 = register_module("pool1", torch::nn::MaxPool3d(torch::nn::MaxPool3dOptions({3, 1, 1}).stride({3, 1, 1})));
    // conv2 = register_module("conv2", torch::nn::Conv3d(torch::nn::Conv3dOptions(64, 8, 3).stride(1).padding({1, 1, 1})));
    // pool2 = register_module("pool2", torch::nn::MaxPool3d(torch::nn::MaxPool3dOptions(2).stride(1).padding({0, 0, 1})));
    dropout = register_module("dropout", torch::nn::Dropout(0.1));

    // fc_crd = register_module("fc_crd1", torch::nn::Linear(3, 99 * 4));
    fc1 = register_module("fc1", torch::nn::Linear(1 * pool * pool * pool, fc_hidden));
    fc2 = register_module("fc2", torch::nn::Linear(fc_hidden, pool * pool * pool));
  }


  torch::Tensor forward(torch::Tensor x, torch::Tensor label, int pool) {
    // auto inpt_data = torch::relu(fc_crd(label.slice(1, 0, 3)));
    // inpt_data = inpt_data.repeat({1,1,3,3,11});
    auto inpt_data = label.unsqueeze(2).unsqueeze(3).repeat({1, pool, pool, pool});
    x = inpt_data - x;
    x = torch::relu(conv1(x.unsqueeze(1)));
    // x = torch::nn::functional::layer_norm(x, torch::nn::functional::LayerNormFuncOptions({pool, pool, pool}).eps(2e-5));
    // x = pool1(x);
    // x = torch::relu(conv2(x));
    // x = pool2(x);
    // x = torch::cat({inpt_data, x.view({x.size(0), -1})} , 1);
    x = torch::relu(fc1(x.view({x.size(0), -1}))); 
    // x = torch::relu(fc1(inpt_data.view({inpt_data.size(0), -1})));
    x =  dropout(x);
    x = fc2(x); 
    return torch::log_softmax(x, /*dim=*/1);
  }
  
  // torch::nn::ConvTranspose3d deconv1{nullptr}; // 添加转置卷积层
  torch::nn::Conv3d conv1{nullptr};
  torch::nn::Conv3d conv2{nullptr};
  torch::nn::Linear fc1{nullptr}, fc2{nullptr};
  torch::nn::MaxPool3d pool2{nullptr}, pool1{nullptr};
  torch::nn::Dropout dropout{nullptr};
};