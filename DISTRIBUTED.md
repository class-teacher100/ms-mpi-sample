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

**重要な注意事項:**
- **`localhost` は使用しないでください** - 各PCから見て自分自身を指すため、意図した分散実行ができません
- 実際のホスト名またはIPアドレスを明示的に指定してください
- フォーマット: `ホスト名またはIP:プロセス数`
- 実行するPC自身も含めて、すべてのPCをホスト名で指定します

### 3. ファイアウォールの設定

両方のPCで、Windowsファイアウォールを設定：

1. **受信規則の追加（推奨）**
   - Windowsセキュリティ → ファイアウォールとネットワーク保護 → 詳細設定
   - 受信の規則 → 新しい規則
   - プログラム → `C:\Program Files\Microsoft MPI\Bin\mpiexec.exe` を選択
   - 接続を許可する
   - ドメイン、プライベート、パブリックすべてチェック

2. **または簡易的にポートを開放**
   ```cmd
   netsh advfirewall firewall add rule name="MS-MPI" dir=in action=allow protocol=TCP localport=8676-8677
   netsh advfirewall firewall add rule name="MS-MPI-UDP" dir=in action=allow protocol=UDP localport=8676-8677
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

### 5. MS-MPI Launch Service の起動

**重要**: MS-MPI v7以降では、古い`smpd`サービスではなく**MS-MPI Launch Service (MsMpiLaunchSvc)**を使用します。

両方のPCで以下のコマンドを実行（管理者権限が必要）：

#### 方法 A: MS-MPI Launch Service を使用（推奨）

```cmd
sc config MsMpiLaunchSvc start= auto
sc start MsMpiLaunchSvc
```

サービスの状態を確認：
```cmd
sc query MsMpiLaunchSvc
```

正常に起動していれば `STATE: 4 RUNNING` と表示されます。

#### 方法 B: spmd デーモンを使用

MS-MPI Launch Serviceが使えない場合は、各PCで`spmd`をデーモンモードで起動：

```cmd
spmd -d
```

注: `spmd.exe`は`%MSMPI_BIN%`環境変数で指定されたフォルダ（通常は`C:\Program Files\Microsoft MPI\Bin`）にあります。

## 実行方法

### 方法 1: hostfile を使用（推奨）

マスターPCから以下のコマンドを実行：

```cmd
mpiexec -hostfile hostfile.txt monte_carlo_pi.exe
```

サンプル数を指定：
```cmd
mpiexec -hostfile hostfile.txt monte_carlo_pi.exe 1000000000
```

注: `-machinefile` の代わりに `-hostfile` を使用します。どちらも動作しますが、`-hostfile`が推奨されます。

### 方法 2: -hosts オプションで直接指定

hostfileを使わずに、コマンドラインで直接ホストを指定：

```cmd
mpiexec -hosts 2 301A-17 PC2のホスト名 -c 4 monte_carlo_pi.exe
```

パラメータの意味：
- `-hosts 2`: 2台のホストを使用
- `301A-17 PC2のホスト名`: 使用するホスト名（スペース区切り）
- `-c 4`: 各ホストで4つのコアを使用

### 明示的にプロセス数を指定

```cmd
mpiexec -n 8 -hostfile hostfile.txt monte_carlo_pi.exe
```

### 認証情報を指定する場合

```cmd
mpiexec -user ユーザー名 -pwd パスワード -hostfile hostfile.txt monte_carlo_pi.exe
```

## トラブルシューティング

### エラー: "Failed RpcCliCreateContext error 1722" / "unable to connect to the smpd service"

**原因**: サービスが起動していない、またはhostfile.txtの記述が不適切
**解決策**:
1. **hostfile.txtで`localhost`を使用していないか確認**
   - `localhost`ではなく実際のホスト名を使用してください
   - 例: `301A-17:4` のように明示的に指定
2. 両方のPCでMS-MPI Launch Serviceが起動しているか確認
   ```cmd
   sc query MsMpiLaunchSvc
   ```
   起動していない場合：
   ```cmd
   sc start MsMpiLaunchSvc
   ```
3. ファイアウォール設定を確認

### エラー: "Unable to connect to hostname"

**原因**: ネットワーク接続の問題
**解決策**:
1. ping でホスト名/IPアドレスに疎通確認
   ```cmd
   ping DESKTOP-PC2
   ping 192.168.1.20
   ```
2. ファイアウォール設定を確認
3. MS-MPI Launch Serviceが起動しているか確認

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

### 動作確認とプロセスの割り振り確認

分散実行が正しく動作しているか確認するには、hello_mpi.exe を使用します。
各プロセスがどのホストで実行されているかが表示されます。

```cmd
mpiexec -hostfile hostfile.txt hello_mpi.exe
```

または：
```cmd
mpiexec -hosts 2 301A-17 PC2のホスト名 -c 4 hello_mpi.exe
```

出力例：
```
Hello from processor 301A-17, rank 0 out of 8 processes
Hello from processor 301A-17, rank 1 out of 8 processes
Hello from processor 301A-17, rank 2 out of 8 processes
Hello from processor 301A-17, rank 3 out of 8 processes
Hello from processor PC2のホスト名, rank 4 out of 8 processes
Hello from processor PC2のホスト名, rank 5 out of 8 processes
Hello from processor PC2のホスト名, rank 6 out of 8 processes
Hello from processor PC2のホスト名, rank 7 out of 8 processes
```

両方のホスト名が表示されれば、分散実行が正常に動作しています。

## パフォーマンスの考慮事項

- ネットワーク通信のオーバーヘッドにより、必ずしも2倍速くなるとは限りません
- monte_carlo_pi.c は通信量が少ない（最後のMPI_Reduceのみ）ため、分散実行の効果が出やすい
- より複雑な通信パターンを持つプログラムでは、ネットワーク速度が重要になります

## 参考

- MS-MPI 公式ドキュメント: https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi
- MS-MPI リリースノート: https://learn.microsoft.com/en-us/message-passing-interface/microsoft-mpi-release-notes
- MS-MPI 実行ガイド: https://github.com/microsoft/Microsoft-MPI/blob/master/docs/RunningMSMPI.md
- mpiexec のオプション一覧: `mpiexec -help2`

## 補足: smpd について

MS-MPI v7以降では、古い`smpd`サービスに代わって**MS-MPI Launch Service (MsMpiLaunchSvc)**が推奨されています。`smpd -install`コマンドは新しいバージョンではサポートされていません。分散実行には上記の方法5で説明したMS-MPI Launch Serviceまたはspmdデーモンを使用してください。
