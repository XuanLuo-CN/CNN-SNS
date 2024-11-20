#! /bin/tcsh -f

######################### Set Parameter #############################

setenv   CASE           Cavity
setenv   MODEL_ID       0.008
setenv   NP             10
setenv   EPOCH_max      100000
setenv   V_limit        0
setenv   HOW_close      9
setenv   POOL           3             # int n | 0
setenv   REPEAT         0              # 0 | 1
setenv   NN_OPT         test_for_Re          # train | test | test_for_Re

setenv   learn_rate             0.00075
setenv   sampling               1
setenv   NP_NN                  30
setenv   Conv1_size             1
setenv   Conv2_kernel_size      3
setenv   FC_hidden              512
setenv   BATCH_SIZE              14000
setenv   PAD_AROUND              0.3

######################################################################
mpiexec -n $NP_NN /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/NN_test $MODEL_ID $NP $EPOCH_max $V_limit $HOW_close $POOL $NN_OPT $Conv1_size $Conv2_kernel_size $FC_hidden $NP_NN $sampling $learn_rate $BATCH_SIZE $PAD_AROUND $CASE $REPEAT


setenv   MODEL_ID       0.005
mpiexec -n $NP_NN /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/NN_test $MODEL_ID $NP $EPOCH_max $V_limit $HOW_close $POOL $NN_OPT $Conv1_size $Conv2_kernel_size $FC_hidden $NP_NN $sampling $learn_rate $BATCH_SIZE $PAD_AROUND $CASE $REPEAT

setenv   MODEL_ID       0.004
mpiexec -n $NP_NN /public/home/graduate/sFlow-LUO2/Neural_Network/source/CNN_v3/build/NN_test $MODEL_ID $NP $EPOCH_max $V_limit $HOW_close $POOL $NN_OPT $Conv1_size $Conv2_kernel_size $FC_hidden $NP_NN $sampling $learn_rate $BATCH_SIZE $PAD_AROUND $CASE $REPEAT
