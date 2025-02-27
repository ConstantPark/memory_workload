# Memory_Workload

To get the performance, we provide three workloads (Vector Load/Add, GEMV).  
This code is tested on Apple Sillicon M1, so some API is only workking on Apple Platform.  
You can change API or Inline Assembly Code to optmize your core (RISC-V).  

You can compile these codes with the command "gcc source_code.c -O2" (w/ any special options).  
The gemv code should specify your BLAS Library (Our code is tested on Apple M1 Silicon).  
If you want to run the gemv code in linux system, you have to modify "include <your BLAS library>".  
You don't have to chage the code inside the main funciton (cblas_gemnv).  
