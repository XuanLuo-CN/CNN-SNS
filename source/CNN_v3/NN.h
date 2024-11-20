#include "sFlow_source/hddm_types.h"
#include <torch/torch.h>
#include "model_v2.h"

////////////////////////////////////////////////////////////////////////////
extern char* CASE; 
extern char* MODEL_ID;

 typedef struct
{ 
  int np, partitions, repeat;
  int pool, pool_opt, pad, pool_pad;
  int how_close;
  float v_limit;
  int dim;
  int length; //length of the cube before padding
  int pool_length; //length of the cube after padding
  int epoch_max, inpt_num;
  int opt;
  int n_beyond;


//NN parametare
  double opt_rate;
  int np_NN;
  int conv1_size;
  int conv2_kernel_size;
  int fc_hidden;
  int sampling;
  int batch_size;
  int final_length[3];
  double pad_around;

  torch::Tensor el_cube = torch::empty({0,0,0}, torch::dtype(torch::kInt)); //cube that contain elements' id (Sort in spatial order)
  torch::Tensor pool_el_cube = torch::empty({0,0,0}, torch::dtype(torch::kInt)); //cube that contain elements' id (Sort in spatial order) after pool
  torch::Tensor crd_cube = torch::empty({0,0,0}, torch::dtype(torch::kDouble)); //cube that contain elements' crd (whether pool or not)
  torch::Tensor convMesh = torch::zeros({0, 0}, torch::dtype(torch::kDouble)); //Barycentric crds of each elements (Sort by id)

  torch::Tensor particle_crd_v = torch::zeros({0, 7}, torch::dtype(torch::kDouble));
  torch::Tensor particle_elm = torch::zeros({0, 3}, torch::dtype(torch::kInt));
  torch::Tensor close_crd = torch::zeros({0, 0, 0, 0}, torch::dtype(torch::kDouble)); 
  torch::Tensor close_el = torch::zeros({0, 0}, torch::dtype(torch::kInt)); 
  torch::Tensor label = torch::zeros({0, 1}, torch::dtype(torch::kLong));

  torch::Tensor inpt_layer = torch::zeros({0, 0}, torch::dtype(torch::kDouble)); 
  torch::Tensor predicted = torch::empty({0}, torch::dtype(torch::kInt));

} Neural_Network;

void NN_init(GlobalMesh *global, Neural_Network *nn, char* argv[]);
void Find_reconstruct_length(GlobalMesh *global, int* length);
void Adv2csv(GlobalMesh *global, std::string DIR);

void Mesh_csv2torch(GlobalMesh *global, Neural_Network *nn, std::string DIR);
void Mesh_torch_save(Neural_Network *nn, std::string DIR);
void Mesh_torch_load(Neural_Network *nn, std::string DIR);

void Particle_csv_2_label(Neural_Network *nn, std::string DIR, GlobalMesh *global);
void Particle_csv2torch(Neural_Network *nn, std::string DIR, int repeat, int Re);
void Particle_torch_save(Neural_Network *nn, std::string DIR, int time_step);
void Particle_torch_load(Neural_Network *nn, std::string DIR, int time_step);


void CNN_train(GlobalMesh  *global, Neural_Network *nn, std::string DIR);
void CNN_test(GlobalMesh  *global, Neural_Network *nn, std::string DIR);
void NN_model_save(Neural_Network *nn, Net& net, std::string DIR);
void NN_model_load(Neural_Network *nn, Net& net, std::string DIR);

///////////////////////// Test ////////////////////////////
void Repeat_test(GlobalMesh  *global, Neural_Network *nn, int n_repeat);
void Time_test(GlobalMesh *global, Neural_Network *nn);

/////////////////////// nn_Print ////////////////////////////
void Printf_conv1_result(GlobalMesh *global, Neural_Network *nn);
void Printf_crd_cube(Neural_Network *nn);
void Printf_el_cube(Neural_Network *nn);
void Printf_pool_el_cube(Neural_Network *nn);
void Printf_basic_data(GlobalMesh *global, Neural_Network *nn, std::string DIR);
void Printf_basic_test_result(Neural_Network *nn, int *search_times, std::string DIR, int Re);
void Printf_test_result(Neural_Network *nn, int *search_times, std::string DIR, int Re);
void Printf_time();

void brycntr(double xx, double yy, double zz, double x[], double y[], double z[], double *lam1, double *lam2, double *lam3, double *lam4 );
void search_element(int gie, GlobalMesh  *global, double pre_x, double pre_y, double pre_z, int* result);
void Splitter(int range, int nproc, int *start, int *end );



#ifdef __cplusplus  
extern "C"{   
#endif
void NN(GlobalMesh *global, char* argv[]);
#ifdef __cplusplus  
}  
#endif