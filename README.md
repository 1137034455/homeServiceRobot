# homeServiceRobot

## 介绍
家庭服务机器人主要完成的任务是人脸识别，人机交谈和一些简单的动作，比如握手、前后移动等。

这是家庭服务机器人的上位机程序，主要完成的功能包括以下：

* 人脸识别
* 人机交流
* 下位机控制

## 开发语言：

	1.C++
	2.C#

## 项目介绍
这个项目在Windows 7系统下使用Visual Studio 2010下编译成功，共分为三个子项目，分别如下：

### FRManager

这个模块主要完成人脸检测、人脸识别的功能。开发工具包为：OpenCV2.3.1。

### HSRobot

完成程序的主界面以及好友管理的功能。

### SRecoManager

完成语音识别和文字转语音(TTS)的功能。开发工具包为Microsoft Speech SDK。

## 运行
以管理员身份运行projectFiles\预设置.bat，主要是设置环境变量和注册人脸识别和语音识别的插件，然后执行projectFiles\bin\HSRobot.exe程序即可。

## 程序界面展示

![](http://7xoi5h.com1.z0.glb.clouddn.com/%E6%88%AA%E5%9B%BE20160311180544.jpg)

![](http://7xoi5h.com1.z0.glb.clouddn.com/%E6%88%AA%E5%9B%BE20160311174404.png)