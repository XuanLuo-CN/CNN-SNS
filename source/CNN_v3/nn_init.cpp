
#include "NN.h"
//////////////////////////////////////

void NN_init(GlobalMesh *global, Neural_Network *nn, char* argv[]){
  int length[3];
  Find_reconstruct_length(global, length);
  nn->length = length[0];

  nn->dim = 3;
  nn->n_beyond = 0;
  nn->partitions = atoi(argv[1]);
  nn->np = atoi(argv[2]);
  nn->epoch_max = atoi(argv[3]);
  nn->v_limit = atof(argv[4]);
  nn->how_close = atoi(argv[5]);
  if(std::string(argv[7]) == "train") nn->opt = 1;
  else if(std::string(argv[7]) == "test") nn->opt = 0;
  else if(std::string(argv[7]) == "test_for_Re") nn->opt = 2;
  
  nn->conv1_size = atoi(argv[8]);
  nn->conv2_kernel_size = atoi(argv[9]);
  nn->fc_hidden = atoi(argv[10]);
  nn->np_NN = atoi(argv[11]);
  nn->sampling = atoi(argv[12]);
  nn->opt_rate = atof(argv[13]);
  nn->batch_size = atoi(argv[14]);
  nn->pad_around = atof(argv[15]);
  if(std::string(argv[17]) == "0") nn->repeat = 0;
  else if(std::string(argv[17]) == "1") nn->repeat = global->nnodes / 40;

  nn->pad = static_cast<int>(nn->pad_around * nn->length);
  if(std::string(argv[6]) != "off" && argv[6] != "0"){
    nn->pool_opt = atoi(argv[6]);
    nn->pool_length = nn->length / nn->pool_opt;
    nn->pool_pad = nn->pad / nn->pool_opt;
    nn->pool = nn->how_close / nn->pool_opt * 2 + 1;

    if (!strcmp(CASE, "Cavity")) nn->final_length[0] = nn->pool_length + 2 * nn->pool_pad;
    else if (!strcmp(CASE, "Sphere"))nn->final_length[0] = 2 * nn->pool_length + 2 * nn->pool_pad;

    nn->final_length[1] = nn->pool_length + 2 * nn->pool_pad;
    nn->final_length[2] = nn->pool_length + 2 * nn->pool_pad;
  }
  // else if
  
  if(COM_ID.myprocid == 0) {
    std::cout << "  nn->final_length: " << nn->final_length[0] << ", " << nn->final_length[1] << ", " << nn->final_length[2] << std::endl;
    std::cout << "  pad_around: " << nn->pad_around * nn->length << "  pad: " << nn->pad << "  pool_pad: " << nn->pool_pad << std::endl;
  }
}


void Find_reconstruct_length(GlobalMesh *global, int* length){
  if (!strcmp(CASE, "Cavity")) length[0] = std::ceil(pow(global->nel, 1.0 / 3));
  else if (!strcmp(CASE, "Sphere")) length[0] = std::ceil(pow(global->nel / 2, 1.0 / 3));
  length[1] = std::ceil(1.0 * global->nel / length[0] / 2);
  length[2] = std::ceil(1.0 * length[1] / length[0]);
  if(COM_ID.myprocid == 0) std::cout << "length[0]: " << length[0] << "    length[1]: " << length[1] << "    length[2]: " << length[2] << std::endl;
}


void Adv2csv(GlobalMesh *global, std::string DIR){
  std::cout << "Adv2csv..." << std::endl;
  std::ofstream crdfile(DIR + "/Sort_data/" + "crd.csv");
  std::ofstream nopfile(DIR + "/Sort_data/" + "nop.csv");
  crdfile << global->nnodes <<"\n";
  nopfile << global->nel <<"\n";

  for(int i=0; i<global->nnodes; i+=1){
    crdfile << global->crd[3 * i + 0] <<","<< global->crd[3 * i + 1] <<","<< global->crd[3 * i + 2] << "\n";
  }
  for(int i=0; i<global->nel; i+=1){
    nopfile << global->nop[4 * i + 0] <<","<< global->nop[4 * i + 1] <<","<< global->nop[4 * i + 2] <<","<< global->nop[4 * i + 3] << "\n";
  }

  nopfile <<std::endl;
  crdfile <<std::endl;
  crdfile.close();
  nopfile.close();
}


