# MS-MPI サンプルプログラム

## ファイル説明
- `hello_mpi.c`: MPIのHello Worldプログラム
- `monte_carlo_pi.c`: モンテカルロ法によるπ推定プログラム

## コンパイル方法

### Visual Studio（MSVC）の場合
```cmd
cl /utf-8 /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" <ファイル名>.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
```

例:
```cmd
cl /utf-8 /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" hello_mpi.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
cl /utf-8 /I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" monte_carlo_pi.c /link /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" msmpi.lib
```

注: `/utf-8` オプションはエンコーディング警告（C4819）を回避するために推奨されます。

### MinGW-w64の場合
```cmd
gcc -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" hello_mpi.c -o hello_mpi.exe -lmsmpi
```

注: パスはMS-MPIのインストール先に応じて調整してください。

## 実行方法

### hello_mpi.exe の実行
```cmd
mpiexec -n 4 hello_mpi.exe
```

期待される出力例:
```
Hello from processor DESKTOP-XXXXX, rank 0 out of 4 processes
Hello from processor DESKTOP-XXXXX, rank 1 out of 4 processes
Hello from processor DESKTOP-XXXXX, rank 2 out of 4 processes
Hello from processor DESKTOP-XXXXX, rank 3 out of 4 processes
```

### monte_carlo_pi.exe の実行
```cmd
mpiexec -n 4 monte_carlo_pi.exe
mpiexec -n 8 monte_carlo_pi.exe 1000000000
```

期待される出力例:
```
pi = 3.14159265
```

注: 出力の順序は実行ごとに異なる場合があります。
