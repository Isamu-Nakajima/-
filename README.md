# ロボットシステム学
##  目標
作成したデバイスドライバーをベースに、LEDでモール信号を表現する。

##  使用した環境・道具
- Raspberry Pi4 MdelB
- Ubuntu20.04
- 5mm LED
- 抵抗器　220Ω
- ブレッドボード
- ジャンパー線 * 3

##  実行方法
```
git clone https://github.com/Isamu-Nakajima/RobotSystem.git
cd RobotSystem/
make
sudo insmod myled.ko
sudo chmod 666 /dev/myled0
```
