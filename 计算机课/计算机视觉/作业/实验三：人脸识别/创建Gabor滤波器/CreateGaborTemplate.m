function [RealTemplate,ImagTemplate,CombineTemplate]=CreateGaborTemplate(Diameter_x,Diameter_y,Band_sigma_x,Band_sigma_y,Frequency_fx,Frequency_fy,Rotation_sita)
[GridX,GridY]=meshgrid(-Diameter_x:Diameter_x,-Diameter_y:Diameter_y);
X=GridX*cos(Rotation_sita)+GridY*sin(Rotation_sita);
Y=-GridX*sin(Rotation_sita)+GridY*cos(Rotation_sita);
%ʵ������
RealTemplate=exp(-(   (X/Band_sigma_x).^2 + (Y/Band_sigma_y).^2             )).*cos( Frequency_fx*GridX +Frequency_fy*GridY  );
%�鲿����
ImagTemplate=exp(-(   (X/Band_sigma_x).^2 + (Y/Band_sigma_y).^2             )).*sin( Frequency_fx*GridX +Frequency_fy*GridY  );
%�����ϵ�ģ��
CombineTemplate=sqrt(RealTemplate.*RealTemplate + ImagTemplate.*ImagTemplate);