void Mesh_csv2torch(GlobalMesh *global, Neural_Network *nn, std::string DIR){
  std::cout << "Mesh_csv2torch..." << std::endl;
  
  std::cout << "Make_el_cube..." << std::endl;
  nn->el_cube.resize_({2 * nn->length, nn->length, nn->length});
  std::ifstream sort_el(DIR + "/Sort_data/sort_el.csv");
  std::string line;
  std::getline(sort_el, line);
  std::cout << "  nline: " << atoi(line.c_str()) <<"   nn->length: " << nn->length * nn->length * nn->length * 2 <<std::endl;
  for(int i=0; i < 2 * nn->length; i++){
    std::cout << "[" << i << "]" << std::endl;
    for(int j=0; j < nn->length; j++){
      for(int k=0; k < nn->length; k++){
        std::getline(sort_el, line);
        nn->el_cube[i][j][k] = atoi(line.c_str());
      }
    }
  }
  torch::nn::functional::PadFuncOptions padOption(std::vector<int64_t>{nn->pad, nn->pad, nn->pad, nn->pad, nn->pad, nn->pad});
  padOption.mode(torch::kReplicate);
  nn->el_cube = torch::nn::functional::pad(nn->el_cube.unsqueeze(0).unsqueeze(0).to(torch::kFloat), padOption).squeeze(0).squeeze(0).to(torch::kInt);

  std::cout << "  Read_conv1..." << std::endl;
  nn->convMesh.resize_({global->nel, nn->dim}).zero_();
  std::ifstream conv1(DIR + "/Sort_data/conv1.csv");
  std::getline(conv1, line);
  std::cout << "  nline: " << atoi(line.c_str()) <<"   global->nel: " << global->nel<<std::endl;
  for(int i=0; i < global->nel; i++){
    if(i % 1000000 == 0) std::cout << i << std::endl;
    std::getline(conv1, line);
    std::string cell;
    std::stringstream ss(line);
    for(int j=0; j < 3; j++){
      std::getline(ss, cell, ',');
      nn->convMesh[i][j] = atof(cell.c_str());
    }
  }

  std::cout << "  Make_crd_cube..." << std::endl;
  nn->crd_cube.resize_({3 * nn->final_length[0], nn->final_length[1], nn->final_length[2]});
  if(nn->pool_opt != 0){
    nn->pool_el_cube = std::get<0>(torch::adaptive_max_pool3d(nn->el_cube.unsqueeze(0).to(torch::kFloat), {nn->final_length[0], nn->final_length[1], nn->final_length[2]})).squeeze(0).toType(torch::kInt);

    for(int i=0; i < nn->final_length[0]; i++){
      for(int j=0; j < nn->final_length[1]; j++){
        for(int k=0; k < nn->final_length[2]; k++){
          if(nn->pool_el_cube[i][j][k].item<int>() >= global->nel){
            std::cout << "!!!!!!!!!!!!!!!" << nn->pool_el_cube[i][j][k].item<int>() <<" "<< i << " "<< j << " "<< k << std::endl;
            nn->pool_el_cube[i][j][k] = global->nel - 1;
          } 
          nn->crd_cube[3 * i][j][k] = nn->convMesh[nn->pool_el_cube[i][j][k].item<int>()][0];
          nn->crd_cube[3 * i + 1][j][k] = nn->convMesh[nn->pool_el_cube[i][j][k].item<int>()][1];
          nn->crd_cube[3 * i + 2][j][k] = nn->convMesh[nn->pool_el_cube[i][j][k].item<int>()][2];
        }
      }
    }
  }
  // else if
  Mesh_torch_save(nn, DIR);
}


void Mesh_torch_save(Neural_Network *nn, std::string DIR){
  std::cout << "Mesh_torch_save..." <<std::endl;
  torch::save(nn->convMesh, DIR + "/Torch_data/" + "convMesh.bin");
  torch::save(nn->crd_cube, DIR + "/Torch_data/" + "crd_cube.bin");
  torch::save(nn->el_cube, DIR + "/Torch_data/" + "el_cube.bin");
  torch::save(nn->pool_el_cube, DIR + "/Torch_data/" + "pool_el_cube.bin");
}


void Mesh_torch_load(Neural_Network *nn, std::string DIR){
  if(COM_ID.myprocid == 0) std::cout << "Mesh_torch_load..." <<std::endl;
  torch::load(nn->convMesh, DIR + "/Torch_data/" + "convMesh.bin");
  torch::load(nn->crd_cube, DIR + "/Torch_data/" + "crd_cube.bin");
  torch::load(nn->el_cube, DIR + "/Torch_data/" + "el_cube.bin");
  torch::load(nn->pool_el_cube, DIR + "/Torch_data/" + "pool_el_cube.bin");

  if(COM_ID.myprocid == 0){
    std::cout << "  crd_cube.size: " << nn->crd_cube.size(0) << ", " << nn->crd_cube.size(1) << ", " << nn->crd_cube.size(2) << std::endl;
    std::cout << "  pool_el_cube.size: " << nn->pool_el_cube.size(0) << "  pad: " << nn->pool_el_cube.size(1) << "  pool_pad: " << nn->pool_el_cube.size(2) << std::endl;

  }
}


