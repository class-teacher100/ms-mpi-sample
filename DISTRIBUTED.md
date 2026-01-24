# MS-MPI 分散実行ガイド

複数のPCでMPIプログラムを実行する方法を説明します。

## 前提条件

### 両方のPCで必要なもの
1. **MS-MPI SDK と Runtime のインストール**
   - MS-MPI SDK: コンパイルに必要
   - MS-MPI Runtime: 実行に必要
   - ダウンロード: https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi

2. **実行ファイルの配置**
   - 両方のPCの同じパスに実行ファイルを配置
   - 例: `C:\Users\cre\mpi\monte_carlo_pi.exe`

3. **ネットワーク設定**
   - 両方のPCが同じLANに接続されていること
   - お互いにpingで疎通確認できること

## セットアップ手順

### 1. ホスト名とIPアドレスの確認

各PCで以下のコマンドを実行：

```cmd
hostname
ipconfig
```

例：
- PC1: hostname=DESKTOP-PC1, IP=192.168.1.10
- PC2: hostname=DESKTOP-PC2, IP=192.168.1.20

### 2. ホストファイルの作成

実行する側のPC（マスター）で、ホストファイルを作成します。

**hostfile.txt** の例：
```
DESKTOP-PC1:4
DESKTOP-PC2:4
```

または IPアドレスで指定：
```
192.168.1.10:4
192.168.1.20:4
```

フォーマット: `ホスト名またはIP:プロセス数`

### 3. ファイアウォールの設定

両方のPCで、Windowsファイアウォールを設定：

1. **受信規則の追加**
   - Windowsセキュリティ → ファイアウォールとネットワーク保護 → 詳細設定
   - 受信の規則 → 新しい規則
   - プログラム → `C:\Program Files\Microsoft MPI\Bin\smpd.exe` を選択
   - 接続を許可する
   - ドメイン、プライベート、パブリックすべてチェック

2. **または簡易的にポートを開放**
   ```cmd
   netsh advfirewall firewall add rule name="MPI" dir=in action=allow protocol=TCP localport=8676-8677
   ```

### 4. 認証設定（必要に応じて）

同じユーザー名とパスワードを使用するか、以下のいずれかを設定：

#### オプション A: 環境変数で認証情報を設定
```cmd
set MPIEXEC_USERNAME=ユーザー名
set MPIEXEC_PASSWORD=パスワード
```

#### オプション B: mpiexec の -user オプション使用
実行時に `-user` と `-pwd` オプションを指定

### 5. smpd サービスの起動確認

両方のPCで smpd (MPI デーモン) が実行されているか確認：

```cmd
sc query smpd
```

停止している場合は起動：
```cmd
sc start smpd
```

または手動でインストール：
```cmd
smpd -install
smpd -start
```

## 実行方法

### 基本的な実行コマンド

マスターPCから以下のコマンドを実行：

```cmd
mpiexec -machinefile hostfile.txt monte_carlo_pi.exe
```

### サンプル数を指定して実行

```cmd
mpiexec -machinefile hostfile.txt monte_carlo_pi.exe 1000000000
```

### 明示的にプロセス数を指定

```cmd
mpiexec -n 8 -machinefile hostfile.txt monte_carlo_pi.exe
```

### 認証情報を指定する場合

```cmd
mpiexec -user ユーザー名 -pwd パスワード -machinefile hostfile.txt monte_carlo_pi.exe
```

## トラブルシューティング

### エラー: "Unable to connect to hostname"

**原因**: ネットワーク接続の問題
**解決策**:
1. ping でホスト名/IPアドレスに疎通確認
   ```cmd
   ping DESKTOP-PC2
   ping 192.168.1.20
   ```
2. ファイアウォール設定を確認
3. smpd サービスが起動しているか確認

### エラー: "Authentication failed"

**原因**: 認証情報の不一致
**解決策**:
1. 両方のPCで同じユーザー名とパスワードを使用
2. または `-user` と `-pwd` オプションで明示的に指定

### エラー: "Unable to find executable"

**原因**: 実行ファイルのパスが一致していない
**解決策**:
1. 両方のPCの同じパスに実行ファイルを配置
2. または共有フォルダを使用
3. または `-wdir` オプションで作業ディレクトリを指定

### プロセスの割り振り確認

プログラムの出力を見て、どのホストでどのランクが実行されているか確認できます。
hello_mpi.exe を実行すると、プロセッサ名が表示されるので確認しやすいです。

```cmd
mpiexec -machinefile hostfile.txt hello_mpi.exe
```

出力例：
```
Hello from processor DESKTOP-PC1, rank 0 out of 8 processes
Hello from processor DESKTOP-PC1, rank 1 out of 8 processes
Hello from processor DESKTOP-PC1, rank 2 out of 8 processes
Hello from processor DESKTOP-PC1, rank 3 out of 8 processes
Hello from processor DESKTOP-PC2, rank 4 out of 8 processes
Hello from processor DESKTOP-PC2, rank 5 out of 8 processes
Hello from processor DESKTOP-PC2, rank 6 out of 8 processes
Hello from processor DESKTOP-PC2, rank 7 out of 8 processes
```

## パフォーマンスの考慮事項

- ネットワーク通信のオーバーヘッドにより、必ずしも2倍速くなるとは限りません
- monte_carlo_pi.c は通信量が少ない（最後のMPI_Reduceのみ）ため、分散実行の効果が出やすい
- より複雑な通信パターンを持つプログラムでは、ネットワーク速度が重要になります

## 参考

- MS-MPI 公式ドキュメント: https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi
- mpiexec のオプション一覧: `mpiexec -help2`
