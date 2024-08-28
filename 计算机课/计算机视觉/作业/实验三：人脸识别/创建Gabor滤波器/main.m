clear;clc;close all





Diameter_x=15;%Gabor滤波x方向半径大小
Diameter_y=15;%Gabor滤波y方向半径大小
Band_sigma_x=8;%x方向带宽
Band_sigma_y=2;%y方向带宽
Frequency_fx=1;%x方向中心调频
Frequency_fy=0.25;%y方向中心调频
Rotation_sita=3*pi/4;%

[RealTemplate,ImagTemplate,CombineTemplate]=CreateGaborTemplate(Diameter_x,Diameter_y,Band_sigma_x,Band_sigma_y,Frequency_fx,Frequency_fy,Rotation_sita);

[X,Y]=meshgrid(-Diameter_x:Diameter_x,-Diameter_y:Diameter_y);

figure
subplot(1,3,1)
surf(X,Y,RealTemplate)
xlabel('x')
ylabel('y')
title('Real')
subplot(1,3,2)
surf(X,Y,ImagTemplate)
xlabel('x')
ylabel('y')
title('imaginary')
subplot(1,3,3)
surf(X,Y,CombineTemplate)
xlabel('x')
ylabel('y')
title('combined')











