# XYFSoftRenderer
一个简单的软光栅渲染器，GUI使用QT制作
## 实现功能
- MVP变换
- MSAA抗锯齿
- 多线程加速
- Bresenham画线算法
- 背面剔除
- Blinn-Phong Shader
- 纹理映射
- 齐次空间裁剪
- Obj模型加载
- 环绕相机
- 透视矫正
  
## 展示
纹理映射
![图片](/Image/纹理映射.PNG)
背面剔除
![图片](/Image/背面剔除.gif)
线框模式
![图片](/Image/线框.PNG)
MSAA
MSAA前
![图片](/Image/MSAA前.PNG) 
MSAA后
![图片](/Image/MSAA后.PNG) 
多线程加速
加速前
![图片](/Image/多线程前.PNG) 
加速后
![图片](/Image/多线程后.PNG) 
环绕相机
![图片](/Image/环绕相机.gif) 
调整光线角度
![图片](/Image/调整光线角度.gif) 
调整光线颜色
![图片](/Image/调整光线颜色.gif) 

## 第三方库
- [QT 6.2.4](https://www.qt.io/blog/qt-6.2.4-released)
- [assimp](https://github.com/assimp/assimp)
- [glm](https://github.com/Groovounet/glm)