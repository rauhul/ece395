clc; close all;
time = 0:1:1866;
ax = a(:,1);           %a is preloaded with accel and ypr data from arduino holding the board out in front of you with
ay = a(:,2);         %x pointing down and y pointing to the left
az = a(:,3);
gx = a(:,4);           
gy = a(:,5);         
gz = a(:,6);
yaw = a(:,7);           
pitch = a(:,8);         
roll = a(:,9);
AX=mean(ax(1:500));
AY=mean(ay(1:500));
AZ=mean(az(1:500));
GX=mean(gx(1:500));
GY=mean(gy(1:500));     %starting pt if first 501 values 
GZ=mean(gz(1:500));     %use rest of pts as new measurements
Yaw = mean(yaw(1:500));
Pitch = mean(pitch(1:500));
Roll = mean(roll(1:500));

figure;

%plot(time,yaw);
%hold on;
%plot(time,pitch);
plot(time,roll);
%%
Pini = [0 0;0 0];       %initial setup based on average values of around 1800 pts
dt= .001;               %saying 1 ms between every datapt but in reality prob much smaller than that
F = [1 -dt; 0 1];       % Qini based on something i saw online lol
Qini =[.001 0; 0 .003]; %X is a vector based on current angle and current gyroscope bias. 
Xk1k1 = [Roll; 0];      %Using roll as current angle and Gx as gyro direc.
H = [1 0];
R = .03;
%%
Xkk1 = F*Xk1k1 + [dt;0]*GX;              %Calculate current state based on prev estimate and gyro measurement

Pkk1 = F*Pini*transpose(F) + Qini;      %Calculate covariance matrix based on prev covar matrix


yk = roll(501) - H*Xkk1;                %diff between measured angle and new angle aka innovation

Sk = H*Pkk1*transpose(H) + R;           % innovation covariance


Kk = Pkk1*transpose(H)/Sk;              %Kalman Gain aka how much we trust innovation


xkk = Xkk1 + Kk*yk;                     %Update esimate of current state

Pkk = (eye(2) - Kk*H)*Pkk1;
%%
Xk1k1 = xkk;
Pini = Pkk;
Xkk1 = F*Xk1k1 + [dt;0]*GX;              %Calculate current state based on prev estimate and gyro measurement

Pkk1 = F*Pini*transpose(F) + Qini;      %Calculate covariance matrix based on prev covar matrix


yk = roll(501) - H*Xkk1;                %diff between measured angle and new angle aka innovation

Sk = H*Pkk1*transpose(H) + R;           % innovation covariance


Kk = Pkk1*transpose(H)/Sk;              %Kalman Gain aka how much we trust innovation


xkk = Xkk1 + Kk*yk;                     %Update esimate of current state

Pkk = (eye(2) - Kk*H)*Pkk1;

















