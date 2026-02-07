# MS-MPI サンプルプログラム

## 環境構築

### 必要なソフトウェア

| ソフトウェア | 用途 | ダウンロード元 |
|---|---|---|
| MS-MPI ランタイム (`msmpisetup.exe`) | `mpiexec` による MPI プログラムの実行 | [Microsoft MPI](https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi) |
| MS-MPI SDK (`msmpisdk.msi`) | ヘッダー (`mpi.h`) とライブラリ (`msmpi.lib`) | 同上 |
| C コンパイラ (以下のいずれか) | ソースコードのコンパイル | |
| - Visual Studio 2022 (MSVC) | 推奨 | [Visual Studio](https://visualstudio.microsoft.com/) |
| - MinGW-w64 (GCC) | 軽量な代替手段 | [MinGW-w64](https://www.mingw-w64.org/) |

### インストール手順

1. **MS-MPI ランタイム**と **MS-MPI SDK** の両方をインストールする（ランタイムだけではコンパイルできません）
2. **Visual Studio 2022** をインストールし、「C++ によるデスクトップ開発」ワークロードを選択する

### インストール確認

インストール後、以下のファイルが存在することを確認してください。

```
C:\Program Files\Microsoft MPI\Bin\mpiexec.exe          （ランタイム）
C:\Program Files (x86)\Microsoft SDKs\MPI\Include\mpi.h （SDK ヘッダー）
C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64\msmpi.lib （SDK ライブラリ）
```

### MSVC 環境変数の設定

MSVC (`cl`) を使うには、ビルド前に環境変数を読み込む必要があります。以下のいずれかの方法を使ってください。

**方法1**: スタートメニューから「x64 Native Tools Command Prompt for VS 2022」を開く

**方法2**: 既存のコマンドプロンプトで以下を実行する
```cmd
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```

環境変数が正しく設定されたことを確認：
```cmd
cl
```
バージョン情報が表示されれば準備完了です。

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