void Particle_csv2torch(Neural_Network *nn, std::string DIR, int repeat, int Re){
  std::cout << "Particle_csv2torch..."<< std::endl;
  int num, sampled_n_line;

  double v, crd[6];
  int center[3], half_length = (nn->pool - 1) / 2;
  std::string line;

    std::ifstream read_file(DIR + "/SNS_result/Re" + std::to_string(Re) + ".csv");
    if (!read_file.is_open()) std::cerr << "Error opening file while reading: " << DIR + "/SNS_result/Re" + std::to_string(Re) + ".csv" << std::endl;
    
    if(nn->pool != 0){
      std::getline(read_file, line);
      int n_line = atoi(line.c_str());
      sampled_n_line = (n_line - 1) / nn->sampling + 1; // How much (n_line - 1) can be divided by nn->sampling, + index_0
      std::cout <<"  nline: "<< n_line <<"  sampled_n_line: "<< sampled_n_line << std::endl;
      if(repeat != 0) sampled_n_line = repeat;

      nn->close_crd.resize_({sampled_n_line, 3 * nn->pool, nn->pool, nn->pool});
      nn->close_el.resize_({sampled_n_line, nn->pool, nn->pool, nn->pool});
      nn->particle_crd_v.resize_({sampled_n_line, 7});
      nn->particle_elm.resize_({sampled_n_line, 3});
      // nn->label.resize_({sampled_n_line, 1});
      
      torch::Tensor min = torch::zeros({nn->pool * nn->pool * nn->pool}, torch::dtype(torch::kDouble));
      
      std::cout << "  Index close el and crd...\n  final_length: " << nn->final_length[0] <<", "<< nn->final_length[1] <<", "<< nn->final_length[2] << std::endl;
      std::cout << "  half_length: "<< half_length << std::endl;

      num = 0;
      for(int i = 0; i < n_line; i++){
        std::getline(read_file, line);
        if(i % nn->sampling != 0) continue;
        else if(i % 100000 == 0) std::cout <<"  ["<< i * 1.0 / n_line <<"]"<< std::endl;
        std::stringstream ss(line);
        std::string cell;
        for(int j = 0; j < 7; j++){
          std::getline(ss, cell, ',');
          nn->particle_crd_v[num][j] = atof(cell.c_str());
        }
        for(int j = 0; j < 3; j++){
          std::getline(ss, cell, ',');
          nn->particle_elm[num][j] = atoi(cell.c_str());
        }

        for (int j = 0; j < 3; j++){
          if (!strcmp(CASE, "Sphere")) center[j] = (nn->particle_crd_v[num][j + 3] * nn->pool_length * 2).floor().item<int>() + nn->pool_pad;
          else if (!strcmp(CASE, "Cavity")) center[j] = (nn->particle_crd_v[num][j + 3] * nn->pool_length).floor().item<int>() + nn->pool_pad;
        }
        nn->close_crd[num] = nn->crd_cube.slice(0, 3 * center[0] - 3 * half_length , 3 * center[0] + 3 * half_length + 3)
                                         .slice(1, center[1] - half_length, center[1] + half_length + 1)
                                         .slice(2, center[2] - half_length, center[2] + half_length + 1);

        nn->close_el[num] = nn->pool_el_cube.slice(0, center[0] - half_length , center[0] + half_length + 1)
                                            .slice(1, center[1] - half_length, center[1] + half_length + 1)
                                            .slice(2, center[2] - half_length, center[2] + half_length + 1);
        num++;

      }
      nn->inpt_num = num;
    }
    // else if

    if(repeat != 0){
      nn->inpt_num = repeat;
      for(int i = num; i < repeat; i++){
        nn->close_el[i] = nn->close_el[i - num];
        nn->close_crd[i] = nn->close_crd[i - num];
        nn->particle_crd_v[i] = nn->particle_crd_v[i - num];
        nn->particle_elm[i] = nn->particle_elm[i - num];
      }
    }
  
    std::cout << "  inpt_num: "<< nn->inpt_num << std::endl;
}


