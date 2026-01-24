# MPI Hello World プログラム

## ファイル説明
- `hello_mpi.c`: MPIのHello Worldプログラム

## コンパイル方法

### Visual Studio（MSVC）の場合
```cmd
cl /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" hello_mpi.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
```

### MinGW-w64の場合
```cmd
gcc -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" hello_mpi.c -o hello_mpi.exe -lmsmpi
```

注: パスはMS-MPIのインストール先に応じて調整してください。

## 実行方法

### 4つのプロセスで実行
```cmd
mpiexec -n 4 hello_mpi.exe
```

### 2つのプロセスで実行
```cmd
mpiexec -n 2 hello_mpi.exe
```

## 期待される出力例
```
Hello from processor DESKTOP-XXXXX, rank 0 out of 4 processes
Hello from processor DESKTOP-XXXXX, rank 1 out of 4 processes
Hello from processor DESKTOP-XXXXX, rank 2 out of 4 processes
Hello from processor DESKTOP-XXXXX, rank 3 out of 4 processes
```

注: 出力の順序は実行ごとに異なる場合があります。
