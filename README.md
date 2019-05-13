# FFmpeg-CUDA
**最近领导布置一个用CUDA解码USB摄像头的任务，小白从头学习...**    
**实现流程： v4l2 -- ffmpeg -- opencv**    
**待更新...**      

-----
### Introduction
* Ubuntu16.04
* 需要编译opencv库和ffmpeg库 

### How to Use
#### **2019/5/13 Code/ff_camera.cpp**    

采用ffmpeg的CPU解码，并用opencv显示
```
cd Code
make 
./camera
```

------
### Reference 
* 首推雷神的各种资料
* **https://blog.csdn.net/leixiaohua1020/article/details/39702113**
* **https://blog.csdn.net/ice__snow/article/details/78608681**
* **https://blog.csdn.net/iamqianrenzhan/article/details/84830277**
* https://blog.csdn.net/zong596568821xp/article/details/80280390
* https://blog.csdn.net/scarecrow_wiscom/article/details/10614145#
* https://blog.csdn.net/teleger/article/details/80716947
* https://www.jianshu.com/p/6ef3c18d61b0
