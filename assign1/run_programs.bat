@echo off

REM Define the input sizes
set input_sizes=64 128 256 512 1024 2048 4096 8192 16384 32768

REM Define the number of runs for each program
set num_runs=10

REM Loop through each input size
for %%s in (%input_sizes%) do (
    echo Running programs for input size %%s...
    
    REM Sequential program
    for /l %%i in (1, 1, %num_runs%) do (
        echo | set /p="Sequential program run %%i: "
        mXv.exe %%s %%s | findstr /r /c:"secs"
    )
    
    REM OpenMP naive program
    for /l %%i in (1, 1, %num_runs%) do (
        echo | set /p="OpenMP naive program run %%i: "
        mXv_openmp.exe %%s %%s | findstr /r /c:"secs"
    )
    
    REM MPI naive program
    for /l %%i in (1, 1, %num_runs%) do (
        echo | set /p="MPI naive program run %%i: "
        mpiexec -n 4 mXv_mpi.exe %%s %%s | findstr /r /c:"secs"
    )
    
    REM OpenMP tiling program
    for /l %%i in (1, 1, %num_runs%) do (
        echo | set /p="OpenMP tiling program run %%i: "
        mXv_openmp_tiled.exe %%s %%s | findstr /r /c:"secs"
    )
    
    REM MPI tiling program
    for /l %%i in (1, 1, %num_runs%) do (
        echo | set /p="MPI tiling program run %%i: "
        mpiexec -n 4 mXv_mpi_tiled.exe %%s %%s | findstr /r /c:"secs"
    )
)
