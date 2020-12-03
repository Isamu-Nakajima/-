# ロボットシステム学
##  目標
作成したデバイスドライバーをベースに、LEDでモールス信号を表現する。

##  使用した環境・道具
- Raspberry Pi4 MdelB
- Ubuntu20.04
- 5mm LED　* 1
- 抵抗器　220Ω * 1
- ブレッドボード
- ジャンパー線 * 3

##  接続方法
<img src= "https://user-images.githubusercontent.com/72371336/100983243-66b89b00-358c-11eb-942b-5874da20c69c.jpeg" width="400" >
LEDのアノードをGPIO25をつなぎ、カソードをGRUNDにつなぐ。

##  実行方法
```
git clone https://github.com/Isamu-Nakajima/RobotSystem.git
cd RobotSystem/
make
sudo insmod myled.ko
sudo chmod 666 /dev/myled0
```
