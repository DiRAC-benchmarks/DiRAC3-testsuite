 
Clover Version    1.100
       MPI Version
    OpenMP Version
   Task Count     16
 Thread Count:     4
 
 Clover will run from the following input:-
 
*clover                                                                                                                                               
                                                                                                                                                      
 state 1 density=0.2 energy=1.0                                                                                                                       
 state 2 density=1.0 energy=2.5 geometry=cuboid xmin=0.0 xmax=5.0 ymin=0.0 ymax=2.0 zmin=0.0 zmax=4.0                                                 
                                                                                                                                                      
 x_cells=10                                                                                                                                           
 y_cells=10                                                                                                                                           
 z_cells=10                                                                                                                                           
                                                                                                                                                      
 xmin=0.0                                                                                                                                             
 ymin=0.0                                                                                                                                             
 zmin=0.0                                                                                                                                             
 xmax=10.0                                                                                                                                            
 ymax=10.0                                                                                                                                            
 zmax=10.0                                                                                                                                            
                                                                                                                                                      
 initial_timestep=0.1                                                                                                                                 
 max_timestep=0.1                                                                                                                                     
 end_step=2                                                                                                                                           
 end_time=1000.1                                                                                                                                      
 profiler_on=1                                                                                                                                        
                                                                                                                                                      
*endclover                                                                                                                                            
 
 Initialising and generating
 
 Reading input file
 
 Reading specification for state            1
 
            state density   0.2000E+00
             state energy   0.1000E+01
 
 Reading specification for state            2
 
            state density   0.1000E+01
             state energy   0.2500E+01
      state geometry cuboid
               state xmin   0.0000E+00
               state xmax   0.5000E+01
               state ymin   0.0000E+00
               state ymax   0.2000E+01
               state zmin   0.0000E+00
               state zmax   0.4000E+01
 
                   x_cells          10
                   y_cells          10
                   z_cells          10
                      xmin  0.0000E+00
                      ymin  0.0000E+00
                      zmin  0.0000E+00
                      xmax  0.1000E+02
                      ymax  0.1000E+02
                      zmax  0.1000E+02
         initial_timestep   0.1000E+00
              max_timestep  0.1000E+00
                  end_step           2
                  end_time  0.1000E+04
               Profiler on
 
     Using Fortran Kernels
 
 Input read finished.
 
 Setting up initial geometry
 
 
 Decomposing the mesh into            2  by            2  by            4  chunk
 s
 
 Generating chunks
 
 Problem initialised and generated
 
 Time   0.000000000000000E+000
                       Volume            Mass         Density        Pressure Internal Energy  Kinetic Energy    Total Energy
 step:      0      0.1000E+04      0.2320E+03      0.2320E+00      0.1168E+00      0.2920E+03      0.0000E+00      0.2920E+03
 
 Starting the calculation
 Step       1 time   0.0000000 control    sound    timestep   1.00E-01       1,       1 x  5.00E-01 y  5.00E-01
 Wall clock   4.560899734497070E-002
 Average time per cell   4.560899734497070E-004
 Step time per cell      4.560899734497070E-004
 Step       2 time   0.1000000 control    sound    timestep   1.00E-01       1,       1 x  5.00E-01 y  5.00E-01
 
 Time   0.200000000000000     
                       Volume            Mass         Density        Pressure Internal Energy  Kinetic Energy    Total Energy
 step:      2      0.1000E+04      0.2320E+03      0.2320E+00      0.1164E+00      0.2911E+03      0.9142E+00      0.2920E+03
 
 
 Calculation complete
 Clover is finishing
 Wall clock   9.875512123107910E-002
 First step overhead -3.812074661254883E-003
 
Profiler Output                 Time            Percentage
Timestep              :          0.0006          0.6024
Ideal Gas             :          0.0006          0.6147
Viscosity             :          0.0007          0.7038
PdV                   :          0.0018          1.8440
Revert                :          0.0003          0.3037
Acceleration          :          0.0020          1.9898
Fluxes                :          0.0005          0.5319
Cell Advection        :          0.0027          2.7341
Momentum Advection    :          0.0098          9.9418
Reset                 :          0.0005          0.4821
Halo Exchange         :          0.0788         79.8223
Summary               :          0.0004          0.3646
Visit                 :          0.0000          0.0000
Total                 :          0.0987         99.9351
The Rest              :          0.0001          0.0649
 FLOPS: Cell Adv                    : -2.213609288845141E+019
 FLOPS: Mom Adv                     :   89464.0000000000     
 Momentum Adv Bytes                 :  0.000000000000000E+000