void Particle_csv_2_label(Neural_Network *nn, std::string DIR, GlobalMesh  *global){
  if(COM_ID.myprocid == 0) std::cout << "Particle_csv_2_label..."<< std::endl;
  int sampled_n_line, particle_elm[3], files[2];
  double v, crd[6], particle_crd_v[7];
  int center[3], half_length = (nn->pool - 1) / 2;
  std::string line;
  torch::Tensor close_crd, min = torch::zeros({nn->pool * nn->pool * nn->pool}, torch::dtype(torch::kDouble)), close_el;

    if (!strcmp(CASE, "Sphere")) {
        files[0] = 66600;
        files[1] = 66601;
    }
    else if (!strcmp(CASE, "Cavity")){
        files[0] = 100;
        files[1] = 101;
    }

  for (int time_step = files[0]; time_step < files[1]; time_step+=1){
    std::ifstream read_file(DIR + "/SNS_result/" + std::to_string(time_step) + ".csv");
    if (!read_file.is_open()) std::cerr << "Error opening file while reading: " << DIR + "/SNS_result/" + std::to_string(time_step) + ".csv" << std::endl;
    
    if(nn->pool != 0){
      std::getline(read_file, line);
      int n_line = atoi(line.c_str());
      sampled_n_line = (n_line - 1) / nn->sampling + 1;
      if(COM_ID.myprocid == 0) std::cout <<"  nline: "<< n_line <<"  sampled_n_line: "<< sampled_n_line << std::endl;

      int *start = new int[COM_ID.nproc];
      int *end = new int[COM_ID.nproc];
      Splitter(sampled_n_line, COM_ID.nproc, start, end);

      torch::Tensor label = torch::zeros({end[COM_ID.myprocid] - start[COM_ID.myprocid], 1}, torch::dtype(torch::kInt));
      

      int num = 0;
      for(int i = 0; i < n_line; i++){
        std::getline(read_file, line);

        if(i % nn->sampling != 0) continue;
        if(i < start[COM_ID.myprocid] * nn->sampling || i >= end[COM_ID.myprocid]  * nn->sampling) continue;
        if(COM_ID.myprocid == COM_ID.nproc - 1) if(i % 10000 == 0) std::cout <<"["<< num * 1.0 / (end[COM_ID.myprocid] - start[COM_ID.myprocid]) <<"]"<< std::endl;

        std::stringstream ss(line);
        std::string cell;
        for(int j = 0; j < 7; j++){
          std::getline(ss, cell, ',');
          particle_crd_v[j] = atof(cell.c_str());
        }
        for(int j = 0; j < 3; j++){
          std::getline(ss, cell, ',');
          particle_elm[j] = atoi(cell.c_str());
        }
        for (int j = 0; j < 3; j++){
          if (!strcmp(CASE, "Sphere")) center[j] = particle_crd_v[j + 3] * nn->pool_length * 2 + nn->pool_pad;
          else if (!strcmp(CASE, "Cavity")) center[j] = particle_crd_v[j + 3] * nn->pool_length + nn->pool_pad;
        }

        // std::cout << "particle_crd: " << nn->particle_crd_v[num][3] * nn->pool_length <<", "<< nn->particle_crd_v[num][4] * nn->pool_length <<", "<< nn->particle_crd_v[num][5] * nn->pool_length;
        // std::cout << "  center: " << center[0] <<", "<< center[1] <<", "<< center[2] <<std::endl;

        close_crd = nn->crd_cube.slice(0, 3 * center[0] - 3 * half_length , 3 * center[0] + 3 * half_length + 3)
                                         .slice(1, center[1] - half_length, center[1] + half_length + 1)
                                         .slice(2, center[2] - half_length, center[2] + half_length + 1);
        
        close_el = nn->pool_el_cube.slice(0, center[0] - half_length , center[0] + half_length + 1)
                                            .slice(1, center[1] - half_length, center[1] + half_length + 1)
                                            .slice(2, center[2] - half_length, center[2] + half_length + 1);
        for(int a = 0; a < nn->pool; a++){
          for(int b = 0; b < nn->pool; b++){
            for(int c = 0; c < nn->pool; c++){
              min[a*nn->pool*nn->pool + b*nn->pool + c] = 0;
              for(int d = 0; d < 3; d++) min[a*nn->pool*nn->pool + b*nn->pool + c] += torch::abs(close_crd[3 * a + d][b][c] - particle_crd_v[3 + d]);
            }
          }
        }
        label[num] = torch::argmin(min);
        num++;
      }
      torch::save(label, DIR + "/Torch_data/TMP_label/" + std::to_string(time_step) + "_label_" + std::to_string(COM_ID.myprocid) + ".bin");
      MPI_Barrier(MPI_COMM_WORLD);

      if(COM_ID.myprocid == 0){
        std::cout << "  combine_global_label..."<< std::endl;
        nn->label.resize_({sampled_n_line, 1});
        for(int i = 0; i < COM_ID.nproc; i++){
          torch::load(label, DIR + "/Torch_data/TMP_label/" + std::to_string(time_step) + "_label_" + std::to_string(i) + ".bin");
          nn->label.index({torch::indexing::Slice(start[i], end[i])}) = label;
        }
        torch::save(nn->label, DIR + "/Torch_data/" + std::to_string(time_step) + "_label.bin");
      }

    }
    // else if
    MPI_Barrier(MPI_COMM_WORLD);
  }
}